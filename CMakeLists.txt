cmake_minimum_required (VERSION 3.0)
project (Boids.Experiment)

########################################
# get current version from git.
set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
include (GetGitRevisionDescription)
git_describe (VERSION --tags --dirty=-d)

#message (STATUS "Version: ${VERSION}")
#parse the version information into pieces.
#string (REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p]([0-9]+))?(-([0-9a-f]+))?(-(x))?$" "\\1" VERSION_MAJOR "${VERSION}")
#set (VERSION_MINOR ${CMAKE_MATCH_2})
#set (VERSION_PATCH ${CMAKE_MATCH_4})
#set (VERSION_HASH ${CMAKE_MATCH_6})
#set (VERSION_DIRTY ${CMAKE_MATCH_8})
#message (STATUS "${VERSION_MAJOR} . ${VERSION_MINOR} . ${VERSION_PATCH} - ${VERSION_HASH} - ${VERSION_DIRTY}")

# separate version values
string (REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p-]([.0-9a-fg-z-]+d?))?$" "\\1" VERSION_MAJOR "${VERSION}")
set (VERSION_MINOR ${CMAKE_MATCH_2})
set (VERSION_REST ${CMAKE_MATCH_4})
message (STATUS "Version: ${VERSION_MAJOR} . ${VERSION_MINOR} - ${VERSION_REST}")

# create version file
set (version_c "${CMAKE_CURRENT_BINARY_DIR}/version.c")
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.c.in
		${version_c})

########################################
# target_compile_features:
#   c_function_prototypes
#     Function prototypes, as defined in ISO/IEC 9899:1990.
#   c_restrict
#     restrict keyword, as defined in ISO/IEC 9899:1999.
#   c_static_assert
#     Static assert, as defined in ISO/IEC 9899:2011.
#   c_variadic_macros
#     Variadic macros, as defined in ISO/IEC 9899:1999.
########################################

set (SOURCE_FILES boids.c ${version_c})
set (TARGET1 boids)
# create executable
add_executable (${TARGET1} ${SOURCE_FILES})
# use compiler standard
target_compile_features (${TARGET1} PUBLIC c_static_assert)
# link against SDL2
target_link_libraries (${TARGET1} SDL2)
