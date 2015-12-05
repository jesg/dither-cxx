/*
 *
 * Copyright (C) 2015 Jason Gowan
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#ifndef DITHER_TYPES_H_
#define DITHER_TYPES_H_

#include <string>

namespace dither {

typedef signed char dval;

#define DITHER_BOOL_T 0
#define DITHER_INT_T 1
#define DITHER_STRING_T 2

struct param {
  std::size_t first;
  dval second;
  std::string name;
  unsigned char type;
};
typedef std::vector<dval> dtest_case;
}

#endif
