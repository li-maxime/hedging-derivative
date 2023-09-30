#include <iostream>
#include "Option.hpp"
#include "BlackScholesModel.hpp"
#include "pnl/pnl_random.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarlo.hpp"
/**
     * Calcule le prix de l'option à la date 0
     *
     * @param[out] prix valeur de l'estimsateur Monte Carlo
     * @param[out] std_dev écart type de l'estimateur
 */
void MonteCarlo :: price(double& prix, double& std_dev)
{
    auto sum = 0.0;
    auto square_sum = 0.0;
    auto nb_steps = opt_->nbTimeSteps_;
    double payoff_result;

    {
        PnlMat* path = pnl_mat_create_from_zero(nb_steps + 1, mod_->size_);
        for (int i = 0; i < nbSamples_; ++i)
        {
            mod_->asset(path, opt_->T_, nb_steps, rng_);
            payoff_result = opt_->payoff(path);
            sum += payoff_result;
            square_sum += payoff_result * payoff_result;
        }
        pnl_mat_free(&path);

    }
    prix = exp(-1 * mod_->r_ * opt_->T_) * sum / nbSamples_;
    std_dev = sqrt(exp(-2 * mod_->r_ * opt_->T_) * (square_sum / nbSamples_ - (prix) * (prix)));
    std_dev = std_dev/sqrt(nbSamples_);
    //pnl_mat_free(&path);

}
MonteCarlo::MonteCarlo(BlackScholesModel* mod, Option* opt, PnlRng* rng, double fdStep, long nbSamples, int nbHedgingDates)
{
    mod_ = mod;
    opt_ = opt;
    rng_ = rng;
    fdStep_ = fdStep;
    nbSamples_ = nbSamples;
    nbHedgingDates_ = nbHedgingDates;
    computeHelper = pnl_vect_create(opt_->size_);
}


/**
     * Calcule le prix de l'option à la date t
     *
     * @param[in]  past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] prix contient le prix
     * @param[out] std_dev contient l'écart type de l'estimateur
 */
void MonteCarlo :: price(const PnlMat* past, double t, double& prix, double& std_dev)
{
    auto sum = 0.0;
    auto square_sum = 0.0;
    auto nb_steps = opt_->nbTimeSteps_;
    double payoff_result;
    {
        PnlMat* path = pnl_mat_create_from_zero(nb_steps + 1, mod_->size_);
        for (int i = 0; i < nbSamples_; ++i)
        {
            mod_->asset(path, t, opt_->T_, nb_steps, rng_, past);
            payoff_result = opt_->payoff(path);
            sum += payoff_result;
            square_sum += payoff_result * payoff_result;
        }
        pnl_mat_free(&path);
    }
    prix = exp(-1 * mod_->r_ * (opt_->T_ - t)) * sum /(double) nbSamples_;
    std_dev = sqrt(exp(-2 * mod_->r_ * (opt_->T_ - t)) * (square_sum /(double) nbSamples_ - (prix) * (prix)));
    std_dev = std_dev/sqrt(nbSamples_);
}

/**
     * Calcule le delta de l'option à la date t
     *
     * @param[in] past contient la trajectoire du sous-jacent
     * jusqu'à l'instant t
     * @param[in] t date à laquelle le calcul est fait
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
 */
void MonteCarlo::delta(const PnlMat* past, double t, PnlVect* delta, PnlVect* std_dev)
{
    auto nb_steps = opt_->nbTimeSteps_;
    {
        PnlMat* path = pnl_mat_create(nb_steps + 1, mod_->size_);
        PnlMat * shift_path = pnl_mat_create(nb_steps + 1, mod_->size_);
        for (int i = 0; i < nbSamples_; i++)
        {
            mod_->asset(path, t,opt_->T_, nb_steps, rng_, past);
            deltaComputeSum(delta, std_dev, path, shift_path, t);
        }
        pnl_mat_free(&path);
        pnl_mat_free(&shift_path);
    }
    auto spot = mod_->spot_;
    deltaComputeValue(delta, std_dev, spot,t);

}


/**
     * Calcule le delta de l'option à la date 0
     *
     * @param[out] delta contient le vecteur de delta
     * @param[out] std_dev contient l'écart type de l'estimateur
 */
void MonteCarlo :: delta(PnlVect* delta, PnlVect* std_dev)
{
    auto nb_steps = opt_->nbTimeSteps_;

    {
        PnlMat* path = pnl_mat_create(nb_steps + 1, mod_->size_);
        PnlMat * shift_path = pnl_mat_create(nb_steps + 1, mod_->size_);
        for (int i = 0; i < nbSamples_; i++)
        {
            mod_->asset(path, opt_->T_, nb_steps, rng_);
            deltaComputeSum(delta, std_dev, path, shift_path, 0);
        }
        pnl_mat_free(&path);
        pnl_mat_free(&shift_path);
    }
    auto spot = mod_->spot_;
    deltaComputeValue(delta, std_dev, spot,0);

}

void MonteCarlo :: deltaComputeSum(PnlVect* delta, PnlVect* std_dev, PnlMat* path, PnlMat* shift_path, double t){
    double difference;
    double intermediate_value;
    for (int d = 0; d < opt_->size_; d++)
    {
        mod_->shiftAsset(shift_path, path, d, fdStep_, t, opt_->T_ / opt_->nbTimeSteps_);
        difference = opt_->payoff(shift_path);
        mod_->shiftAsset(shift_path, path, d, -fdStep_, t, opt_->T_ / opt_->nbTimeSteps_);
        difference = difference - opt_->payoff(shift_path);

        {
            intermediate_value = GET(delta, d) + difference;
            LET(delta, d) = intermediate_value;
        }

        {
            intermediate_value = GET(std_dev, d) + difference * difference;
            LET(std_dev, d) = intermediate_value;
        }


    }
}

void MonteCarlo :: deltaComputeValue(PnlVect* delta, PnlVect* std_dev, PnlVect* spot_t, double t){
    pnl_vect_div_scalar(delta, 2 * fdStep_ * nbSamples_);
    pnl_vect_div_vect_term(delta, spot_t);

    pnl_vect_clone(computeHelper,delta);
    pnl_vect_mult_vect_term(computeHelper, computeHelper);

    pnl_vect_div_scalar(std_dev, (2 * fdStep_ ) * (2 * fdStep_ ) * nbSamples_);
    pnl_vect_div_vect_term(std_dev, spot_t);
    pnl_vect_div_vect_term(std_dev, spot_t);

    pnl_vect_minus_vect(std_dev, computeHelper);

    pnl_vect_mult_scalar(delta, exp(-1 * mod_->r_ * (opt_->T_ - t)));
    pnl_vect_mult_scalar(std_dev, exp(-2 * mod_->r_ * (opt_->T_ - t)) / nbSamples_);
    pnl_vect_map_inplace(std_dev, sqrt);
}
MonteCarlo::~MonteCarlo()
{
    pnl_vect_free(&computeHelper);
}
