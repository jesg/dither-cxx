/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <gtest/gtest.h>
#include "dither.h"
#include "dither_types.h"
#include "constraint_handler.h"
#include <gecode/int.hh>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

TEST(ConstraintTest, canValidateGecodeConstraintOnTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<int>> constraints;
  std::vector<int> tmp;
  int constraint[] = {-1, 0, 0, -1};
  for (unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
  }
  constraints.push_back(tmp);

  dither::GecodeConstraintHandler handler(arr, constraints);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_FALSE(handler.violate_constraints(dcase));
  dcase[1] = 20;
  ASSERT_TRUE(handler.violate_constraints(dcase));
  dcase[1] = 0;
  dcase[2] = 0;
  ASSERT_TRUE(handler.violate_constraints(dcase));
}

TEST(ConstraintTest, canValidateGecodeConstraintOnParams) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<int>> constraints;
  std::vector<int> tmp;
  int constraint[] = {-1, 0, 0, -1};
  for (unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
  }
  constraints.push_back(tmp);

  dither::GecodeConstraintHandler handler(arr, constraints);
  std::vector<dither::param> params;
  dither::param my_param;
  my_param.first = 0;
  my_param.second = 1;
  params.push_back(my_param);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_FALSE(handler.violate_constraints(params));

  params.clear();
  my_param.second = 20;
  params.push_back(my_param);
  ASSERT_TRUE(handler.violate_constraints(params));

  params.clear();
  my_param.first = 1;
  my_param.second = 0;
  params.push_back(my_param);
  my_param.first = 2;
  my_param.second = 0;
  params.push_back(my_param);
  ASSERT_TRUE(handler.violate_constraints(params));
}

TEST(ConstraintTest, canGroundGecodeConstraintOnTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<int>> constraints;
  std::vector<int> tmp;
  std::vector<int> tmp2;
  std::vector<int> tmp3;
  std::vector<int> tmp4;
  int constraint[] = {-1, 0, 0, -1};
  int constraint2[] = {-1, 0, -1, -1};
  int constraint3[] = {-1, -1, 0, 0};
  int constraint4[] = {-1, -1, 0, 1};
  for (unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
    tmp2.push_back(constraint2[i]);
    tmp3.push_back(constraint3[i]);
    tmp4.push_back(constraint4[i]);
  }
  constraints.push_back(tmp);
  constraints.push_back(tmp2);
  constraints.push_back(tmp3);
  constraints.push_back(tmp4);

  dither::GecodeConstraintHandler handler(arr, constraints);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_TRUE(handler.ground(dcase));
  ASSERT_EQ(dcase[1], 1);
  ASSERT_EQ(dcase[0], 0);
  std::fill(dcase.begin(), dcase.end(), -1);
  dcase[1] = 20;
  ASSERT_FALSE(handler.ground(dcase));

  std::fill(dcase.begin(), dcase.end(), -1);
  dcase[1] = 0;
  dcase[2] = 0;
  ASSERT_FALSE(handler.ground(dcase));

  std::fill(dcase.begin(), dcase.end(), -1);
  dcase[2] = 0;
  dcase[3] = 0;
  ASSERT_FALSE(handler.ground(dcase));
}
