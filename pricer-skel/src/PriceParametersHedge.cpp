#include "PriceParametersHedge.hpp"

void PriceParametersHedge::parse_hedge(std::string input_data) {
    std::ifstream ifs(input_data);
    nlohmann::json j = nlohmann::json::parse(ifs);

    PriceParameters::parse_price_zero(j);

    j.at("hedging dates number").get_to(hedging_dates_number);
    j.at("trend").get_to(trend);
    if (trend->size == 1 && hedging_dates_number> 1){
        pnl_vect_resize_from_scalar(trend, hedging_dates_number, GET(trend,0));
    }
    j.at("fd step").get_to(fd_step);
}

void PriceParametersHedge::free_params_hedge() {
    PriceParameters::free_params_price_zero();
    if (trend && trend->size > 1) {
        pnl_vect_free(&trend);
    }
}