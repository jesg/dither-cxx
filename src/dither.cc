/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "dither.h"

ipog_handle dither_ipog_new(const unsigned int t) { return new dither::Ipog(t); }

void dither_ipog_delete(ipog_handle self) {
  delete static_cast<dither::Ipog *>(self);
}

void dither_ipog_add_parameter_int(ipog_handle self, const int index,
                                   const int *values, const int values_length) {
  const std::string name = std::to_string(index);
  static_cast<dither::Ipog *>(self)->add_parameter(name, values, values_length);
}

void dither_ipog_run(ipog_handle self) {
  static_cast<dither::Ipog *>(self)->init_param_cache();
  static_cast<dither::Ipog *>(self)->run();
}

int dither_ipog_size(ipog_handle self) {
  return static_cast<dither::Ipog *>(self)->size();
}

void dither_ipog_display_raw_solution(ipog_handle self) {
  static_cast<dither::Ipog *>(self)->display_raw_solution();
}

void dither_ipog_fill(ipog_handle self, int *solution) {
  static_cast<dither::Ipog *>(self)->fill(solution);
}

void dither_ipog_add_constraint(ipog_handle self, const int constraint[], const int length) {
  static_cast<dither::Ipog *>(self)->add_constraint(constraint, length);
}

void dither_ipog_add_previously_tested(ipog_handle self, const int tested[], const int length) {
  static_cast<dither::Ipog *>(self)->add_previously_tested(tested, length);
}
