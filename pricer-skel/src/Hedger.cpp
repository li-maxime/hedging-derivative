
#include "Hedger.hpp"
#include <iostream>


Hedger :: Hedger(Portfolio* pf, MonteCarlo* pricer)
{
    this->pf_ = pf;
    this->pricer_ = pricer;
}

double
Hedger ::  hedge(PnlMat* MarketData)
{
    // compute initial value
    auto r = this->pricer_->mod_->r_;
    auto T = this->pricer_->opt_->T_;
    auto dH = T/this->pricer_->nbHedgingDates_;
    auto ratio = this->pricer_->nbHedgingDates_/this->pricer_->opt_->nbTimeSteps_;

    auto spots = pnl_vect_wrap_mat_row(MarketData, 0);
    auto initial_price = 0.0;
    auto initial_std_dev = 0.0;

    this->pricer_->mod_->spot_ = &spots;
    this->pricer_->price(initial_price, initial_std_dev);


    auto delta = pnl_vect_create_from_zero(this->pricer_->opt_->size_);
    auto std_dev = pnl_vect_create_from_zero(this->pricer_->opt_->size_);
    this->pricer_->delta(delta, std_dev);

    this->pf_->portfolio_value = initial_price;
    this->pf_->update(delta, &spots);


    auto past = pnl_mat_create_from_zero(this->pricer_->opt_->nbTimeSteps_ + 1, this->pricer_->opt_->size_);
    pnl_mat_set_row(past, &spots, 0);
    auto j = 1;
    // compute for t > 0
    for (int i = 1; i < MarketData->m; i++)
    {
        pnl_vect_set_zero(delta);
        pnl_vect_set_zero(std_dev);
        spots = pnl_vect_wrap_mat_row(MarketData, i);
        pnl_mat_set_row(past, &spots, j);
        auto t = dH*i;
        this->pricer_->delta(past, t, delta, std_dev);
        pf_->update_portfolio_value(r, T, this->pricer_->nbHedgingDates_, delta, &spots);
        if (i % ratio == 0){
            j++;
        }
    }

    double pnl = this->pf_->portfolio_value + pnl_vect_scalar_prod(delta, &spots) - this->pricer_->opt_->payoff(past);
    pnl_mat_free(&past);
    pnl_vect_free(&delta);
    pnl_vect_free(&std_dev);

    return pnl;
}

