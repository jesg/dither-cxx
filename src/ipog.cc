/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "ipog.h"
#include <forward_list>
#include <iterator>
#include <utility>
#include <tuple>
#include <limits.h>

namespace dither {

Ipog::~Ipog() {
  if(constraint_handler != NULL) {
    delete constraint_handler;
  }
}

Ipog::Ipog() {
  t_ = 2;
}

Ipog::Ipog(const unsigned int t) {
  t_ = t;
}

void Ipog::init_bound() {
  using dither::product;
  dtest_case tmp;

  product(bound_, tmp, input_params_.begin(), input_params_.begin() + t_);
  auto size = param_cache_.size();
  for (auto it = bound_.begin(); it != bound_.end(); ++it) {
    (*it).resize(size, -1);
    (*it).shrink_to_fit();
  }

  merge_scratch_.resize(param_cache_.size(), -1);

  if(constraints.size() > 0) {
    for(auto it = param_cache_.cbegin(); it != param_cache_.cend(); ++it) {
      const dval tmp = (*it).size() - 1;
      ranges.push_back(tmp);
    }
    constraint_handler = new SimpleConstraintHandler(ranges, constraints);
  } else {
    constraint_handler = new BaseConstraintHandler();
  }
}

std::forward_list<std::vector<param>> Ipog::cover(const int k) {
  std::vector<int> input(k);
  for (std::size_t i = 0; i < k; i++) {
    input[i] = i;
  }

  std::forward_list<std::vector<int>> output;
  combinations(t_ - 1, input, output);
  std::forward_list<std::vector<std::vector<param>>> product_input;
  const std::vector<param> k_param = param_cache_[k];
  for (auto it = output.begin(); it != output.end(); ++it) {
    std::vector<std::vector<param>> param_tuple;
    (*it).push_back(k);
    for (auto iit = (*it).begin(); iit != (*it).end(); ++iit) {
      auto vals = param_cache_[*iit];
      param_tuple.push_back(vals);
    }
    product_input.push_front(param_tuple);
  }

  std::forward_list<std::vector<param>> coverage;
  for (auto it = product_input.begin(); it != product_input.end(); ++it) {
    std::vector<param> tmp;
    std::vector<std::vector<param>> x = *it;
    product2(coverage, tmp, (*it).cbegin(), (*it).cend());
  }
  return coverage;
}

void Ipog::run() {
  init_bound();
  for (auto k = t_; k < input_params_.size(); k++) {
    std::forward_list<std::vector<param>> pi = cover(k);

    {
      auto prev = bound_.cbefore_begin();
      auto next = bound_.begin();
      auto end = bound_.cend();
      while (next != end) {
        if (maximize_coverage(k, *next, pi) == -1) {
          next = bound_.erase_after(prev);
        } else {
          ++prev;
          ++next;
        }
      }
    }

    /* vertical extension */
    for (auto pairs = pi.cbegin(); pairs != pi.cend(); ++pairs) {
      const std::vector<param> &test_case = *pairs;
      bool case_covered = constraint_handler->violate_constraints(test_case);

			if (!case_covered) {
				bool is_merged = false;
				auto prev = unbound_.before_begin();
				auto next = unbound_.begin();
				auto end = unbound_.end();
				while (next != end) {
					const int merge_result = merge(k, *next, test_case);
					if (merge_result == 0) {
						bound_.push_front(*next);
						unbound_.erase_after(prev);
						is_merged = true;
						break;
					} else if (merge_result == 1) {
						is_merged = true;
						break;
					}
					++prev;
					++next;
				}

				if (!is_merged) {
					dtest_case unbound_test_case(param_cache_.size(), -1);
					for (auto it = test_case.cbegin(); it != test_case.cend(); ++it) {
						unbound_test_case[(*it).first] = (*it).second;
					}
					if (!constraint_handler->violate_constraints(unbound_test_case)) {
						unbound_.push_front(unbound_test_case);
					}
				}
			}

    }
  }
  ground_solutions();
}

/* -1 no merge, 0 perfect merge (no unbound), 1 partial merge */
inline const int Ipog::merge(const int k, dtest_case &test_case,
    const std::vector<param> &pairs) {
  for (auto it = pairs.cbegin(); it != pairs.cend(); ++it) {
    auto value = test_case[(*it).first];
    if (!(value == -1 || value == (*it).second)) {
      return -1;
    }
  }

  std::copy(test_case.cbegin(), test_case.cend(), merge_scratch_.begin());

  for (auto it = pairs.cbegin(); it != pairs.cend(); ++it) {
    merge_scratch_[(*it).first] = (*it).second;
  }

  if (constraint_handler->violate_constraints(merge_scratch_)) {
    return -1;
  }

  for (auto it = pairs.cbegin(); it != pairs.cend(); ++it) {
    test_case[(*it).first] = (*it).second;
  }

  for (auto i = 0; i < k; i++) {
    if (test_case[i] == -1) {
      return 1;
    }
  }

  return 0;
}

inline bool Ipog::is_covered(const dtest_case &test_case,
    const std::vector<param> &params) {
  for (auto param = params.cbegin(); param != params.cend(); ++param) {
    if (test_case[(*param).first] != (*param).second) {
      return false;
    }
  }
  return true;
}

const int Ipog::maximize_coverage(const int k, dtest_case &test_case,
    std::forward_list<std::vector<param>> &pi) {
  const std::vector<param> &param_range = param_cache_[k];
  int current_max = -1;
  param max_param = param_range[0];
  std::forward_list<std::forward_list<std::vector<param>>::iterator> covered;

  for (auto it = param_range.cbegin(); it != param_range.cend(); ++it) {
    std::forward_list<std::forward_list<std::vector<param>>::iterator>
      tmp_covered;
    const param current_param = *it;

    test_case[current_param.first] = current_param.second;
    if (!constraint_handler->violate_constraints(test_case)) {
      int count = 0;
      auto prev = pi.before_begin();
      for (auto params = pi.cbegin(); params != pi.cend(); ++params) {
        if (is_covered(test_case, *params)) {
          tmp_covered.push_front(prev);
          count++;
        }
        ++prev;
      }

      if (count > current_max) {
        current_max = count;
        max_param = current_param;
        covered = tmp_covered;
      }
    }
    test_case[current_param.first] = -1;
  }

  if (current_max == -1) {
    return -1;
  }

  /* remove covered */
  for (auto it = covered.begin(); it != covered.end(); ++it) {
    pi.erase_after(*it);
  }

  test_case[max_param.first] = max_param.second;
  return current_max;
}

void Ipog::display_raw_solution() {
  /* display_header(); */
  for (auto it = bound_.cbegin(); it != bound_.cend(); ++it) {
    display_test_case(*it);
  }
}

void Ipog::add_parameter(const std::string name, const int *terms,
    const unsigned int terms_length) {
  std::vector<int> tmp(terms, terms + terms_length);
  std::pair<std::string, std::vector<int>> key_value(name, tmp);
  int_params_.insert(key_value);
  ordered_param_names_.push_back(name);
}

void Ipog::add_parameter(const std::string name, const std::string *terms,
    const unsigned int terms_length) {
  std::vector<std::string> tmp(terms, terms + terms_length);
  std::pair<std::string, std::vector<std::string>> key_value(name, tmp);
  str_params_.insert(key_value);
  ordered_param_names_.push_back(name);
}

void Ipog::add_parameter(const std::string name) {
  const bool arr[] = {true, false};
  std::vector<bool> tmp(arr, arr + 2);
  std::pair<std::string, std::vector<bool>> key_value(name, tmp);
  bool_params_.insert(key_value);
  ordered_param_names_.push_back(name);
}

void Ipog::init_param_cache() {
  std::vector<std::tuple<std::string, int, int>> tmp;
  for (auto it = int_params_.cbegin(); it != int_params_.cend(); ++it) {
    if ((*it).second.size() < 1) {
      continue;
    }
    tmp.push_back(
	std::make_tuple((*it).first, DITHER_INT_T, (*it).second.size()));
  }

  for (auto it = str_params_.cbegin(); it != str_params_.cend(); ++it) {
    if ((*it).second.size() < 1) {
      continue;
    }
    tmp.push_back(
	std::make_tuple((*it).first, DITHER_STRING_T, (*it).second.size()));
  }

  for (auto it = bool_params_.cbegin(); it != bool_params_.cend(); ++it) {
    if ((*it).second.size() < 1) {
      continue;
    }
    tmp.push_back(
	std::make_tuple((*it).first, DITHER_BOOL_T, (*it).second.size()));
  }

  std::sort(tmp.begin(), tmp.end(), [](std::tuple<std::string, int, int> a,
        std::tuple<std::string, int, int> b) {
    return std::get<2>(a) > std::get<2>(b);
  });

  std::unordered_map<std::string, int> original_name_to_index;
  int count = 0;
  for(auto it = ordered_param_names_.cbegin(); it != ordered_param_names_.cend(); ++it, count++) {
    original_name_to_index.insert({{*it, count}});
  }
  ordered_param_index_.resize(tmp.size());
  reverse_ordered_param_index_.resize(tmp.size());

  count = 0;
  for (auto it = tmp.cbegin(); it != tmp.cend(); ++it, count++) {
    std::vector<param> params;
    std::vector<dval> dvals;
    auto name = std::get<0>(*it);
    auto type = std::get<1>(*it);
    reverse_param_index_.insert({{count, name}});
    ordered_param_index_[count] = original_name_to_index.find(name)->second;
    reverse_ordered_param_index_[original_name_to_index.find(name)->second] = count;

    for (dval i = 0; i < SCHAR_MAX && i < std::get<2>(*it); i++) {
      param my_param;
      my_param.first = count;
      my_param.second = i;
      my_param.name = name;
      my_param.type = type;

      params.push_back(my_param);
      dvals.push_back(i);
    }
    param_cache_.push_back(params);
    input_params_.push_back(dvals);
  }
}

int Ipog::size() {
  return solution_size;
}

std::string *Ipog::header() {
  std::string *result = new std::string[param_cache_.size()];
  for (std::size_t i = 0; i < param_cache_.size(); i++) {
    result[i] = reverse_param_index_[i];
  }
  return result;
}

  void Ipog::add_constraint(const int* constraint, const unsigned int length) {
    std::vector<dval> tmp(length);
    std::copy(constraint, constraint+length, tmp.begin());
    for(auto it = tmp.cbegin(); it != tmp.cend(); ++it) {
      if((*it) > SCHAR_MAX) {
        std::cerr << "WARNING: constraint value > " << SCHAR_MAX << " behavior undefined" << std::endl;
      }
    }
    constraints.push_back(tmp);
  }

void Ipog::ground_solutions() {
	std::size_t solution_count = 0;
  std::vector<dval> transform_scratch(param_cache_.size(), 0);

  auto prev = bound_.cbefore_begin();
  auto next = bound_.begin();
  auto end = bound_.cend();
  while (next != end) {
    if (constraint_handler->violate_constraints(*next)) {
      next = bound_.erase_after(prev);
    } else {
      transform(transform_scratch, *next);
			solution_count++;
      ++prev;
      ++next;
    }
  }

  while(!unbound_.empty()) {
    dtest_case first = unbound_.front();
    if(constraint_handler->ground(first)) {
      transform(transform_scratch, first);
			solution_count++;
      bound_.push_front(first);
    }
    unbound_.pop_front();
  }
	solution_size = solution_count;
  }

void Ipog::fill(int *solution) {
  std::size_t i = 0;
  for(auto it = bound_.cbegin(); it != bound_.cend(); ++it) {
    for(auto iit = (*it).cbegin(); iit != (*it).cend(); ++iit, i++) {
      solution[i] = *iit;
    }
  }
}
}
