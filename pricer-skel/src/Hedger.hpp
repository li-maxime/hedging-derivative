#ifndef MC_PRICER_HEDGER_HPP
#define MC_PRICER_HEDGER_HPP

#include "Portfolio.hpp"
#include "pnl/pnl_vector.h"
#include "MonteCarlo.hpp"

class Hedger
{
  public:
    Portfolio* pf_;
    MonteCarlo* pricer_;
    Hedger(Portfolio* pf, MonteCarlo* pricer);
    double hedge(PnlMat* market_data);
};

#endif //MC_PRICER_HEDGER_HPP
