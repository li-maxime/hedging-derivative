
#include <iostream>
#include <algorithm>
#include "Option.hpp"
#include "BasketOption.hpp"

BasketOption::BasketOption(double T_, int nbTimeSteps_, int size_, double strike, PnlVect* weight)
{
    this->T_ = T_;
    this->nbTimeSteps_ = nbTimeSteps_;
    this->size_ = size_;
    this->strike = strike;
    this->weight = weight;
}

double
BasketOption::payoff(const PnlMat* path)
{
    PnlVect* final_value = pnl_vect_create_from_zero(size_);
    pnl_mat_get_row(final_value ,path, path->m-1);
    double weighted_sum = pnl_vect_scalar_prod(final_value, weight);
    pnl_vect_free(&final_value);
    return std::max(weighted_sum-strike,0.0);
}



