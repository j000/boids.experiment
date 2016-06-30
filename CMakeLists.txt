cmake_minimum_required (VERSION 3.0)
project (Boids.Experiment)

# Appends the cmake/modules path to MAKE_MODULE_PATH variable.
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Make a version file containing the current version from git.
#
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-d)

#message(STATUS "Version: ${VERSION}")

#parse the version information into pieces.
#string(REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p]([0-9]+))?(-([0-9a-f]+))?(-(x))?$" "\\1" VERSION_MAJOR "${VERSION}")
#set(VERSION_MINOR ${CMAKE_MATCH_2})
#set(VERSION_PATCH ${CMAKE_MATCH_4})
#set(VERSION_HASH ${CMAKE_MATCH_6})
#set(VERSION_DIRTY ${CMAKE_MATCH_8})
#message(STATUS "${VERSION_MAJOR} . ${VERSION_MINOR} . ${VERSION_PATCH} - ${VERSION_HASH} - ${VERSION_DIRTY}")

string(REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p-]([-.0-9a-f]+d?))?$" "\\1" VERSION_MAJOR "${VERSION}")
set(VERSION_MINOR ${CMAKE_MATCH_2})
set(VERSION_REST ${CMAKE_MATCH_4})
message(STATUS "Version: ${VERSION_MAJOR} . ${VERSION_MINOR} - ${VERSION_REST}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.c.in
		${CMAKE_CURRENT_BINARY_DIR}/version.c)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.c")

add_executable(boids boids.c ${version_file})
