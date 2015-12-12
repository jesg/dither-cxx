/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef COMBINATIONS_H_
#define COMBINATIONS_H_

#include <vector>
#include <forward_list>
#include "dither_types.h"

namespace dither {

inline void product(
    std::forward_list<dtest_case>& rvvi,           // final result
    dtest_case& rvi,                               // current result
    std::vector<dtest_case>::const_iterator me,    // current input
    std::vector<dtest_case>::const_iterator end);  // final input

inline void product2(
    std::forward_list<std::vector<param>>& rvvi,           // final result
    std::vector<param>& rvi,                               // current result
    std::vector<std::vector<param>>::const_iterator me,    // current input
    std::vector<std::vector<param>>::const_iterator end);  // final input

inline void gcombinations_(const std::vector<int>&, std::vector<int>&,
                           std::forward_list<std::vector<int>>&);

inline void combinations(const int, const std::vector<int>&,
                         std::forward_list<std::vector<int>>&);

inline void product2(
    std::forward_list<std::vector<param>>& rvvi,          // final result
    std::vector<param>& rvi,                              // current result
    std::vector<std::vector<param>>::const_iterator me,   // current input
    std::vector<std::vector<param>>::const_iterator end)  // final input
{
  if (me == end) {
    // terminal condition of the recursion. We no longer have
    // any input vectors to manipulate. Add the current result (rvi)
    // to the total set of results (rvvvi).
    rvvi.push_front(rvi);
    return;
  }

  // need an easy name for my vector-of-ints
  const std::vector<param>& mevi = *me;
  for (std::vector<param>::const_iterator it = mevi.begin(); it != mevi.end();
       it++) {
    // final rvi will look like "a, b, c, ME, d, e, f"
    // At the moment, rvi already has "a, b, c"
    rvi.push_back(*it);                // add ME
    product2(rvvi, rvi, me + 1, end);  // add "d, e, f"
    rvi.pop_back();                    // clean ME off for next round
  }
}

void combinations(const int n, const std::vector<int>& input,
                  std::forward_list<std::vector<int>>& output) {
  std::vector<int> scratch(n);

  if (n <= input.size()) {
    for (int i = 0; i < n; i++) {
      scratch[i] = i;
    }

    gcombinations_(input, scratch, output);
    while (1) {
      int i;
      for (i = n - 1; (i >= 0) && (scratch[i] == (input.size() - n + i)); i--)
        ;

      if (i < 0) {
        break;
      } else {
        scratch[i]++;
        for (++i; i < n; i++) {
          scratch[i] = scratch[i - 1] + 1;
        }
        gcombinations_(input, scratch, output);
      }
    }
  }
}

inline void gcombinations_(const std::vector<int>& input,
                           std::vector<int>& scratch,
                           std::forward_list<std::vector<int>>& output) {
  std::vector<int> result(scratch.size());
  for (std::size_t i = 0; i < result.size(); i++) {
    result[i] = input[scratch[i]];
  }
  output.push_front(result);
}

inline void product(
    std::forward_list<dtest_case>& rvvi,          // final result
    dtest_case& rvi,                              // current result
    std::vector<dtest_case>::const_iterator me,   // current input
    std::vector<dtest_case>::const_iterator end)  // final input
{
  if (me == end) {
    // terminal condition of the recursion. We no longer have
    // any input vectors to manipulate. Add the current result (rvi)
    // to the total set of results (rvvvi).
    rvvi.push_front(rvi);
    return;
  }

  // need an easy name for my vector-of-ints
  const dtest_case& mevi = *me;
  for (dtest_case::const_iterator it = mevi.begin(); it != mevi.end(); it++) {
    // final rvi will look like "a, b, c, ME, d, e, f"
    // At the moment, rvi already has "a, b, c"
    rvi.push_back(*it);               // add ME
    product(rvvi, rvi, me + 1, end);  // add "d, e, f"
    rvi.pop_back();                   // clean ME off for next round
  }
}

inline void product3(
    std::forward_list<dtest_case>& results,
    std::vector<dtest_case>::const_iterator begin,
    std::vector<dtest_case>::const_iterator end) {
  dtest_case ranges;
  for(auto it = begin; it != end; ++it) {
    const dval tmp = (*it).size() - 1;
    ranges.push_back(tmp);
  }

  dtest_case scratch(ranges.size(), 0);

  const std::size_t max = ranges.size() - 1;
  for(std::size_t i = max;;) {

    if(i == max) {
      for(dval val = 0; val <= ranges[i]; val++) {
        results.push_front(scratch);
        scratch[i]++;
      }
      scratch[i] = 0;
      i--;
    } else if(i == 0 && scratch[i] >= ranges[i]) {
      return;
    } else if(scratch[i] < ranges[i]) {
      scratch[i]++;
      i++;
    } else {
      scratch[i] = -1;
      i--;
    }
  }
}

inline void product4(
    std::forward_list<std::vector<param>>& results,
    std::vector<std::vector<param>>& param_matrix) {
  std::vector<int> ranges;
  std::vector<param> scratch;
  for(auto params : param_matrix) {
    ranges.push_back(params.size() - 1);
    scratch.push_back(params[0]);
  }
  std::vector<int> indexes(scratch.size(), 0);

  const std::size_t max = ranges.size() - 1;
  for(std::size_t i = max;;) {

    if(i == max) {
      for(std::size_t val = 0; val <= ranges[i]; val++) {
        scratch[i] = param_matrix[i][indexes[i]];
        results.push_front(scratch);
        indexes[i]++;
      }
      indexes[i] = 0;
      i--;
    } else if(i == 0 && indexes[i] >= ranges[i]) {
      return;
    } else if(indexes[i] < ranges[i]) {
      indexes[i]++;
      scratch[i] = param_matrix[i][indexes[i]];
      i++;
    } else {
      indexes[i] = -1;
      i--;
    }
  }
}
}

#endif
