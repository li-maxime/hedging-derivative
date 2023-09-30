
#include "Option.hpp" // Include the base class header file
#include "pnl/pnl_vector.h"
#include "AsianOption.hpp"
#include <algorithm>


    AsianOption :: AsianOption(double T, int nbTimeSteps, int size, PnlVect* lambdas, double strike)
    {
        T_ = T;
        nbTimeSteps_ = nbTimeSteps;
        size_ = size;
        this->K_ = strike;
        this->weights = lambdas;
    }

    double
    AsianOption ::payoff(const PnlMat* path)
    {
        double result= 0.0;
        double N_ =path->m;
        double D_ = path->n;
        for (int t_i = 0; t_i < N_; t_i++)
        {
            for (int d = 0; d < D_; d++)
            {
                result += GET(this->weights,d) * MGET(path, t_i, d);
            }
        }
        result = (1.0 / (N_))*result;
        return std::max(result-K_,0.0);
    }

