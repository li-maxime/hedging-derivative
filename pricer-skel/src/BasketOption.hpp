#pragma once

#include "Option.hpp"

class BasketOption :public Option
{
  public:
    double strike;
    PnlVect* weight;
    BasketOption(double T_, int nbTimeSteps_,int size_,double strike,PnlVect* weight);
    double payoff(const PnlMat* path) override;
};


