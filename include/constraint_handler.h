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
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <gecode/search.hh>

namespace dither {

class BaseConstraintHandler {
 public:
  BaseConstraintHandler(){};
  ~BaseConstraintHandler(){};
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

class GecodeConstraintSpace : public Gecode::Space {

  public:
    Gecode::IntVarArray vars_;

    GecodeConstraintSpace(const std::vector<int> ranges, const std::vector<std::vector<int>> constraints) {
      Gecode::IntVarArray tmp_vars(*this, ranges.size());
      vars_ = tmp_vars;
      for(unsigned int i = 0; i < ranges.size(); i++) {
        Gecode::IntVar tmp(*this, 0, ranges[i]);
        vars_[i] = tmp;
      }

      for(auto it = constraints.cbegin(); it != constraints.cend(); ++it) {
        Gecode::BoolVar b;
        bool not_set = true;

        auto values = *it;
        for(unsigned int i = 0; i < vars_.size(); i++) {
          if(values[i] == -1) continue;
          if(not_set) {
            not_set = false;
            b = expr(*this, vars_[i] == values[i]);
          } else {
            b = expr(*this, b && vars_[i] == values[i]);
          }
        }
        rel(*this, !b);
      }
    }

    GecodeConstraintSpace(bool share, GecodeConstraintSpace& c) : Gecode::Space(share, c) {
      vars_.update(*this, share, c.vars_);
    }

    virtual Gecode::Space * copy(bool share) {
      return new GecodeConstraintSpace(share, *this);
    }

    void print() const {
      std::cout << vars_ << std::endl;
    }
};

class GecodeConstraintHandler: public BaseConstraintHandler {
  protected:
    std::vector<int>& params;
    std::vector<std::vector<int>>& constraints;

  public:

    GecodeConstraintHandler(std::vector<int>& ranges, std::vector<std::vector<int>>& constraints) : params(ranges), constraints(constraints) {}

    bool violate_constraints(const dtest_case &test_case) {
      GecodeConstraintSpace tmp_space(params, constraints);

      unsigned int index = 0;
      for(auto it = test_case.cbegin(); it != test_case.cend(); ++it) {
        if((*it) != -1) {
          rel(tmp_space, tmp_space.vars_[index] == (*it));
        }
        index++;
      }

      Gecode::DFS<GecodeConstraintSpace> dfs(&tmp_space);
      while(GecodeConstraintSpace * s = dfs.next()) {
        delete s;
        return false;
      }
      return true;
    }

    bool violate_constraints(const std::vector<param> &test_case) {
      GecodeConstraintSpace tmp_space(params, constraints);

      for(auto it = test_case.cbegin(); it != test_case.cend(); ++it) {
        if((*it).second != -1) {
          rel(tmp_space, tmp_space.vars_[(*it).first] == (*it).second);
        }
      }

      Gecode::DFS<GecodeConstraintSpace> dfs(&tmp_space);
      while(GecodeConstraintSpace * s = dfs.next()) {
        delete s;
        return false;
      }
      return true;
    }

    bool ground(dtest_case &test_case) {
      GecodeConstraintSpace tmp_space(params, constraints);

      // bind bound variables
      unsigned int index = 0;
      for(auto it = test_case.cbegin(); it != test_case.cend(); ++it) {
        if((*it) != -1) {
          rel(tmp_space, tmp_space.vars_[index] == (*it));
        }
        index++;
      }

      Gecode::DFS<GecodeConstraintSpace> dfs(&tmp_space);
      while(GecodeConstraintSpace * s = dfs.next()) {
        unsigned int i = 0;
        for(auto it = s->vars_.begin(); it != s->vars_.end(); ++it) {
          if(test_case[i] == -1 && (*it).size() > 0) {
            test_case[i] = (*it).min();
          } else {
          }
          i++;
        }
        delete s;
        return true;
      }
      return false;
    }
};
}

#endif  // CONSTRAINT_HANDLER_H_
