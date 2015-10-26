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

TEST(IpogTest, 2Way) {
  std::string people[] = {"jason", "jennie", "carrie"};
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
  std::string people[] = {"jason", "jennie", "carrie"};
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
