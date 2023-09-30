#include "gtest/gtest.h"
#include "../BasketOption.hpp"

TEST(BasketOption, Payoff){
    PnlVect * weight = pnl_vect_create_from_list(3,1.0,10.0,100.0);
    BasketOption option(3.0, 2, 3,0,weight);
    PnlMat* path = pnl_mat_create_from_list(2, 3, 1.0,2.0,3.0,4.0,5.0,6.0);
    auto result = option.payoff(path);
    ASSERT_EQ(result, 654);
}


