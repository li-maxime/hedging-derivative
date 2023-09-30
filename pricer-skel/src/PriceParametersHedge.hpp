#pragma once

#include "PriceParameters.hpp"

class PriceParametersHedge : public PriceParameters{
public:
    PnlVect* trend;
    int hedging_dates_number;
    double fd_step;

    void parse_hedge(std::string);
    void free_params_hedge();
};


