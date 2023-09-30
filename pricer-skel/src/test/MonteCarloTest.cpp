#include "gtest/gtest.h"
#include "../BlackScholesModel.hpp"
#include "../PerformanceOption.hpp"
#include "../MonteCarlo.hpp"
#include "../BasketOption.hpp"

TEST(MonteCarlo, deltaComputeSum){
    auto bs_model = new BlackScholesModel(5,0.02,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    BasketOption* option = new BasketOption(3.0, 4, 5,0,weights);

    auto mc = new MonteCarlo(bs_model, option, pnl_rng_create(PNL_RNG_MERSENNE),2.5, 50000,1);
    pnl_rng_sseed(mc->rng_, 1);

    PnlVect *delta = pnl_vect_create_from_zero(5);
    PnlVect *std_dev = pnl_vect_create_from_zero(5);

    auto path = pnl_mat_create_from_scalar(5,5,1.0);
    auto shift_path = pnl_mat_new();
    mc->deltaComputeSum(delta, std_dev, path, shift_path, 0);
    PnlVect* expected_result = pnl_vect_create_from_scalar(5, 1.0);
    ASSERT_EQ(pnl_vect_isequal(delta, expected_result, 0.01), 1);
    ASSERT_EQ(pnl_vect_isequal(std_dev, expected_result, 0.01), 1);
}
//Test using perf.json
TEST(MonteCarlo, price){
    auto bs_model = new BlackScholesModel(5,0.03,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    PerformanceOption* opt = new PerformanceOption(weights, 2.0, 12, 5);


    auto mc = new MonteCarlo(bs_model, opt, pnl_rng_create(PNL_RNG_MERSENNE),0.1, 50000,1);
    pnl_rng_sseed(mc->rng_, 42);

    double prix = 0.0;
    double std_dev = 0.0;
    mc->price(prix, std_dev);
    //Intervalle de confiance à 95%
    ASSERT_NEAR(prix, 1.2573, 1.96*std_dev);


}
//Test using perf.json
TEST(MonteCarlo, delta){
    auto bs_model = new BlackScholesModel(5,0.03,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    PerformanceOption* opt = new PerformanceOption(weights, 2.0, 12, 5);


    auto mc = new MonteCarlo(bs_model, opt, pnl_rng_create(PNL_RNG_MERSENNE),0.1, 50000,1);
    pnl_rng_sseed(mc->rng_, 42);

    PnlVect *delta = pnl_vect_create_from_zero(5);
    PnlVect *StdDev = pnl_vect_create_from_zero(5);
    mc->delta(delta, StdDev);
    PnlVect* expected_result = pnl_vect_create_from_list(5, -5.40574e-07, -4.34042e-07, -1.45947e-07, 1.47271e-06, -1.12195e-06);
    for (int i = 0; i<opt->size_; i++){
        //Intervalle de confiance à 95%
        ASSERT_NEAR(GET(delta, i), GET(expected_result, i), 1.96*GET(StdDev, i));
    }
}

TEST(MonteCarlo, price_t){
    auto bs_model = new BlackScholesModel(5,0.03,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    PerformanceOption* opt = new PerformanceOption(weights, 2.0, 12, 5);


    auto mc = new MonteCarlo(bs_model, opt, pnl_rng_create(PNL_RNG_MERSENNE),0.1, 50000,1);
    pnl_rng_sseed(mc->rng_, 42);

    auto old_spot = pnl_mat_create_from_scalar(1,5,100.0);
    double prix = 0.0;
    double std_dev = 0.0;
    mc->price(old_spot, 0, prix, std_dev);
    //Intervalle de confiance à 95%
    ASSERT_NEAR(prix, 1.2573, 1.96*std_dev);


}

TEST(MonteCarlo, delta_t){
    auto bs_model = new BlackScholesModel(5,0.03,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    PerformanceOption* opt = new PerformanceOption(weights, 2.0, 12, 5);


    auto mc = new MonteCarlo(bs_model, opt, pnl_rng_create(PNL_RNG_MERSENNE),0.1, 50000,1);
    pnl_rng_sseed(mc->rng_, 42);

    auto old_spot = pnl_mat_create_from_scalar(1,5,100.0);

    PnlVect *delta = pnl_vect_create_from_zero(5);
    PnlVect *StdDev = pnl_vect_create_from_zero(5);
    mc->delta(old_spot, 0, delta, StdDev);
    PnlVect* expected_result = pnl_vect_create_from_list(5, -5.40574e-07, -4.34042e-07, -1.45947e-07, 1.47271e-06, -1.12195e-06);
    for (int i = 0; i<opt->size_; i++){
        //Intervalle de confiance à 95%
        ASSERT_NEAR(GET(delta, i), GET(expected_result, i), 1.96*GET(StdDev, i));
    }
}

//Test price at t>0:

TEST(MonteCarlo, price_at_t){
    //Initialisation du modèle de B&S
    auto blackScholesModel = new BlackScholesModel(5, 0.04879, 0.0, pnl_vect_create_from_scalar(5, 0.2), pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    auto rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 42);

    //Creation de l'option 1 dont on calcul le prix à t=0
    auto basketOption1 = new BasketOption(1, 1, 5, 100.0, weights);

    //Creation de l'option 2 dont on calcul le prix à t=2 pour avoir le même TimeToMaturity que l'option 1
    auto basketOption2 = new BasketOption(3, 1, 5, 100.0, weights);

    //On initie la matrice des Past telle que les valeurs dans la dernière ligne sont les mêmes que celles de la ligne 0 pour le path de l'option 1
    PnlMat* old_spots = pnl_mat_create_from_list(2, 5,
                                                 0.0,     0.0,      0.0,      0.0,      0.0,
                                                 100.0,     100.0,      100.0,      100.0,      100.0);



    auto mc = new MonteCarlo(blackScholesModel, basketOption1, rng,0.1, 500000,1);

    auto priceRef = 0.0;
    auto stdDev = 0.0;

    mc->price(priceRef, stdDev);


    mc->opt_ = basketOption2;

    auto price2 = 0.0;
    auto dev = 0.0;

    //On test le prix à t=2 pour une option de maturité 3 puisque celle-ci a le même TimeToMaturity que l'option 1
    mc->price(old_spots, 2, price2, dev);


    //Test si la valeur est dans l'interval de confiance à 95%
    ASSERT_NEAR(priceRef, price2, stdDev *1.96);
}

//Test delta at t>0:

TEST(MonteCarlo, delta_at_t){
    //Initialisation du modèle de B&S
    auto blackScholesModel = new BlackScholesModel(5, 0.04879, 0.0, pnl_vect_create_from_scalar(5, 0.2), pnl_vect_create_from_scalar(5, 100.0));
    PnlVect* weights = pnl_vect_create_from_scalar(5, 0.2);
    auto rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 42);

    //Creation de l'option 1 dont on calcul le prix à t=0
    auto basketOption1 = new BasketOption(1, 1, 5, 100.0, weights);

    //Creation de l'option 2 dont on calcul le prix à t=2 pour avoir le même TimeToMaturity que l'option 1
    auto basketOption2 = new BasketOption(3, 1, 5, 100.0, weights);

    //On initie la matrice des Past telle que les valeurs dans la dernière ligne sont les mêmes que celles de la ligne 0 pour le path de l'option 1
    PnlMat* old_spots = pnl_mat_create_from_list(2, 5,
                                                 0.0,     0.0,      0.0,      0.0,      0.0,
                                                 100.0,     100.0,      100.0,      100.0,      100.0);



    auto mc = new MonteCarlo(blackScholesModel, basketOption1, rng,0.1, 500000,1);

    auto deltaRef = pnl_vect_create_from_zero(5);
    auto stdDev = pnl_vect_create_from_zero(5);


    mc->delta(deltaRef, stdDev);


    mc->opt_ = basketOption2;

    auto delta = pnl_vect_create_from_zero(5);
    auto stdDev2 = pnl_vect_create_from_zero(5);

    //On test le delta à t=2 pour une option de maturité 3 puisque celle-ci a le même TimeToMaturity que l'option 1
    mc->delta(old_spots, 2, delta, stdDev2);



    //Test si la valeur est dans l'interval de confiance à 95%
    for (int i = 0; i<5; i++){
        //Intervalle de confiance à 95%
        ASSERT_NEAR(GET(deltaRef, i), GET(delta, i), 1.96*GET(stdDev2, i));
    }
}