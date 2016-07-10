cmake_minimum_required (VERSION 3.0)
project (Boids.Experiment C)

# create executable
add_executable (boids boids.c)
target_sources (boids PRIVATE cleanlist.c)

# use compiler standard
message(STATUS "${CMAKE_C_COMPILER_ID}")
if (CMAKE_C_COMPILER_ID STREQUAL MSVC)
	message(STATUS "MSVC detected")
	message(STATUS "$ENV{SDL2_LIBRARIES}")
	set_property(TARGET boids PROPERTY C_STANDARD 99)
	set_property(TARGET boids PROPERTY C_STANDARD_REQUIRED ON)
else ()
########################################
# target_compile_features:
#   c_function_prototypes Function prototypes, as defined in ISO/IEC 9899:1990.
#   c_restrict restrict keyword, as defined in ISO/IEC 9899:1999.
#   c_static_assert Static assert, as defined in ISO/IEC 9899:2011.
#   c_variadic_macros Variadic macros, as defined in ISO/IEC 9899:1999.
########################################
	target_compile_features (boids PUBLIC c_variadic_macros)
endif ()

########################################
# libraries
########################################
# link against SDL2
if (NOT (${SDL2_LIBRARIES} AND ${SDL2_INCLUDE_DIRS}))
	find_package (SDL2 REQUIRED)
endif ()
# CMP0004 policy
# trailing space issue
string (STRIP "${SDL2_LIBRARIES} " SDL2_LIBRARIES)
string (STRIP "${SDL2_INCLUDE_DIRS} " SDL2_INCLUDE_DIRS)
target_include_directories (boids PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries (boids ${SDL2_LIBRARIES})

########################################
# get current version from git.
########################################
list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (GetGitRevisionDescription)
git_describe (VERSION --tags --dirty=-d)
# separate version values
string (REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p-]([.0-9a-fg-z-]+d?))?$" "\\1" VERSION_MAJOR "${VERSION}")
set (VERSION_MINOR ${CMAKE_MATCH_2})
set (VERSION_REST ${CMAKE_MATCH_4})
message (STATUS "Version: ${VERSION_MAJOR} . ${VERSION_MINOR} - ${VERSION_REST}")
# create version file
set (version_c "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/version.c")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.c.in
		${version_c})
target_sources (boids PRIVATE ${version_c})

########################################
# build types
########################################

# building inside travis-ci
IF ($ENV{TRAVIS})
	SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
ENDIF ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	message (WARNING "Building debug version")
	set(CMAKE_VERBOSE_MAKEFILE ON)
	add_definitions(-DDEBUG)
endif ()

SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -Wextra")

