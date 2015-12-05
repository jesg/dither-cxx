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

/* TEST(IpogTest, 4Way10) { */
/*   ipog_handle ipog = dither_ipog_new(); */
/*   dither_ipog_set_t(ipog, 4); */
/*   int five[] = {0, 1, 2, 4}; */
/*   for(unsigned int i = 0; i < 10; i++) { */
/*     dither_ipog_add_parameter_int(ipog, "b" + i, five, 5); */
/*   } */
/*   dither_ipog_init_param_cache(ipog); */

/*   dither_ipog_run(ipog); */
/*   std::cout << dither_ipog_size(ipog) << std::endl; */
/*   ASSERT_EQ (dither_ipog_size(ipog), 1789); */
/*   dither_ipog_delete(ipog); */
/* } */

/* TEST(IpogTest, 2WayLargeValue) { */
/*   ipog_handle ipog = dither_ipog_new(); */
/*   dither_ipog_set_t(ipog, 4); */
/*   int vals[] = {0, 1, 2, 3, 4, 5}; */
/*   for(unsigned int i = 0; i < 10; i++) { */
/*     dither_ipog_add_parameter_int(ipog, "i10-" + i, vals, 6); */
/*   } */
/*   dither_ipog_init_param_cache(ipog); */

/*   dither_ipog_run(ipog); */
/*   std::cout << dither_ipog_size(ipog) << std::endl; */
/*   ASSERT_EQ (dither_ipog_size(ipog), 3719); */
/*   dither_ipog_delete(ipog); */
/* } */
