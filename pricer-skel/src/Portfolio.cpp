#include "Portfolio.hpp"
#include <math.h>

Portfolio :: Portfolio(double initial_value, int size_)
{
    this->portfolio_value = initial_value;
    this->composition = pnl_vect_create_from_zero(size_);
}

void
Portfolio ::update(PnlVect* new_compo, _PnlVect* pVect) {
    this->portfolio_value = this->portfolio_value - pnl_vect_scalar_prod(new_compo, pVect);
    pnl_vect_clone(this->composition, new_compo);
}


void
Portfolio ::update_portfolio_value(double r, double T, double H, PnlVect* new_compo, PnlVect* spots)
{
    pnl_vect_minus_vect(this->composition, new_compo);
    this->portfolio_value = this->portfolio_value * exp(r * T / H)  + pnl_vect_scalar_prod(this->composition, spots);
    pnl_vect_clone(this->composition, new_compo);
}

double Portfolio :: get_pnl(PnlVect* spots, double payoff){
    return this->portfolio_value + pnl_vect_scalar_prod(this->composition, spots) - payoff;
}
Portfolio::~Portfolio()
{
    pnl_vect_free(&composition);
}
