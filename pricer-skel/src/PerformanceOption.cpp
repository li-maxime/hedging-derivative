//
// Created by nguyenan on 08/09/2023.
//

#include "PerformanceOption.hpp"
#include <algorithm>
#include <iostream>
PerformanceOption::PerformanceOption(PnlVect* weights, double maturity, int steps, int size){
    weights_ = weights;
    T_ = maturity;
    nbTimeSteps_ = steps;
    size_ = size;
    this->current = pnl_vect_create_from_zero(size_);
}
double
PerformanceOption::payoff(const PnlMat *path){
    double pay = 1.0;
    double numerator = 0.0;
    pnl_mat_get_row(current, path, 0);
    double denominator = pnl_vect_scalar_prod(current, weights_);

    for (int i = 1; i < path->m; i++){
        pnl_mat_get_row(current, path, i);
        numerator = pnl_vect_scalar_prod(current, weights_);
        pay += std::max(0.0, (numerator / denominator) - 1.0);
        denominator = numerator;
    }
    return pay;

}

PerformanceOption::~PerformanceOption()
{
    pnl_vect_free(&current);
}

