/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "dither.h"

ipog_handle dither_ipog_new() { return new dither::Ipog(); }

void dither_ipog_delete(ipog_handle self) {
  delete static_cast<dither::Ipog *>(self);
}

void dither_ipog_set_t(ipog_handle self, const int t) {
  static_cast<dither::Ipog *>(self)->set_t(t);
}

void dither_ipog_add_parameter_int(ipog_handle self, const char *name,
                                   const int *values, const int values_length) {
  static_cast<dither::Ipog *>(self)->add_parameter(name, values, values_length);
}

void dither_ipog_add_parameter_enum(ipog_handle self, const char name[],
                                    const std::string *values,
                                    const int values_length) {
  static_cast<dither::Ipog *>(self)->add_parameter(name, values, values_length);
}

void dither_ipog_add_parameter(ipog_handle self, const char name[]) {
  static_cast<dither::Ipog *>(self)->add_parameter(name);
}

void dither_ipog_init_param_cache(ipog_handle self) {
  static_cast<dither::Ipog *>(self)->init_param_cache();
}

void dither_ipog_run(ipog_handle self) {
  static_cast<dither::Ipog *>(self)->run();
}

int dither_ipog_size(ipog_handle self) {
  return static_cast<dither::Ipog *>(self)->size();
}

void dither_ipog_display_raw_solution(ipog_handle self) {
  static_cast<dither::Ipog *>(self)->display_raw_solution();
}

std::string *dither_ipog_header(ipog_handle self) {
  return static_cast<dither::Ipog *>(self)->header();
}

void dither_add_constraint(ipog_handle self, const int constraint[], const int length) {
  static_cast<dither::Ipog *>(self)->add_constraint(constraint, length);
}
