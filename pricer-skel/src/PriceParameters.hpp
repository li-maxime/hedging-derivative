#include <string>
#include <fstream>
#include "json_helper.hpp"
#include "pnl/pnl_vector.h"

#ifndef MC_PRICER_PRICEPARAMETERS_HPP
#define MC_PRICER_PRICEPARAMETERS_HPP

class PriceParameters {
protected:
    void parse_price_zero(nlohmann::json j);
public :
    std::string model_type;
    int option_size;
    PnlVect* spot;
    double maturity;
    PnlVect* volatility;
    double interest_rate;
    double correlation;
    std::string option_type;
    PnlVect* payoff_coefficients;
    int timestep_number;
    int sample_number;
    double strike;
    void parse_price_zero_wrap(std::string);
    void free_params_price_zero();
};


#endif //MC_PRICER_PRICEPARAMETERS_HPP
