
/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <gtest/gtest.h>
#include "combinations.h"
#include <vector>

/* product3 should be ~10% faster than product */
TEST(CombinationsTest, product3BackwardsCompatible) {
  int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  dither::dtest_case case1(arr, arr + 5);
  dither::dtest_case case2(arr, arr + 3);
  dither::dtest_case case3(arr, arr + 7);
  dither::dtest_case case4(arr, arr + 4);
  dither::dtest_case case5(arr, arr + 2);
  dither::dtest_case case6(arr, arr + 10);

  std::vector<dither::dtest_case> cases;
  cases.push_back(case1);
  cases.push_back(case2);
  cases.push_back(case3);
  cases.push_back(case4);
  cases.push_back(case5);
  cases.push_back(case6);

  dither::dtest_case tmp;
  std::forward_list<dither::dtest_case> results;
  dither::product(results, tmp, cases.cbegin(), cases.cend());

  std::forward_list<dither::dtest_case> results2;
  dither::product3(results2, cases.cbegin(), cases.cend());

  ASSERT_TRUE(std::equal(results.begin(), results.end(), results2.begin()));
}
