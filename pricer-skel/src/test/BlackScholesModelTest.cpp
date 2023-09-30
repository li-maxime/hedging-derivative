#include "gtest/gtest.h"
#include "../BlackScholesModel.hpp"
#include "../PerformanceOption.hpp"
#include "../MonteCarlo.hpp"

TEST(BlackScholesModel, shiftAsset_t_between){
    auto bs_model = new BlackScholesModel(5,0.02,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    auto path = pnl_mat_create_from_scalar(5,5,1.0);

    auto shift_path = pnl_mat_new();
    bs_model->shiftAsset(shift_path, path, 1,1.0,0.1,1.0);
    auto vect = pnl_vect_new();
    auto expected_result = pnl_vect_create_from_scalar(5,2.0);
    LET(expected_result, 0) = 1.0;
    pnl_mat_get_col(vect, shift_path, 1);
    ASSERT_EQ(pnl_vect_isequal(vect, expected_result, 0.01), 1);
}

TEST(BlackScholesModel, shiftAsset_t_exact){
    auto bs_model = new BlackScholesModel(5,0.02,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    auto path = pnl_mat_create_from_scalar(5,5,1.0);

    auto shift_path = pnl_mat_new();
    bs_model->shiftAsset(shift_path, path, 1,1.0,1.0,1.0);
    auto vect = pnl_vect_new();
    auto expected_result = pnl_vect_create_from_scalar(5,2.0);
    LET(expected_result, 0) = 1.0;
    pnl_mat_get_col(vect, shift_path, 1);
    ASSERT_EQ(pnl_vect_isequal(vect, expected_result, 0.01), 1);
}

TEST(BlackScholesModel, shiftAsset_t_0){
    auto bs_model = new BlackScholesModel(5,0.02,0.5,pnl_vect_create_from_scalar(5, 0.2),pnl_vect_create_from_scalar(5, 100.0));
    auto path = pnl_mat_create_from_scalar(5,5,1.0);

    auto shift_path = pnl_mat_new();
    bs_model->shiftAsset(shift_path, path, 1,1.0,0.0,1.0);
    auto vect = pnl_vect_new();
    auto expected_result = pnl_vect_create_from_scalar(5,2.0);
    pnl_mat_get_col(vect, shift_path, 1);
    ASSERT_EQ(pnl_vect_isequal(vect, expected_result, 0.01), 1);
}

