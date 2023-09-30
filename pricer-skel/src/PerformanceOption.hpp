//
// Created by nguyenan on 08/09/2023.
//

#pragma once
#include "Option.hpp"
class PerformanceOption : public Option {
    PnlVect *weights_;
public :
    PerformanceOption(PnlVect *weights, double maturity, int steps, int size);
    double payoff(const PnlMat* path) override;
    PnlVect *current;
    ~PerformanceOption();
};


