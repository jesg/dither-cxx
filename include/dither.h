/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef DITHER_H_
#define DITHER_H_

#include <string>
#include "ipog.h"

extern "C" {

typedef void* ipog_handle;

ipog_handle dither_ipog_new();
void dither_ipog_set_t(ipog_handle self, const int t);
void dither_ipog_add_parameter_int(ipog_handle, const char[], const int[],
                                   const int);
void dither_ipog_add_parameter_enum(ipog_handle, const char[],
                                    const std::string[], const int);
void dither_ipog_add_parameter(ipog_handle, const char[]);
void dither_ipog_init_param_cache(ipog_handle);
void dither_ipog_run(ipog_handle);
int dither_ipog_size(ipog_handle);
void dither_ipog_display_raw_solution(ipog_handle);
std::string* dither_ipog_header(ipog_handle);
void dither_ipog_delete(ipog_handle self);
}
#endif  // DITHER_H_
