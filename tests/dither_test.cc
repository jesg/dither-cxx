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
#include <gecode/int.hh>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

TEST(IpogTest, 2Way) {
  std::string people[] = {"bill", "joe", "jane"};
  int nums[] = {1, 2, 3, 4};
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 2);
  dither_ipog_add_parameter_int(ipog, "nums", nums, 4);
  dither_ipog_add_parameter_enum(ipog, "people", people, 3);
  dither_ipog_add_parameter(ipog, "on");
  dither_ipog_init_param_cache(ipog);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 12);
  dither_ipog_delete(ipog);
}


TEST(IpogTest, 3Way) {
  std::string people[] = {"bill", "joe", "jane"};
  int nums[] = {1, 2, 3, 4};
  int nums2[] = {1, 2, 3, 4, 5, 6};
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 3);
  dither_ipog_add_parameter_int(ipog, "nums", nums, 4);
  dither_ipog_add_parameter_int(ipog, "nums2", nums2, 6);
  dither_ipog_add_parameter_enum(ipog, "people", people, 3);
  dither_ipog_add_parameter(ipog, "on");
  dither_ipog_init_param_cache(ipog);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 72);
  dither_ipog_delete(ipog);
}

TEST(ConstraintTest, canValidateGecodeConstraintOnTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<int>> constraints;
  std::vector<int> tmp;
  int constraint[] = {-1, 0, 0, -1};
  for(unsigned int i = 0; i < 4; i++) {
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
  for(unsigned int i = 0; i < 4; i++) {
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
  for(unsigned int i = 0; i < 4; i++) {
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

TEST(IpogTest, 2WayWithConstraintsExcludeSubCombination) {
  std::string people[] = {"bill", "joe", "jane"};
  int nums[] = {1, 2, 3, 4};
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 2);
  dither_ipog_add_parameter_int(ipog, "nums", nums, 4);
  dither_ipog_add_parameter_enum(ipog, "people", people, 3);
  dither_ipog_add_parameter(ipog, "on");
  dither_ipog_init_param_cache(ipog);

  int constraint[] = {0, 1, -1};
  int constraint2[] = {0, 0, -1};
  int constraint3[] = {0, 2, -1};
  dither_add_constraint(ipog, constraint, 3);
  dither_add_constraint(ipog, constraint2, 3);
  dither_add_constraint(ipog, constraint3, 3);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 9);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 3WayTCAS) {
  std::string people[] = {"bill", "joe", "jane"};
  int nums[] = {1, 2, 3, 4};
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 3);
  dither_ipog_add_parameter_int(ipog, "nums", nums, 4);
  for(unsigned int i = 0; i < 7; i++) {
    dither_ipog_add_parameter(ipog, "b" + i);
  }
  for(unsigned int i = 0; i < 3; i++) {
    dither_ipog_add_parameter_enum(ipog, "s3-" + i, people, 3);
  }
  int ten[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for(unsigned int i = 0; i < 2; i++) {
    dither_ipog_add_parameter_int(ipog, "i10-" + i, ten, 10);
  }
  dither_ipog_init_param_cache(ipog);

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 416);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 4Way10) {
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 4);
  int five[] = {0, 1, 2, 4};
  for(unsigned int i = 0; i < 10; i++) {
    dither_ipog_add_parameter_int(ipog, "b" + i, five, 5);
  }
  dither_ipog_init_param_cache(ipog);

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 1789);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 2WayLargeValue) {
  ipog_handle ipog = dither_ipog_new();
  dither_ipog_set_t(ipog, 4);
  int vals[] = {0, 1, 2, 3, 4, 5};
  for(unsigned int i = 0; i < 10; i++) {
    dither_ipog_add_parameter_int(ipog, "i10-" + i, vals, 6);
  }
  dither_ipog_init_param_cache(ipog);

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ (dither_ipog_size(ipog), 3719);
  dither_ipog_delete(ipog);
}
