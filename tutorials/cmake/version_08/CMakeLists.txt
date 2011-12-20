cmake_minimum_required (VERSION 2.6)
project (Tutorial)

# should we use our own math functions?
option (USE_MATHFUN
	"Use tutorial provided math implementation" ON)

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
	"${PROJECT_BINARY_DIR}/TutorialConfig.h"
	)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the MathFunctions library?
if (USE_MATHFUN)
	include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
	add_subdirectory (MathFunctions)
	set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)
endif (USE_MATHFUN)

# add the executable
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial ${EXTRA_LIBS})

# add the install targets
install (TARGETS Tutorial DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
	DESTINATION include)

# enable testing is mandatory, otherwise tests won't be available
ENABLE_TESTING()

# The testing feature of cmake will generate a 'test' makefile target. However,
# this target doesn't depend on the executable, so we create a new target,
# 'check'. This target will first build the executable, and then run the tests.
# NOTE: This will also rebuild the executable before running tests. This may or
#       may not be what you want. The regular 'test' target won't do this.
# NOTE: Generally 'make all test' is preferred. The reason for this is that
#       target collisions may occur when nesting cmake projects.
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
	DEPENDS Tutorial)


#
# unit tests
#

# this time around with a macro to simplify adding tests
macro (do_test arg result)
	add_test(TutorialComp${arg} Tutorial ${arg})
	set_tests_properties (TutorialComp${arg}
		PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# does the application run
add_test (TutorialRuns Tutorial 25)

# does the usage message work?
add_test (TutorialUsage Tutorial)
set_tests_properties (TutorialUsage
	PROPERTIES
	PASS_REGULAR_EXPRESSION "Usage:.*number")

# does it compute the square root of 25
do_test (25 "25 is 5")

# does it handle negative numbers
do_test (-25 "-25 is -nan")

# does it handle small numbers
do_test (0.0001 "0.0001 is 0.01")
