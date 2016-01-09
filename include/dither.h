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

ipog_handle dither_ipog_new(const unsigned int);
void dither_ipog_add_parameter_int(ipog_handle, const int, const int[],
                                   const int);
void dither_ipog_run(ipog_handle);
int dither_ipog_size(ipog_handle);
void dither_ipog_display_raw_solution(ipog_handle);
void dither_ipog_fill(ipog_handle, int[]);
void dither_ipog_delete(ipog_handle self);
void dither_ipog_add_constraint(ipog_handle self, const int[], const int);
void dither_ipog_add_previously_tested(ipog_handle self, const int[],
                                       const int);
}
#endif  // DITHER_H_
