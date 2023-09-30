#include <iostream>
#include "HedgingResults.hpp"
#include "BlackScholesModel.hpp"
#include "MonteCarlo.hpp"
#include "AsianOption.hpp"
#include "BasketOption.hpp"
#include "PerformanceOption.hpp"
#include "PriceParametersHedge.hpp"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "Hedger.hpp"

int main(int argc, char **argv) {
    auto* params = new PriceParametersHedge();

    PnlMat* market_data = pnl_mat_create_from_file(argv[1]);
    params->parse_hedge(argv[2]);

    auto rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng, 43);

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

    auto mc = new MonteCarlo(model, option, rng, params->fd_step, params->sample_number, params->hedging_dates_number);

    double prix = 0.0;
    double std_dev = 0.0;

    mc->price(prix, std_dev);

    auto portfolio = new Portfolio(prix, params->option_size);
    auto hedge = new Hedger(portfolio, mc);
    double pnl = hedge->hedge(market_data);

    HedgingResults res(prix, std_dev, pnl);
    std::cout << res << std::endl;

    params->free_params_hedge();


    delete(model);
    delete(option);
    delete(mc);
    pnl_rng_free(&rng);
    delete(params);
    pnl_mat_free(&market_data);
    delete(hedge);
    delete(portfolio);
    exit(0);
}
