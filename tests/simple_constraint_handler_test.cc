/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include <gtest/gtest.h>
#include "simple_constraint_handler.h"
#include <vector>

TEST(SimpleConstraintHandlerTest, canCreate) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  dither::dval constraint[] = {-1, 0, 0, -1};
  std::vector<dither::dval> tmp;
  for(unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
  }
  constraints.push_back(tmp);
  dither::SimpleConstraintHandler handler(arr, constraints);
}

TEST(SimpleConstraintHandlerTest, canTestCaseViolateConstraint) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  dither::dval constraint[] = {-1, 0, 0, -1};
  dither::dval constraint2[] = {-1, 0, 0, 0};
  std::vector<dither::dval> tmp;
  std::vector<dither::dval> tmp2;
  for(unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
    tmp2.push_back(constraint2[i]);
  }
  constraints.push_back(tmp);
  constraints.push_back(tmp2);
  std::vector<dither::dval> test_case;
  test_case.push_back(0);
  test_case.push_back(-1);
  test_case.push_back(-1);
  test_case.push_back(-1);
  dither::SimpleConstraintHandler handler(arr, constraints);
  ASSERT_FALSE(handler.violate_constraints(test_case));
  test_case[1] = 0;
  test_case[2] = 0;
  ASSERT_TRUE(handler.violate_constraints(test_case));
  test_case[1] = 1;
  test_case[3] = 0;
  ASSERT_FALSE(handler.violate_constraints(test_case));
}

TEST(SimpleConstraintHandlerTest, canParamsViolateConstraint) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  dither::dval constraint[] = {-1, 0, 0, -1};
  dither::dval constraint2[] = {-1, 0, 0, 0};
  std::vector<dither::dval> tmp;
  std::vector<dither::dval> tmp2;
  for(unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
    tmp2.push_back(constraint2[i]);
  }
  constraints.push_back(tmp);
  constraints.push_back(tmp2);
  std::vector<dither::param> test_case;
  dither::param param1;
  param1.first = 0;
  param1.second = 0;
  test_case.push_back(param1);
  dither::SimpleConstraintHandler handler(arr, constraints);
  ASSERT_FALSE(handler.violate_constraints(test_case));
  dither::param param2;
  param2.first = 1;
  param2.second = 0;
  test_case.push_back(param2);
  ASSERT_FALSE(handler.violate_constraints(test_case));
  dither::param param3;
  param3.first = 2;
  param3.second = 0;
  test_case.push_back(param3);
  ASSERT_TRUE(handler.violate_constraints(test_case));
}

TEST(SimpleConstraintHandlerTest, canGroundTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  dither::dval constraint[] = {-1, 0, 0, -1};
  dither::dval constraint2[] = {-1, 0, 0, 0};
  dither::dval constraint3[] = {0, -1, -1, 0};
  dither::dval constraint4[] = {1, -1, -1, 0};
  std::vector<dither::dval> tmp;
  std::vector<dither::dval> tmp2;
  std::vector<dither::dval> tmp3;
  std::vector<dither::dval> tmp4;
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
  std::vector<dither::dval> test_case;
  test_case.push_back(0);
  test_case.push_back(-1);
  test_case.push_back(-1);
  test_case.push_back(-1);
  dither::SimpleConstraintHandler handler(arr, constraints);
  ASSERT_TRUE(handler.ground(test_case));
  std::vector<dither::dval> test_case2;
  test_case2.push_back(0);
  test_case2.push_back(0);
  test_case2.push_back(0);
  test_case2.push_back(-1);
  ASSERT_FALSE(handler.ground(test_case2));
  std::vector<dither::dval> test_case3;
  test_case3.push_back(0);
  test_case3.push_back(-1);
  test_case3.push_back(0);
  test_case3.push_back(-1);
  // constraint solver should be able to unwind
  ASSERT_TRUE(handler.ground(test_case3));
}


TEST(GecodeCompatibilityConstraintTest, canValidateGecodeConstraintOnTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  std::vector<dither::dval> tmp;
  int constraint[] = {-1, 0, 0, -1};
  for(unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
  }
  constraints.push_back(tmp);

  dither::SimpleConstraintHandler handler(arr, constraints);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_FALSE(handler.violate_constraints(dcase));
  // test specific to gecode edge case
  /* dcase[1] = 20; */
  /* ASSERT_TRUE(handler.violate_constraints(dcase)); */
  dcase[1] = 0;
  dcase[2] = 0;
  ASSERT_TRUE(handler.violate_constraints(dcase));

}

TEST(GecodeCompatibilityConstraintTest, canValidateGecodeConstraintOnParams) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  std::vector<dither::dval> tmp;
  int constraint[] = {-1, 0, 0, -1};
  for(unsigned int i = 0; i < 4; i++) {
    tmp.push_back(constraint[i]);
  }
  constraints.push_back(tmp);

  dither::SimpleConstraintHandler handler(arr, constraints);
  std::vector<dither::param> params;
  dither::param my_param;
  my_param.first = 0;
  my_param.second = 1;
  params.push_back(my_param);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_FALSE(handler.violate_constraints(params));

  // test specific to gecode edge case
  /* params.clear(); */
  /* my_param.second = 20; */
  /* params.push_back(my_param); */
  /* ASSERT_TRUE(handler.violate_constraints(params)); */

  params.clear();
  my_param.first = 1;
  my_param.second = 0;
  params.push_back(my_param);
  my_param.first = 2;
  my_param.second = 0;
  params.push_back(my_param);
  ASSERT_TRUE(handler.violate_constraints(params));
}

TEST(GecodeCompatibilityConstraintTest, canGroundSimpleConstraintOnTestCase) {
  std::vector<int> arr;
  arr.push_back(2);
  arr.push_back(3);
  arr.push_back(4);
  arr.push_back(5);
  std::vector<std::vector<dither::dval>> constraints;
  std::vector<dither::dval> tmp;
  std::vector<dither::dval> tmp2;
  std::vector<dither::dval> tmp3;
  std::vector<dither::dval> tmp4;
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

  dither::SimpleConstraintHandler handler(arr, constraints);
  dither::dtest_case dcase(4, -1);
  dcase[0] = 0;
  ASSERT_TRUE(handler.ground(dcase));
  ASSERT_EQ(dcase[1], 1);
  ASSERT_EQ(dcase[0], 0);

  // test specific to gecode edge case
  /* std::fill(dcase.begin(), dcase.end(), -1); */
  /* dcase[1] = 20; */
  /* ASSERT_FALSE(handler.ground(dcase)); */

  std::fill(dcase.begin(), dcase.end(), -1);
  dcase[1] = 0;
  dcase[2] = 0;
  ASSERT_FALSE(handler.ground(dcase));

  std::fill(dcase.begin(), dcase.end(), -1);
  dcase[2] = 0;
  dcase[3] = 0;
  ASSERT_FALSE(handler.ground(dcase));
}
