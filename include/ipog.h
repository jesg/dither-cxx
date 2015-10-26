/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef IPOG_H_
#define IPOG_H_

#include <vector>
#include <iostream>
#include <forward_list>
#include <algorithm>
#include <string>
#include <unordered_map>
#include "combinations.h"
#include "dither_types.h"

namespace dither {

class Ipog {
  int t_;
  std::vector<std::vector<dval>> input_params_;
  std::vector<std::vector<param>> param_cache_;
  std::forward_list<dtest_case> bound_;
  std::forward_list<dtest_case> unbound_;
  std::unordered_map<std::string, std::vector<int>> int_params_;
  std::unordered_map<std::string, std::vector<std::string>> str_params_;
  std::unordered_map<std::string, std::vector<bool>> bool_params_;
  std::unordered_map<std::string, int> param_index_;
  std::unordered_map<int, std::string> reverse_param_index_;
  dtest_case merge_scratch_;

 public:
  Ipog();
  Ipog(const unsigned char);
  void set_t(const int t) { t_ = t; }
  void add_parameter(const std::string, const int[], const int);
  void add_parameter(const std::string, const std::string[], const int);
  void add_parameter(const std::string);
  void init_bound();
  void init_param_cache();
  void run();
  int size();
  std::string *header();
  inline bool is_valid() { return t_ <= param_cache_.size(); }
  std::forward_list<std::vector<param>> cover(int);
  const int maximize_coverage(const int, dtest_case &,
                              std::forward_list<std::vector<param>> &);
  inline bool is_covered(const dtest_case &test_case,
                         const std::vector<param> &params);
  inline bool is_covered(const std::vector<param> &params);
  inline const int merge(const int, dtest_case &, const std::vector<param> &);
  void display_raw_solution();
  inline void display_header() {
    for (int i = 0; i < param_cache_.size();) {
      std::cout << reverse_param_index_[i];
      if (++i < param_cache_.size()) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }

  inline void display_test_case(const dtest_case &test_case) {
    for (int i = 0; i < test_case.size();) {
      const dval value = test_case[i];
      if (value == -1) {
        std::cout << '-';
        if (++i < test_case.size()) {
          std::cout << ',';
        }
        continue;
      }
      const param my_param = param_cache_[i][value];
      switch (my_param.type) {
        case DITHER_INT_T:
          std::cout << int_params_[my_param.name][my_param.second];
          break;
        case DITHER_BOOL_T:
          std::cout << std::boolalpha
                    << bool_params_[my_param.name][my_param.second];
          break;
        case DITHER_STRING_T:
          std::cout << str_params_[my_param.name][my_param.second];
          break;
      }
      if (++i < test_case.size()) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }
};
}

#endif
