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
#include "base_constraint_handler.h"
#include "simple_constraint_handler.h"

namespace dither {

class Ipog {
  std::size_t t_;
  std::vector<std::vector<dval>> input_params_;
  std::vector<std::vector<param>> param_cache_;
  std::forward_list<dtest_case> bound_;
  std::forward_list<dtest_case> unbound_;
  std::unordered_map<std::string, std::vector<int>> int_params_;
  std::unordered_map<std::string, std::vector<std::string>> str_params_;
  std::unordered_map<std::string, std::vector<bool>> bool_params_;
  std::vector<std::string> ordered_param_names_;
  std::vector<int> ordered_param_index_;
  std::vector<int> reverse_ordered_param_index_;
  std::unordered_map<std::string, int> param_index_;
  std::unordered_map<int, std::string> reverse_param_index_;
  dtest_case merge_scratch_;
  BaseConstraintHandler* constraint_handler;
  std::vector<std::vector<dval>> constraints;
  std::vector<dval> ranges;
	std::size_t solution_size;
	std::vector<std::vector<dval>> original_previously_tested_;
	std::vector<std::vector<dval>> previously_tested_;

  inline void transform(std::vector<dval>& scratch, std::vector<dval>& test_case) {
    for(std::size_t i = 0; i < test_case.size(); i++) {
      scratch[ordered_param_index_[i]] = test_case[i];
    }
    std::copy(scratch.cbegin(), scratch.cend(), test_case.begin());
  }

  inline bool has_previously_tested(std::vector<param*>& test_case);
  inline bool has_previously_tested(dtest_case& test_case);
  inline bool has_previously_tested(const int, dtest_case& test_case);

 public:
  Ipog();
  ~Ipog();
  Ipog(const unsigned int);
  void set_t(const unsigned int t) { t_ = t; }
  void add_parameter(const std::string, const int[], const unsigned int);
  void add_parameter(const std::string, const std::string[], const unsigned int);
  void add_parameter(const std::string);
  void init_bound();
  void init_param_cache();
  void run();
  int size();
  std::string *header();
  inline void ground_solutions();
  inline bool is_valid() { return t_ <= param_cache_.size(); }
  inline std::forward_list<std::vector<param*>> cover(int);
  inline const int maximize_coverage(const int, dtest_case &,
                              std::forward_list<std::vector<param*>> &);
  void add_constraint(const int[], const unsigned int);
	void add_previously_tested(const int[], const std::size_t);
  inline bool is_covered(const dtest_case &test_case,
                         const std::vector<param*> &params);
  inline bool is_covered(const std::vector<param> &params);
  inline const int merge(const int, dtest_case &, const std::vector<param*> &);
  void display_raw_solution();
  void fill(int[]);
  inline void display_header() {
    for (std::size_t i = 0; i < param_cache_.size();) {
      std::cout << reverse_param_index_[i];
      if (++i < param_cache_.size()) {
        std::cout << ',';
      }
    }
    std::cout << std::endl;
  }

  inline void display_test_case(const dtest_case &test_case) {
    for (std::size_t i = 0; i < test_case.size();) {
      const dval value = test_case[i];
      if (value == -1) {
        std::cout << '-';
        if (++i < test_case.size()) {
          std::cout << ',';
        }
        continue;
      }
      const param my_param = param_cache_[reverse_ordered_param_index_[i]][value];
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
