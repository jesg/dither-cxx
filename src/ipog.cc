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
#include <cstddef>
#include <memory>

namespace dither {

Ipog::~Ipog() {
  if (constraint_handler != NULL) {
    delete constraint_handler;
  }
}

Ipog::Ipog() { t_ = 2; }

Ipog::Ipog(const unsigned int t) : t_(t) {}

void Ipog::init_bound() {
  using dither::product3;
  dtest_case tmp;

  product3(bound_, input_params_.begin(), input_params_.begin() + t_);
  auto size = param_cache_.size();
  for (auto it = bound_.begin(); it != bound_.end(); ++it) {
    (*it).resize(size, -1);
    (*it).shrink_to_fit();
  }
  bound_.remove_if([this](dtest_case &a) { return has_previously_tested(a); });

  merge_scratch_.resize(param_cache_.size(), -1);

  if (constraints.size() > 0) {
    for (auto it = param_cache_.cbegin(); it != param_cache_.cend(); ++it) {
      const dval tmp = (*it).size() - 1;
      ranges.push_back(tmp);
    }
    constraint_handler = new SimpleConstraintHandler(ranges, constraints);
  } else {
    constraint_handler = new BaseConstraintHandler();
  }
}

inline param **Ipog::cover(const int k, std::forward_list<param **> &coverage) {
  std::vector<int> input(k);
  for (std::size_t i = 0; i < k; i++) {
    input[i] = i;
  }

  std::forward_list<std::vector<int>> output;
  combinations(t_ - 1, input, output);
  std::forward_list<std::vector<std::vector<param> *>> product_input;
  int count = 0;
  for (auto it = output.begin(); it != output.end(); ++it) {
    std::vector<std::vector<param> *> param_tuple;
    (*it).push_back(k);
    int inner_count = 1;
    for (auto iit = (*it).begin(); iit != (*it).end(); ++iit) {
      param_tuple.push_back(&param_cache_[*iit]);
      inner_count *= param_cache_[*iit].size();
    }
    count += inner_count;
    product_input.push_front(param_tuple);
  }

  param **local_param_cache = new param *[count * t_];
  std::size_t j = 0;
  for (auto it = product_input.begin(); it != product_input.end(); ++it) {
    j += t_ * product4(coverage, local_param_cache + j, *it);
  }
  coverage.remove_if([this](param **a) { return has_previously_tested(a); });
  return local_param_cache;
}

void Ipog::run() {
  init_bound();
  for (auto k = t_; k < input_params_.size(); k++) {
    std::forward_list<param **> pi;
    param **params = cover(k, pi);
    std::unique_ptr<param *[]> params_ptr(params);

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
      param **test_case = *pairs;
      bool case_covered =
          constraint_handler->violate_constraints(test_case, t_);

      if (!case_covered) {
        for (auto it = unbound_.cbegin(); it != unbound_.cend(); ++it) {
          if (is_covered(*it, test_case)) {
            case_covered = true;
            break;
          }
        }
      }

      if (!case_covered) {
        bool is_merged = false;

        for (auto next = unbound_.begin(); next != unbound_.end(); ++next) {
          const int merge_result = merge(k, *next, test_case);

          if (merge_result >= 0) {
            for (std::size_t i = 0; i < t_; i++) {
              const param *it = test_case[i];
              (*next)[it->first] = it->second;
            }
            is_merged = true;
            break;
          }
        }

        if (!is_merged) {
          dtest_case unbound_test_case(param_cache_.size(), -1);
          for (std::size_t i = 0; i < t_; i++) {
            const param *it = test_case[i];
            unbound_test_case[it->first] = it->second;
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
                             param **pairs) {
  for (std::size_t i = 0; i < t_; i++) {
    const param *it = pairs[i];
    auto value = test_case[it->first];
    if (!(value == -1 || value == it->second)) {
      return -1;
    }
  }

  std::copy(test_case.cbegin(), test_case.cend(), merge_scratch_.begin());

  for (std::size_t i = 0; i < t_; i++) {
    const param *it = pairs[i];
    merge_scratch_[it->first] = it->second;
  }

  if (constraint_handler->violate_constraints(merge_scratch_)) {
    return -1;
  }

  for (auto i = 0; i <= k; i++) {
    if (merge_scratch_[i] == -1) {
      return 1;
    }
  }

  return 0;
}

inline bool Ipog::is_covered(const dtest_case &test_case, param **params) {
  for (std::size_t i = 0; i < t_; i++) {
    const param *my_param = params[i];
    if (test_case[my_param->first] != my_param->second) {
      return false;
    }
  }
  return true;
}
inline bool Ipog::is_covered(const dtest_case &test_case,
                             const std::vector<param *> &params) {
  for (auto param = params.cbegin(); param != params.cend(); ++param) {
    if (test_case[(*param)->first] != (*param)->second) {
      return false;
    }
  }
  return true;
}

inline const int Ipog::maximize_coverage(const int k, dtest_case &test_case,
                                         std::forward_list<param **> &pi) {
  const std::vector<param> &param_range = param_cache_[k];
  std::vector<std::forward_list<std::forward_list<param **>::iterator>> covered(
      param_range.size());
  std::vector<int> counts(param_range.size());
  const std::size_t index = param_range[0].first;
  std::vector<std::size_t> valid_indexes;
  valid_indexes.reserve(counts.size());
  for (std::size_t i = 0; i < counts.size(); i++) {
    test_case[index] = param_range[i].second;
    if (constraint_handler->violate_constraints(test_case)) {
      counts[i] = -1;
    } else {
      valid_indexes.push_back(i);
    }
  }

  auto prev = pi.before_begin();
  for (auto params = pi.begin(); params != pi.end(); ++params, ++prev) {
    for (auto i : valid_indexes) {
      test_case[index] = param_range[i].second;
      if (is_covered(test_case, *params)) {
        covered[i].push_front(prev);
        ++counts[i];
      }
    }
  }

  int current_max = -1;
  std::size_t max_index = 0;
  for (auto i : valid_indexes) {
    if (current_max < counts[i]) {
      current_max = counts[i];
      max_index = i;
    }
  }

  if (current_max == -1) {
    return -1;
  }

  /* remove covered */
  for (auto it = covered[max_index].begin(); it != covered[max_index].end();
       ++it) {
    pi.erase_after(*it);
  }

  test_case[index] = param_range[max_index].second;
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
  for (auto it = ordered_param_names_.cbegin();
       it != ordered_param_names_.cend(); ++it, count++) {
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
    reverse_ordered_param_index_[original_name_to_index.find(name)->second] =
        count;

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

  std::vector<dval> scratch(param_cache_.size());
  for (auto it = original_previously_tested_.cbegin();
       it != original_previously_tested_.cend(); ++it) {
    for (std::size_t i = 0; i < scratch.size(); i++) {
      scratch[reverse_ordered_param_index_[i]] = (*it)[i];
    }
    previously_tested_.push_back(scratch);
  }

  // transform constraints
  for (std::size_t j = 0; j < constraints.size(); j++) {
    for (std::size_t i = 0; i < scratch.size(); i++) {
      scratch[reverse_ordered_param_index_[i]] = constraints[j][i];
    }
    std::copy(scratch.begin(), scratch.end(), constraints[j].begin());
  }
}

int Ipog::size() { return solution_size; }

std::string *Ipog::header() {
  std::string *result = new std::string[param_cache_.size()];
  for (std::size_t i = 0; i < param_cache_.size(); i++) {
    result[i] = reverse_param_index_[i];
  }
  return result;
}

void Ipog::add_constraint(const int *constraint, const unsigned int length) {
  std::vector<dval> tmp(length);
  std::copy(constraint, constraint + length, tmp.begin());
  for (auto it = tmp.cbegin(); it != tmp.cend(); ++it) {
    if ((*it) > SCHAR_MAX) {
      std::cerr << "WARNING: constraint value > " << SCHAR_MAX
                << " behavior undefined" << std::endl;
    }
  }
  constraints.push_back(tmp);
}

inline void Ipog::ground_solutions() {
  std::size_t solution_count = 0;
  std::vector<dval> transform_scratch(param_cache_.size(), 0);

  auto prev = bound_.cbefore_begin();
  auto next = bound_.begin();
  auto end = bound_.cend();
  while (next != end) {
    if (constraint_handler->violate_constraints(*next) ||
        has_previously_tested(*next)) {
      next = bound_.erase_after(prev);
    } else {
      transform(transform_scratch, *next);
      solution_count++;
      ++prev;
      ++next;
    }
  }

  while (!unbound_.empty()) {
    dtest_case first = unbound_.front();
    if (constraint_handler->ground(first)) {
      if (!has_previously_tested(first)) {
        transform(transform_scratch, first);
        solution_count++;
        bound_.push_front(first);
      }
    }
    unbound_.pop_front();
  }
  solution_size = solution_count;
}

void Ipog::fill(int *solution) {
  std::size_t i = 0;
  for (auto it = bound_.cbegin(); it != bound_.cend(); ++it) {
    for (auto iit = (*it).cbegin(); iit != (*it).cend(); ++iit, i++) {
      solution[i] = *iit;
    }
  }
}

void Ipog::add_previously_tested(const int values[], const std::size_t length) {
  std::vector<dval> tmp(length);
  std::copy(values, values + length, tmp.begin());
  original_previously_tested_.push_back(tmp);
}

inline bool Ipog::has_previously_tested(std::vector<param *> &test_case) {
  for (auto it = previously_tested_.cbegin(); it != previously_tested_.cend();
       ++it) {
    bool flag = true;
    for (auto iit = test_case.cbegin(); iit != test_case.cend(); ++iit) {
      if ((*it)[(*iit)->first] != (*iit)->second) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return true;
    }
  }
  return false;
}

inline bool Ipog::has_previously_tested(param **params) {
  for (auto it = previously_tested_.cbegin(); it != previously_tested_.cend();
       ++it) {
    bool flag = true;
    for (std::size_t i = 0; i < t_; i++) {
      const param *iit = params[i];
      if ((*it)[iit->first] != iit->second) {
        flag = false;
        break;
      }
    }
    if (flag) {
      return true;
    }
  }
  return false;
}

inline bool Ipog::has_previously_tested(dtest_case &test_case) {
  for (auto it = previously_tested_.cbegin(); it != previously_tested_.cend();
       ++it) {
    if (std::equal(test_case.cbegin(), test_case.cend(), (*it).cbegin())) {
      return true;
    }
  }
  return false;
}

inline bool Ipog::has_previously_tested(const int k, dtest_case &test_case) {
  for (auto it = previously_tested_.cbegin(); it != previously_tested_.cend();
       ++it) {
    if (std::equal(test_case.cbegin(), test_case.cbegin() + k,
                   (*it).cbegin())) {
      return true;
    }
  }
  return false;
}
}
