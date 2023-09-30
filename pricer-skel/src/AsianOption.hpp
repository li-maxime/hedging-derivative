#pragma once

#include "Option.hpp"

class AsianOption :public Option
{
  public:
    double K_;
    PnlVect* weights;
    AsianOption(double T_, int nbTimeSteps_,int size_,PnlVect* lambdas, double strike);
    double payoff(const PnlMat* path) override;
};


