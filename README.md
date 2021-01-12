### Dither-CXX

Dither-cxx is a C++11 library that implements IPOG.

# How to build / test
This repository is self-contained, with dependencies on googletest handled via git-submodules.

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

    cmake .. -DBUILD_SHARED_LIBS={ON|OFF}
    cmake --build . --target dither dither_test
    cmake --build . --target run_unit_tests

After building, all executables, static and shared libraries can be found under `build/bin`.
# TODO
* write a DSL in mercury, flex/bison, prolog, minizinc, or lua for command line interface
* add benchmarks
* Fix compiler warnings

# Copyright
BSD 3-Clause License

Copyright (c) 2015 Jason Gowan See LICENSE for details.
