#include "gtest/gtest.h"
#include <iostream>
#include "../PerformanceOption.hpp"

TEST(PerformanceOption, Payoff){
    PnlVect * weight = pnl_vect_create_from_list(3,1.0,10.0,100.0);
    PerformanceOption* option = new PerformanceOption(weight,1.0,1,3);
    PnlMat* path = pnl_mat_create_from_list(2, 3, 1.0,2.0,3.0,2.0,4.0,6.0);
    auto result = option->payoff(path);
    ASSERT_EQ(result, 2.0);

}