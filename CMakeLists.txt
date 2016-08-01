CMAKE_MINIMUM_REQUIRED(VERSION 3.2) # travis-ci is stuck on this
PROJECT("Boids.Experiment" C)

# search for modules in./ cmake
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# reset configuration types
# mostly for VS
IF(CMAKE_CONFIGURATION_TYPES)
SET(CMAKE_CONFIGURATION_TYPES Debug Release)
SET(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
			      "Reset the configurations to what we need" FORCE)
ENDIF(CMAKE_CONFIGURATION_TYPES)

########################################
# build
########################################

# why is it set by default anyway?
STRING (REGEX REPLACE "-rdynamic" "" CMAKE_SHARED_LIBRARY_LINK_C_FLAGS
	"${CMAKE_SHARED_LIBRARY_LINK_C_FLAGS}")

# building inside travis-ci
IF (DEFINED ENV{TRAVIS})
	SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
ENDIF (DEFINED ENV{TRAVIS})

# don't cache build type
SET (CMAKE_BUILD_TYPE2 "${CMAKE_BUILD_TYPE}")
UNSET (CMAKE_BUILD_TYPE CACHE)
SET (CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE2}")
UNSET (CMAKE_BUILD_TYPE2)

# put compiled applications here
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

########################################
# build our project
ADD_SUBDIRECTORY(src)
