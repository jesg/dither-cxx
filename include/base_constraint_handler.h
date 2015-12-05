/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef BASE_CONSTRAINT_HANDLER_H_
#define BASE_CONSTRAINT_HANDLER_H_

#include <vector>
#include "dither_types.h"

namespace dither {

  class BaseConstraintHandler {
    public:
      BaseConstraintHandler(){};
      virtual ~BaseConstraintHandler(){};
      virtual bool violate_constraints(const dtest_case &test_case) { return false; }
      virtual bool violate_constraints(const std::vector<param> &test_case) {
        return false;
      }
      virtual bool ground(dtest_case &test_case) {
        for (auto it = test_case.begin(); it != test_case.end(); ++it) {
          if ((*it) == -1) {
            *it = 0;
          }
        }
        return true;
      }
  };
}
#endif  // BASE_CONSTRAINT_HANDLER_H_
