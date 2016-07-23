CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT ("Boids.Experiment" C)

# create executable
ADD_EXECUTABLE (boids boids.c)
TARGET_SOURCES (boids PRIVATE cleanlist.c)

# use compiler standard
IF (DEFINED MSVC OR ((DEFINED CMAKE_C_COMPILE_FEATURES) AND (CMAKE_C_COMPILE_FEATURES STREQUAL "")))
	MESSAGE (STATUS "MSVC detected")
	SET_PROPERTY (TARGET boids PROPERTY C_STANDARD 99)
	SET_PROPERTY (TARGET boids PROPERTY C_STANDARD_REQUIRED ON)
ELSE (DEFINED MSVC OR ((DEFINED CMAKE_C_COMPILE_FEATURES) AND (CMAKE_C_COMPILE_FEATURES STREQUAL "")))
########################################
# target_compile_features:
#   c_function_prototypes Function prototypes, as defined in ISO/IEC 9899:1990.
#   c_restrict restrict keyword, as defined in ISO/IEC 9899:1999.
#   c_static_assert Static assert, as defined in ISO/IEC 9899:2011.
#   c_variadic_macros Variadic macros, as defined in ISO/IEC 9899:1999.
########################################
	TARGET_COMPILE_FEATURES (boids PUBLIC c_variadic_macros)
ENDIF (DEFINED MSVC OR ((DEFINED CMAKE_C_COMPILE_FEATURES) AND (CMAKE_C_COMPILE_FEATURES STREQUAL "")))

# search for modules in ./cmake
LIST (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

########################################
# libraries
########################################
# link against SDL2
IF (NOT (DEFINED SDL2_LIBRARIES AND DEFINED SDL2_INCLUDE_DIRS))
	IF (DEFINED ENV{APPVEYOR})
		SET (SDL2_LIBRARIES "$ENV{SDL2_LIBRARIES}")
		SET (SDL2_INCLUDE_DIRS "$ENV{SDL2_INCLUDE_DIRS}")
	ELSE (DEFINED ENV{APPVEYOR})
		# it's much faster when paths are already in the cache
		# so try to NOT call find_package
		FIND_PACKAGE (SDL2 REQUIRED)
	ENDIF (DEFINED ENV{APPVEYOR})
ENDIF (NOT (DEFINED SDL2_LIBRARIES AND DEFINED SDL2_INCLUDE_DIRS))
# CMP0004 policy: remove trailing space
STRING (STRIP "${SDL2_LIBRARIES} " SDL2_LIBRARIES)
STRING (STRIP "${SDL2_INCLUDE_DIRS} " SDL2_INCLUDE_DIRS)
TARGET_INCLUDE_DIRECTORIES (boids PRIVATE "${SDL2_INCLUDE_DIRS}")
FOREACH (LIB ${SDL2_LIBRARIES})
	TARGET_LINK_LIBRARIES (boids PRIVATE ${LIB})
ENDFOREACH (LIB ${SDL2_LIBRARIES})

########################################
# get current version from git.
########################################
INCLUDE (GetGitRevisionDescription)
GIT_DESCRIBE (VERSION --tags --dirty=.dirty)
# separate version values
STRING (REGEX REPLACE "^v([0-9]+)\\.([0-9]+)([.p-]([.0-9a-fg-z-]+d?))?$" "\\1"
	VERSION_MAJOR "${VERSION}"
)
SET (VERSION_MINOR "${CMAKE_MATCH_2}")
SET (VERSION_REST "${CMAKE_MATCH_4}")
MESSAGE (STATUS "Version: ${VERSION_MAJOR}.${VERSION_MINOR}-${VERSION_REST}")
# create version file
SET (version_c "${CMAKE_CURRENT_BINARY_DIR}/version.c")
CONFIGURE_FILE ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.c.in" "${version_c}")
TARGET_SOURCES (boids PRIVATE "${version_c}")

########################################
# build types
########################################

# building inside travis-ci
IF (DEFINED ENV{TRAVIS})
	SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
ENDIF (DEFINED ENV{TRAVIS})

IF (CMAKE_BUILD_TYPE STREQUAL "Debug")
	MESSAGE (WARNING "Building debug version")
	SET (CMAKE_VERBOSE_MAKEFILE ON)
	SET_PROPERTY (TARGET boids APPEND PROPERTY COMPILE_DEFINITIONS "DEBUG")
ENDIF (CMAKE_BUILD_TYPE STREQUAL "Debug")

IF (DEFINED MSVC)
	MESSAGE (STATUS "Using MSVC compiler")
	IF ($ENV{PLATFORM} STREQUAL x86)
		SET (MSVC_C_ARCHITECTURE_ID x86)
	ENDIF ($ENV{PLATFORM} STREQUAL x86)
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall")
	SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ogixt")
	SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /Od")
ELSEIF (CMAKE_C_COMPILER_ID STREQUAL "Clang")
	MESSAGE (STATUS "Using Clang compiler")
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Weverything -Wno-documentation -Wno-documentation-unknown-command")
	SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
	SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
ELSEIF (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	MESSAGE (STATUS "Using GNU compiler")
	SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
	SET (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2")
	SET (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")
ELSE (DEFINED MSVC)
	MESSAGE (ERROR "Unsupported compiler.")
ENDIF (DEFINED MSVC)
