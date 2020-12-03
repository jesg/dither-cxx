# - Try to find Gecode
# Once done this will define
#  GECODE_FOUND          - System has Gecode
#  GECODE_INCLUDE_DIRS   - The Gecode include directories
#  GECODE_LIBRARIES      - The libraries needed to use Gecode
#  GECODE_TARGETS        - The names of imported targets created for gecode
# User can set Gecode_ROOT to the preferred installation prefix

find_path(GECODE_INCLUDE gecode/kernel.hh
          PATH_SUFFIXES include)

find_file(GECODE_CONFIG_LOC gecode/support/config.hpp
          HINTS ${GECODE_INCLUDE}
          PATH_SUFFIXES include)

if(NOT "${GECODE_CONFIG_LOC}" STREQUAL "GECODE_CONFIG_LOC-NOTFOUND")
  file(READ "${GECODE_CONFIG_LOC}" GECODE_CONFIG)
  string(REGEX MATCH "\#define GECODE_VERSION \"([0-9]+.[0-9]+.[0-9]+)\"" _ "${GECODE_CONFIG}")
  set(GECODE_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "\#define GECODE_LIBRARY_VERSION \"([0-9]+-[0-9]+-[0-9]+)\"" _ "${GECODE_CONFIG}")
  set(GECODE_LIBRARY_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "\#define GECODE_STATIC_LIBS ([0-9]+)" _ "${GECODE_CONFIG}")
  set(GECODE_STATIC_LIBS "${CMAKE_MATCH_1}")
  string(REGEX MATCH "\#define GECODE_HAS_GIST" GECODE_HAS_GIST "${GECODE_CONFIG}")
  string(REGEX MATCH "\#define GECODE_HAS_MPFR" GECODE_HAS_MPFR "${GECODE_CONFIG}")
  unset(GECODE_CONFIG)
endif()
unset(GECODE_CONFIG_LOC)

set(GECODE_COMPONENTS Driver Flatzinc Float Int Kernel Minimodel Search Set Support)
if(GECODE_HAS_GIST)
  list(APPEND GECODE_COMPONENTS Gist)
endif()

foreach(GECODE_COMP ${GECODE_COMPONENTS})
  # Try to find gecode library
  string(TOLOWER "gecode${GECODE_COMP}" GECODE_LIB)
  set(GECODE_LIB_LOC_RELEASE "GECODE_LIB_LOC_RELEASE-NOTFOUND")
  set(GECODE_LIB_LOC_DEBUG "GECODE_LIB_LOC_DEBUG-NOTFOUND")
  find_library(GECODE_LIB_LOC_RELEASE NAMES ${GECODE_LIB} lib${GECODE_LIB} ${GECODE_LIB}-${GECODE_LIBRARY_VERSION}-r-x64
               HINTS ${GECODE_INCLUDE}
               PATH_SUFFIXES lib)
               find_library(GECODE_LIB_LOC_DEBUG NAMES ${GECODE_LIB} lib${GECODE_LIB} ${GECODE_LIB}-${GECODE_LIBRARY_VERSION}-d-x64
               HINTS ${GECODE_INCLUDE}
               PATH_SUFFIXES lib)

  if(NOT "${GECODE_LIB_LOC_RELEASE}" STREQUAL "GECODE_LIB_LOC_RELEASE-NOTFOUND" AND NOT "${GECODE_LIB_LOC_DEBUG}" STREQUAL "GECODE_LIB_LOC_DEBUG-NOTFOUND")
      list(APPEND GECODE_LIBRARY ${GECODE_LIB_LOC_RELEASE})
      add_library(Gecode::${GECODE_COMP} INTERFACE IMPORTED)
      target_link_libraries(Gecode::${GECODE_COMP} INTERFACE $<$<CONFIG:Release>:${GECODE_LIB_LOC_RELEASE}>)
      target_link_libraries(Gecode::${GECODE_COMP} INTERFACE $<$<CONFIG:Debug>:${GECODE_LIB_LOC_DEBUG}>)
      target_include_directories(Gecode::${GECODE_COMP} INTERFACE ${GECODE_INCLUDE})
      set(Gecode_FIND_REQUIRED_${GECODE_COMP} TRUE)
      set(Gecode_${GECODE_COMP}_FOUND TRUE)
  endif()
endforeach(GECODE_COMP)

if(WIN32 AND GECODE_HAS_GIST AND GECODE_STATIC_LIBS)
  find_package(Qt5 QUIET COMPONENTS Core Gui Widgets PrintSupport)
  set_target_properties(Gecode::Gist PROPERTIES
                        INTERFACE_LINK_LIBRARIES "Qt5::Core;Qt5::Gui;Qt5::Widgets;Qt5::PrintSupport")
endif()

unset(GECODE_REQ_LIBS)
unset(GECODE_LIB_WIN)
unset(GECODE_LIB_LOC)

if(GECODE_LIBRARY AND GECODE_HAS_MPFR)
  find_package(MPFR)
  list(APPEND GECODE_LIBRARY ${MPFR_LIBRARIES})
  list(APPEND GECODE_TARGETS ${MPFR_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GECODE_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
  Gecode
  REQUIRED_VARS GECODE_INCLUDE GECODE_LIBRARY
  VERSION_VAR GECODE_VERSION
  HANDLE_COMPONENTS
  FAIL_MESSAGE "Could NOT find Gecode, use Gecode_ROOT to hint its location"
)

mark_as_advanced(GECODE_INCLUDE GECODE_LIBRARY)

set(GECODE_LIBRARIES ${GECODE_LIBRARY})
set(GECODE_INCLUDE_DIRS ${GECODE_INCLUDE})