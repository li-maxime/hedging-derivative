#pragma once
#include "pnl/pnl_vector.h"

class Portfolio
{
  public :
    double portfolio_value;
    PnlVect* composition;
    void update(PnlVect* new_compo, _PnlVect* pVect);
    void update_portfolio_value(double r, double T, double H, PnlVect* new_compo, PnlVect* spots);
    Portfolio(double initial_value, int size_); // nb risky asset + risk free asset
    ~Portfolio();

    double get_portfolio_total_value(PnlVect* spots);
    double get_pnl(PnlVect* spots, double payoff);
};

