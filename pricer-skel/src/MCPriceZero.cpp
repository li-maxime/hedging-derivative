#include <iostream>
#include "json_helper.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "AsianOption.hpp"
#include "BasketOption.hpp"
#include "PerformanceOption.hpp"
#include "PriceParameters.hpp"
#include "PricingResults.hpp"
#include "pnl/pnl_vector.h"


int main(int argc, char **argv) {
    double fd_step = 0.1;
    auto* params = new PriceParameters();
    params->parse_price_zero_wrap(argv[1]);
    auto rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 42);
     BlackScholesModel* model;
     if ((params->model_type) == "bs"){
         model = new BlackScholesModel(params->option_size, params->interest_rate, params->correlation, params->volatility, params->spot);
     }

    Option* option;
     if ((params->option_type) == "asian"){
         auto* new_option = new AsianOption(params->maturity, params->timestep_number, params->option_size, params->payoff_coefficients, params->strike);
         option = new_option;
     }
     else if ((params->option_type) == "basket"){
         auto* new_option = new BasketOption(params->maturity, params->timestep_number, params->option_size, params->strike, params->payoff_coefficients);
         option = new_option;
     }
     else if ((params->option_type) == "performance"){
         auto* new_option = new PerformanceOption(params->payoff_coefficients, params->maturity, params->timestep_number, params->option_size);
         option = new_option;
     }
     else{
         exit(0);
     }

    auto mc = new MonteCarlo(model, option, rng, fd_step, params->sample_number ,option->nbTimeSteps_);

    double prix = 0.0;
    double std_dev = 0.0;

    mc->price(prix, std_dev);

    PnlVect* delta = pnl_vect_create_from_zero(params->option_size);
    PnlVect* deltaStdDev = pnl_vect_create_from_zero(params->option_size);
    mc->delta(delta, deltaStdDev);


    PricingResults res(prix, std_dev, delta, deltaStdDev);
    std::cout << res << std::endl;

    params->free_params_price_zero();
    pnl_rng_free(&rng);
    delete(model);
    delete(option);
    delete(mc);
    delete(params);
    exit(0);
}
