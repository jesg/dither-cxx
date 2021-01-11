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
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

TEST(IpogTest, 2Way) {
  int nums[] = {1, 2, 3, 4};
  ipog_handle ipog = dither_ipog_new(2);
  dither_ipog_add_parameter_int(ipog, 0, nums, 4);
  dither_ipog_add_parameter_int(ipog, 1, nums, 3);
  dither_ipog_add_parameter_int(ipog, 2, nums, 2);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 12);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 3Way) {
  int nums[] = {1, 2, 3, 4, 5, 6};
  ipog_handle ipog = dither_ipog_new(3);
  dither_ipog_add_parameter_int(ipog, 0, nums, 4);
  dither_ipog_add_parameter_int(ipog, 1, nums, 6);
  dither_ipog_add_parameter_int(ipog, 2, nums, 3);
  dither_ipog_add_parameter_int(ipog, 3, nums, 2);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 72);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 2WayWithConstraintsExcludeSubCombination) {
  int nums[] = {1, 2, 3, 4};
  ipog_handle ipog = dither_ipog_new(2);
  dither_ipog_add_parameter_int(ipog, 0, nums, 4);
  dither_ipog_add_parameter_int(ipog, 1, nums, 3);
  dither_ipog_add_parameter_int(ipog, 2, nums, 2);
  int tested[] = {2, 2, 1};

  int constraint[] = {0, 1, -1};
  int constraint2[] = {0, 0, -1};
  int constraint3[] = {0, 2, -1};
  dither_ipog_add_constraint(ipog, constraint, 3);
  dither_ipog_add_constraint(ipog, constraint2, 3);
  dither_ipog_add_constraint(ipog, constraint3, 3);
  dither_ipog_add_previously_tested(ipog, tested, 3);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 9);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 2WayWithConstraintsExcludeSubCombinationFromRubyGem) {
  int nums[] = {0, 1, 2, 3};
  ipog_handle ipog = dither_ipog_new(3);
  dither_ipog_add_parameter_int(ipog, 0, nums, 2);
  dither_ipog_add_parameter_int(ipog, 1, nums, 2);
  dither_ipog_add_parameter_int(ipog, 2, nums, 2);
  dither_ipog_add_parameter_int(ipog, 3, nums, 4);

  int constraint[] = {0, 1, 0, -1};
  dither_ipog_add_constraint(ipog, constraint, 4);
  dither_ipog_run(ipog);
  dither_ipog_display_raw_solution(ipog);
  std::size_t s = dither_ipog_size(ipog) * 4;
  int *solution = new int[s];
  dither_ipog_fill(ipog, solution);
  std::size_t i = 0;
  while (i < s) {
    ASSERT_FALSE(solution[i] == 0 && solution[i + 1] == 1 &&
                 solution[i + 2] == 0);
    i += 4;
  }
  dither_ipog_delete(ipog);
  delete[] solution;
}

TEST(IpogTest, 3WayTCAS) {
  int nums[] = {1, 2, 3, 4, 5, 6};
  ipog_handle ipog = dither_ipog_new(3);
  dither_ipog_add_parameter_int(ipog, 0, nums, 4);
  for (unsigned int i = 0; i < 7; i++) {
    dither_ipog_add_parameter_int(ipog, i + 1, nums, 2);
  }
  for (unsigned int i = 0; i < 2; i++) {
    dither_ipog_add_parameter_int(ipog, 8 + i, nums, 3);
  }
  int ten[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for (unsigned int i = 0; i < 2; i++) {
    dither_ipog_add_parameter_int(ipog, 11 + i, ten, 10);
  }

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 405);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 4Way10) {
  ipog_handle ipog = dither_ipog_new(4);
  int five[] = {0, 1, 2, 3, 4};
  for (unsigned int i = 0; i < 10; i++) {
    dither_ipog_add_parameter_int(ipog, i, five, 5);
  }

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 1964);
  dither_ipog_delete(ipog);
}

TEST(IpogTest, 2WayLargeValue) {
  ipog_handle ipog = dither_ipog_new(4);
  int vals[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (unsigned int i = 0; i < 10; i++) {
    dither_ipog_add_parameter_int(ipog, i, vals, 6);
  }

  dither_ipog_run(ipog);
  std::cout << dither_ipog_size(ipog) << std::endl;
  ASSERT_EQ(dither_ipog_size(ipog), 4055);
  dither_ipog_delete(ipog);
}
