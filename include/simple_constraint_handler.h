/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef SIMPLE_CONSTRAINT_HANDLER_H_
#define SIMPLE_CONSTRAINT_HANDLER_H_

#include <vector>
#include <utility>
#include <algorithm>
#include "dither_types.h"
#include "base_constraint_handler.h"

namespace dither {

class SimpleConstraintHandler : public BaseConstraintHandler {
 protected:
  std::vector<std::vector<std::pair<std::size_t, dval>>> constraints;
  std::vector<dval> params;
  std::vector<dval> scratch;

  inline bool violate_constraint(
      const dtest_case& test_case,
      const std::vector<std::pair<std::size_t, dval>>& constraint);
  inline bool violate_constraints_(const dtest_case& test_case);

 public:
  SimpleConstraintHandler(std::vector<dval>& ranges,
                          std::vector<std::vector<dval>>& pconstraints);
  bool violate_constraints(const dtest_case& test_case);
  bool violate_constraints(const std::vector<param*>& test_case);
  bool violate_constraints(param** test_case, const std::size_t length);
  bool ground(dtest_case& test_case);
};
}

#endif  // SIMPLE_CONSTRAINT_HANDLER_H_
