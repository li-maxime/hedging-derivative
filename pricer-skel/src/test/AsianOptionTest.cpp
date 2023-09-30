#include "gtest/gtest.h"
#include <iostream>
#include "../AsianOption.hpp"

TEST(AsianOption, Payoff){
    PnlVect * weight = pnl_vect_create_from_list(5,0.2,0.2,0.2,0.2,0.2);
    AsianOption option(5.0, 5, 5,weight, 100);
    PnlMat* path = pnl_mat_create_from_list(5, 5, 100.0,100.0,100.0,100.0,100.0,
                                            100.5873319,100.579641,100.7505747,101.2413343,101.0350516,
                                            101.3259344,102.737497,100.7204512,101.5967642,102.2174274,
                                            101.8744858,101.230462,104.7494111,105.2510445,101.9734378,
                                            99.26933376,104.0457763,101.8985474,106.5937275,100.9637462);
    double result = option.payoff(path);

    ASSERT_DOUBLE_EQ(result, 1.6256792024000219);
}
