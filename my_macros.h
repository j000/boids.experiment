#ifndef MY_MACROS_H
#  define MY_MACROS_H

#  include <stdio.h>

/*
 * variadic macro expansion 
 * unfortunatelly there are some issues with comas, so we need to do this like this
 * https://stackoverflow.com/a/11172679
 */
/* expands to the first argument */
#  define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#  define FIRST_HELPER(first, ...) first
/*
 * if there's only one argument:
 *    expands to nothing
 * if there is more than one argument:
 *    expands to a comma followed by everything but the first argument
 * only supports up to 9 arguments but can be trivially expanded
 */
#  define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#  define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#  define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#  define REST_HELPER_ONE(first)
#  define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#  define NUM(...) \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, \
                TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#  define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

/* MS VS sets this when using GUI, so let's set our version */
#  if defined(_DEBUG) && !defined(DEBUG)
#    define DEBUG
#  endif

/* some code only for debug version */
#  ifdef DEBUG
#    pragma message "Building debug version."

/* check if we can use __func__ */
#    if __STDC_VERSION__ >= 199901L
#      define ERROR(...) fprintf(stderr, "[%s:%d:%s()] " FIRST(__VA_ARGS__) "\n", __FILE__, __LINE__, __func__ REST(__VA_ARGS__))
#    else
#      define ERROR(...) fprintf(stderr, "[%s:%d] " FIRST(__VA_ARGS__) "\n", __FILE__, __LINE__ REST(__VA_ARGS__))
#    endif

#    define DUMP(format, var) ERROR(#var " = " format, var)

#  else				// DEBUG

/* expand to nothing */
#    define ERROR(...)
#    define DUMP(format, var)

#  endif			// DEBUG

// http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
// http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros
#  if defined(__GNUC__) || defined(__clang__)	// different flags for different compilers
#    define NORETURN __attribute__ ((noreturn))
/* skip SDL errors */
#    if defined(__clang__)
#      define SDL_PUSH_WARNINGS _Pragma ("GCC diagnostic push") \
		_Pragma ("clang diagnostic ignored \"-Wreserved-id-macro\"") \
		_Pragma ("clang diagnostic ignored \"-Wunknown-pragmas\"") \
		_Pragma ("clang diagnostic ignored \"-Wmacro-redefined\"") \
		_Pragma ("clang diagnostic ignored \"-Wpadded\"")
#      define SDL_POP_WARNINGS _Pragma ("GCC diagnostic pop")
#    else			// __clang__
#      define SDL_PUSH_WARNINGS
#      define SDL_POP_WARNINGS
#    endif			// __clang__
#  elif defined(_MSC_VER)	// MSVC
#    define NORETURN __declspec (noreturn)
/* skip SDL errors */
#    define SDL_PUSH_WARNINGS __pragma ("warning(push)") \
		__pragma ("warning(disable: 4820)")
#    define SDL_POP_WARNINGS __pragma ("warning(pop)")
#  endif			// compilers

#endif				// MY_MACROS_H
