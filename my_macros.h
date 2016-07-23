#ifndef MY_MACROS_H
#  define MY_MACROS_H

#  include <stdio.h>

#  ifdef DEBUG
#    warning "Building debug version."

/* check if we can use __func__ */
#    if __STDC_VERSION__ >= 199901L
#      define ERROR(...) fprintf(stderr, "[%s:%d:%s()] " FIRST(__VA_ARGS__) "\n", __FILE__, __LINE__, __func__ REST(__VA_ARGS__))
#    else
#      define ERROR(...) fprintf(stderr, "[%s:%d] " FIRST(__VA_ARGS__) "\n", __FILE__, __LINE__ REST(__VA_ARGS__))
#    endif

/* https://stackoverflow.com/a/11172679 */
/* expands to the first argument */
#    define FIRST(...) FIRST_HELPER(__VA_ARGS__, throwaway)
#    define FIRST_HELPER(first, ...) first
/*
 * if there's only one argument:
 *    expands to nothing
 * if there is more than one argument:
 *    expands to a comma followed by everything but the first argument
 * only supports up to 9 arguments but can be trivially expanded
 */
#    define REST(...) REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#    define REST_HELPER(qty, ...) REST_HELPER2(qty, __VA_ARGS__)
#    define REST_HELPER2(qty, ...) REST_HELPER_##qty(__VA_ARGS__)
#    define REST_HELPER_ONE(first)
#    define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#    define NUM(...) \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE,\
                TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#    define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10

#    define DUMP(format, var) ERROR(#var " = " format, var)
#  else
#    define ERROR(...)
#    define DUMP(fmt, var)
#  endif

#endif // MY_MACROS_H
