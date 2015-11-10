/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef CONSTRAINT_HANDLER_H_
#define CONSTRAINT_HANDLER_H_

#include <vector>
#include "dither_types.h"

namespace dither {

class BaseConstraintHandler {
 public:
  BaseConstraintHandler(){};
  ~BaseConstraintHandler(){};
  bool violate_constraints(const dtest_case &test_case) { return false; }
  bool violate_constraints(const std::vector<param> &test_case) {
    return false;
  }
  void ground(dtest_case &test_case) {
    for (auto it = test_case.begin(); it != test_case.end(); ++it) {
      if ((*it) == -1) {
        *it = 0;
      }
    }
  }
};
}

#endif  // CONSTRAINT_HANDLER_H_
