### Dither-CXX

Dither-cxx is a C++11 library that implements IPOG.

# How to build / test
This repository is self-contained, with dependencies on gecode and googletest handled via git-submodules.

  git clone ...
  git submodule update --init --recursive
  mkdir build && cd build
  cmake ..
  cmake --build . --config {Release|Debug} --target dither dither_test 
  cmake --build . --config {Release|Debug} --target run_unit_tests 

Depending on your generator you might have to specify the build configuration during the cmake config stage:

  cmake .. -DCMAKE_BUILD_TYPE={Release|Debug}
  cmake --build . --target dither dither_test
  cmake --build . --target run_unit_tests

If you want to build dither as a dynamic library, use:

  cmake .. -DBUILD_SHARED_LIBS=ON
  cmake --build . --target dither dither_test
  cmake --build . --target run_unit_tests

# TODO

* evaluate gecode or YAP (Yet Another Prolog) for constraint handling (abandonded)
* try to optimize the merge in the vertical extension (done)
* write a DSL in mercury, flex/bison, prolog, minizinc, or lua for command line interface
* add benchmarks

* replace git-submodule dependencies by conan
* re-add constraint solving (interface seemed to have changed?)
* Fix compiler warnings
* Bump gecode version (4.4.0 -> 6.2.0)

# Copyright
BSD 3-Clause License

Copyright (c) 2015 Jason Gowan See LICENSE for details.
