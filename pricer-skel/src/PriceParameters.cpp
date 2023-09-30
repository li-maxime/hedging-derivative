//
// Created by nguyenan on 12/09/2023.
//
#include "PriceParameters.hpp"
#include "nlohmann/json.hpp"

void PriceParameters::parse_price_zero_wrap(std::string input_data) {
    std::ifstream ifs(input_data);
    nlohmann::json j = nlohmann::json::parse(ifs);
    parse_price_zero(j);

}

void PriceParameters::parse_price_zero(nlohmann::json j){
    model_type=j.at("model type").get<std::string>();
    j.at("option size").get_to(option_size);
    if (j.contains("strike")){
        j.at("strike").get_to(strike);
    }
    j.at("spot").get_to(spot);
    if (spot->size ==1 && option_size > 1){
        pnl_vect_resize_from_scalar(spot, option_size, GET(spot, 0));
    }
    j.at("maturity").get_to(maturity);
    j.at("volatility").get_to(volatility);
    if (volatility->size == 1 && option_size > 1){
        pnl_vect_resize_from_scalar(volatility, option_size, GET(volatility,0));
    }
    j.at("interest rate").get_to(interest_rate);
    j.at("correlation").get_to(correlation);
    option_type = j.at("option type").get<std::string>();
    j.at("payoff coefficients").get_to(payoff_coefficients);
    if (payoff_coefficients->size == 1 && option_size > 1){
        pnl_vect_resize_from_scalar(payoff_coefficients, option_size, GET(payoff_coefficients,0));
    }
    j.at("timestep number").get_to(timestep_number);
    j.at("sample number").get_to(sample_number);
}
void PriceParameters::free_params_price_zero() {
    if (spot && spot->size > 1) {
        pnl_vect_free(&spot);
    }
    if (volatility && volatility->size > 1) {
        pnl_vect_free(&volatility);
    }
    if (payoff_coefficients && payoff_coefficients->size > 1) {
        pnl_vect_free(&payoff_coefficients);
    }
}
