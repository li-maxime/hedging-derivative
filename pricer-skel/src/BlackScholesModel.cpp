
#include "pnl/pnl_random.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "BlackScholesModel.hpp"
#include "cmath"
#include <algorithm>

/// \brief Modèle de Black Scholes


BlackScholesModel::BlackScholesModel(int size, double r, double rho, PnlVect* sigma, PnlVect* spot)
{
    size_ = size;
    r_ = r;
    rho_ = rho;
    sigma_ = sigma;
    spot_ = spot;
    matrix_correlations_ = pnl_mat_create_from_scalar(size_, size_, rho_);
    pnl_mat_set_diag(matrix_correlations_, 1.0, 0);
    pnl_mat_chol(matrix_correlations_);
    this-> G = pnl_vect_create_from_zero(size_);
    this-> St = pnl_vect_create_from_zero(size_);
}
/**
     * Génère une trajectoire du modèle et la stocke dans path
     *
     * @param[out] path contient une trajectoire du modèle.
     * C'est une matrice de taille (nbTimeSteps+1) x d
     * @param[in] T  maturité
     * @param[in] nbTimeSteps nombre de dates de constatation
 */

void BlackScholesModel :: asset(PnlMat* path, double T, int nbTimeSteps, PnlRng* rng) const
{
    auto dt = T / ((double) nbTimeSteps);
    pnl_mat_set_row(path, spot_, 0);
    auto current_spot = pnl_vect_copy(spot_);
    for(int i = 1; i<nbTimeSteps+1; i++){
        stepSimulation(current_spot, dt, rng);
        pnl_mat_set_row(path, current_spot, i);
        current_spot = current_spot;
    }
    pnl_vect_free(&current_spot);

}


/**
     * Calcule une trajectoire du modèle connaissant le
     * passé jusqu' à la date t
     *
     * @param[out] path  contient une trajectoire du sous-jacent. La partie jusqu'à
     * l'instant t est données par la matrice past
     * @param[in] t date jusqu'à laquelle on connait la trajectoire.
     * t n'est pas forcément une date de discrétisation
     * @param[in] nbTimeSteps nombre de pas de constatation
     * @param[in] T date jusqu'à laquelle on simule la trajectoire
     * @param[in] past trajectoire réalisée jusqu'a la date t
 */
void BlackScholesModel :: asset(PnlMat* path, double t, double T, int nbTimeSteps, PnlRng* rng, const PnlMat* past)
{
    double dt = T/(double) nbTimeSteps;

    //Copy past into path excluding step t
    pnl_mat_set_subblock(path, past, 0,0);

    //Simulating step just after t

    int next_iteration = ceil(t/dt);


    pnl_mat_get_row(St,past, next_iteration);


    double time_until_next_step = std::max(dt * next_iteration - t, 0.0);
    stepSimulation(St, time_until_next_step,rng);
    pnl_mat_set_row(path, St,next_iteration);

    //Simulating rest of steps
    int start = next_iteration + 1;
    for(int i = start; i<nbTimeSteps + 1; i++){
        stepSimulation(St, dt, rng);
        pnl_mat_set_row(path, St, i);
    }
}

void BlackScholesModel :: stepSimulation(PnlVect* current_spot,double dt,PnlRng* rng) const{
    pnl_vect_rng_normal_d(G, size_, rng);
    PnlVect* to_prod = pnl_vect_new();
    double sigma_d ;
    double si_m1_d ;
    double spot_value;
    for (int d = 0; d < size_; d++)
    {
        sigma_d = GET(sigma_, d);
        si_m1_d = GET(current_spot, d);
        pnl_mat_get_row(to_prod, matrix_correlations_, d);
        double random_term = pnl_vect_scalar_prod(to_prod, G);
        spot_value = si_m1_d * exp((r_ - (sigma_d * sigma_d / 2)) * dt + sigma_d * sqrt(dt) * random_term);
        pnl_vect_set(current_spot, d, spot_value);
    }
    pnl_vect_free(&to_prod);
}


/**
     * Shift d'une trajectoire du sous-jacent
     *
     * @param[in]  path contient en input la trajectoire
     * du sous-jacent
     * @param[out] shift_path contient la trajectoire path dont la
     * composante d a été shiftée par (1+h) à partir de la date t.
     * @param[in] t date à partir de laquelle on shift
     * @param[in] h pas de différences finies
     * @param[in] d indice du sous-jacent à shifter
     * @param[in] timestep pas de constatation du sous-jacent
 */
void BlackScholesModel ::shiftAsset(PnlMat* shift_path, const PnlMat* path, int d, double h, double t, double timestep){
    pnl_mat_clone(shift_path, path);
    int start = ceil(t/timestep);
    for (int i = start; i<path->m; i++){
        MLET(shift_path, i, d) = MGET(shift_path, i, d) * (1+h);
    }
}
BlackScholesModel::~BlackScholesModel()
{
    pnl_mat_free(&matrix_correlations_);
    pnl_vect_free(&G);
    pnl_vect_free(&St);
}
