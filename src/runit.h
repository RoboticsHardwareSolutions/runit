/**
 * @file
 * runit - the C unit test framework
 *
 */

#ifndef RUNIT_H
#define RUNIT_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Semantic version of this file and framework.
 */
#define RUNIT_VERSION "0.1.0"

#include <stdio.h>  /* For printf() */
#include <math.h>   /* For fabs(), fabsf(), isnan(), isinf(), isfinite() */
#include <string.h> /* For strncmp(), memcmp() */
#include <stddef.h> /* For size_t */

/**
 * Boolean indicating if all tests passed successfully (when 0) or not.
 *
 * Useful as a value to return from the main function of the test executable,
 * so that the test executable returns non-zero in case at least one test
 * failed.
 */
extern char runit_at_least_one_fail;

/**
 * Counter of all runit assertion macro calls that failed the check.
 *
 * The user should not change this value, but may freely read it.
 * Useful to inspect the amount of errors (which should be 0) for changes
 * during different launches of the test suite.
 */

extern unsigned int runit_counter_assert_failures;

/**
 * Counter of all runit assertion macro calls that passed the check.
 *
 * The user should not change this value, but may freely read it.
 * Useful to inspect the amount of passes for changes
 * during different launches of the test suite.
 */
extern unsigned int runit_counter_assert_passes;

/**
 * Absolute tolerance when comparing two single-precision floating point
 * value for approximate-equality using runit_fapprox().
 *
 * If the difference between the two float values is bigger than this
 * tolerance, the assertion fails.
 */
#define RUNIT_FLOAT_EQ_ABSTOL (1e-5f)

/**
 * Absolute tolerance when comparing two double-precision floating point
 * value for approximate-equality using runit_dapprox().
 *
 * If the difference between the two double values is bigger than this
 * tolerance, the assertion fails.
 */
#define RUNIT_DOUBLE_EQ_ABSTOL (1e-8)


/**
 * Prints a brief report message providing the point where this report is
 * and the amount of successes and failures at this point.
 *
 * Particularly useful to call it at the end of the test runner executable
 * or after a certain group of test cases to verify that progress through the
 * test suite is happening. In case of sudden crashes of the test suite,
 * multiple of these reports may be added to aid debugging in understanding
 * where the issue arisees.
 */
#define runit_report() \
    printf("REPORT | File: %s:%d | Test case: %s" \
           " | Passes: %5u | Failures: %5u\n", \
           RUNIT_FILENAME, __LINE__, __func__,     \
           runit_counter_assert_passes, runit_counter_assert_failures)

/**
 * Extract only filename from full path (handles both Unix and Windows paths)
 */
#define RUNIT_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : \
                        (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

/**
 * 
 * Verifies if the given boolean expression is true.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * It's the most generic macro offered by runit. If you need something more
 * specific, try using the other macros instead.
 *
 * The `do-while(0)` construct allows to write multi-line macros.
 *
 * If your system does not support `printf()`, replace it with something
 * else in this file! For example a `transmit()` function to communicate
 * the result to other devices.
 *
 * Example:
 * ```
 * runit_assert(1);      // Passes
 * runit_assert(0);      // Fails
 * runit_assert(3 < 1);  // Fails
 * ```
 */
#define runit_assert(expression) do {                   \
    if (!(expression)) {                               \
        printf("FAIL | File: %s:%d | Test case: %s\n", \
               RUNIT_FILENAME, __LINE__, __func__);    \
        runit_counter_assert_failures++;                \
        runit_at_least_one_fail = 1;                    \
        return;                                        \
    } else {                                           \
        runit_counter_assert_passes++;                  \
    }                                                  \
} while (0)

/**
 * Verifies if the given boolean expression is true.
 *
 * Just a rename of runit_assert() for consistency with runit_false().
 */
#define runit_true(x) runit_assert(x)

/**
 * Verifies if the given boolean expression is false.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_false(1);      // Fails
 * runit_false(0);      // Passes
 * ```
 */
#define runit_false(x) runit_assert(!(x))

/**
 * Verifies if the two arguments are exactly equal.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * It is recommended to use this macro instead of calling `runit_assert(a == b)`
 * to avoid mistyping the equality comparison operator `==` into the assignment
 * operator `=`.
 *
 * Example:
 * ```
 * runit_eq(12, 12);      // Passes
 * runit_eq(12.0f, 12U);  // Passes due to implicit conversion of 12U to 12.0f
 * runit_eq(100, 1);      // Fails
 * ```
 */
#define runit_eq(a, b) runit_assert((a) == (b))

/**
 * Verifies if the two arguments are not equal.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_neq(12, 12);      // Fails
 * runit_neq(12.0f, 12U);  // Fails
 * runit_neq(100, 1);      // Passes
 * ```
 */
#define runit_neq(a, b) runit_assert((a) != (b))

/**
 * Verifies if the first argument is strictly Greater Than the second.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_gt(1, 10);     // Fails
 * runit_gt(10, 10);    // Fails
 * runit_gt(100, 10);   // Passes
 * ```
 */
#define runit_gt(a, b) runit_assert((a) > (b))

/**
 * Verifies if the first argument is Greater or Equal to the second.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_ge(1, 10);     // Fails
 * runit_ge(10, 10);    // Passes
 * runit_ge(100, 10);   // Passes
 * ```
 */
#define runit_ge(a, b) runit_assert((a) >= (b))

/**
 * Verifies if the first argument is strictly Less Than the second.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_lt(1, 10);     // Passes
 * runit_lt(10, 10);    // Fails
 * runit_lt(100, 10);   // Fails
 * ```
 */
#define runit_lt(a, b) runit_assert((a) < (b))

/**
 * Verifies if the first argument is Less or Equal to the second.
 *
 * Otherwise stops the test case immediately and reports the failing file,
 * function and line number on standard output.
 *
 * Example:
 * ```
 * runit_le(1, 10);     // Passes
 * runit_le(10, 10);    // Fails
 * runit_le(100, 10);   // Fails
 * ```
 */
#define runit_le(a, b) runit_assert((a) <= (b))

/**
 * Verifies if two single-precision floating point values are within a given
 * absolute tolerance from each other.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_fdelta(1.0f, 1.00000001f, 0.1f);  // Passes
 * runit_fdelta(1.0f, 1.1f, 0.15f);        // Passes
 * runit_fdelta(1.0f, 2.0f, 0.1f);         // Fails
 * ```
 */
#define runit_fdelta(a, b, delta) runit_assert(fabsf((a) - (b)) <= fabsf(delta))

/**
 * Verifies if two single-precision floating point values are within a fixed
 * absolute tolerance #runit_FLOAT_EQ_ABSTOL from each other.
 *
 * Useful to check for almost-equality but ignoring minor rounding errors.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_fapprox(1.0f, 1.000001f);  // Passes
 * runit_fapprox(1.0f, 1.1);        // Fails
 * ```
 */
#define runit_fapprox(a, b) runit_fdelta((a), (b), RUNIT_FLOAT_EQ_ABSTOL)

/**
 * Verifies if two double-precision floating point values are within a given
 * absolute tolerance from each other.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_ddelta(1.0, 1.00000001, 0.1);  // Passes
 * runit_ddelta(1.0, 1.1, 0.15);        // Passes
 * runit_ddelta(1.0, 2.0, 0.1);         // Fails
 * ```
 */
#define runit_ddelta(a, b, delta) \
    runit_assert(fabs((a) - (b)) <= fabs(delta))

/**
 * Verifies if two double-precision floating point values are within a fixed
 * absolute tolerance #RUNIT_DOUBLE_EQ_ABSTOL from each other.
 *
 * Useful to check for almost-equality but ignoring minor rounding errors.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_dapprox(1.0, 1.00000001);  // Passes
 * runit_dapprox(1.0, 1.1);         // Fails
 * ```
 */
#define runit_dapprox(a, b) runit_ddelta((a), (b), RUNIT_DOUBLE_EQ_ABSTOL)

/**
 * Verifies that the floating point value is Not a Number (NaN).
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_nan(NAN);        // Passes
 * runit_nan(INFINITY);   // Fails
 * runit_nan(-INFINITY);  // Fails
 * runit_nan(1.0);        // Fails
 * runit_nan(1);          // Fails
 * ```
 */
#define runit_nan(value) runit_assert(isnan(value))

/**
 * Verifies that the floating point value is infinity, either positive or
 * negative.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_inf(NAN);        // Fails
 * runit_inf(INFINITY);   // Passes
 * runit_inf(-INFINITY);  // Passes
 * runit_inf(1.0);        // Fails
 * runit_inf(1);          // Fails
 * ```
 */
#define runit_inf(value) runit_assert(isinf(value))

/**
 * Verifies that the floating point value is positive infinity.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_plusinf(NAN);        // Fails
 * runit_plusinf(INFINITY);   // Passes
 * runit_plusinf(-INFINITY);  // Fails
 * runit_plusinf(1.0);        // Fails
 * runit_plusinf(1);          // Fails
 * ```
 */
#define runit_plusinf(value) runit_assert((isinf(value)) && ((value) > 0))

/**
 * Verifies that the floating point value is negative infinity.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_minusinf(NAN);        // Fails
 * runit_minusinf(INFINITY);   // Fails
 * runit_minusinf(-INFINITY);  // Passes
 * runit_minusinf(1.0);        // Fails
 * runit_minusinf(1);          // Fails
 * ```
 */
#define runit_minusinf(value) runit_assert((isinf(value)) && ((value) < 0))

/**
 * Verifies that the floating point value is finite, thus not NaN or
 * +/- infinity.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_finite(NAN);        // Fails
 * runit_finite(INFINITY);   // Fails
 * runit_finite(-INFINITY);  // Fails
 * runit_finite(1.0);        // Passes
 * runit_finite(1);          // Passes
 * ```
 */
#define runit_finite(value) runit_assert(isfinite(value))

/**
 * Verifies that the floating point value is not finite, thus either NaN or
 * +/- infinity.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_notfinite(NAN);        // Passes
 * runit_notfinite(INFINITY);   // Passes
 * runit_notfinite(-INFINITY);  // Passes
 * runit_notfinite(1.0);        // Fails
 * runit_notfinite(1);          // Fails
 * ```
 */
#define runit_notfinite(value) runit_assert(!isfinite(value))

/**
 * Verifies if the bits of the value specified by a bit mask are set to 1.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_flag(0x07, 1 << 1);  // Passes
 * runit_flag(0x07, 0x04);    // Passes
 * runit_flag(0x07, 0x06);    // Passes
 * runit_flag(0x07, 0xF0);    // Fails
 * ```
 */
#define runit_flag(value, mask) runit_assert(((value) & (mask)))

/**
 * Verifies if the bits of the value specified by a bit mask are set to 0.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_noflag(0x07, 1 << 5);  // Passes
 * runit_noflag(0x07, 0xF8);    // Passes
 * runit_noflag(0x07, 0x04);    // Fails
 * ```
*/
#define runit_noflag(value, mask) runit_assert(((value) & (mask)) == 0)

/**
 * Verifies if two strings are equal up to a given length or until the shortest
 * string terminates.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_streq("abcd", "abcd", 2);    // Passes
 * runit_streq("abcd", "abcd", 4);    // Passes
 * runit_streq("abcd", "abcd", 100);  // Passes, reached null terminator
 * runit_streq("abcd", "ABCD", 4);    // Fails, different casing
 * ```
 */
#define runit_streq(a, b, maxlen) \
    runit_assert(strncmp((a), (b), (maxlen)) == 0)

/**
 * Verifies if two memory sections are equal up to a given length.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_memeq("abcd", "abcd", 2);    // Passes
 * runit_memeq("abcd", "abcd", 4);    // Passes
 * runit_memeq("abcd", "abcd", 100);  // UNDEFINED as exceeding known memory
 * runit_memeq("abcd", "ABCD", 4);    // Fails
 * ```
 */
#define runit_memeq(a, b, len) runit_assert(memcmp((a), (b), len) == 0)

/**
 * Verifies if two memory sections are different within the given length.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_memneq("abcd", "abcd", 2);    // Fails
 * runit_memneq("abcd", "abcd", 4);    // Fails
 * runit_memneq("abcd", "abcd", 100);  // UNDEFINED as exceeding known memory
 * runit_memneq("abcd", "abCD", 4);    // Passes
 * ```
*/
#define runit_memneq(a, b, len) runit_assert(memcmp((a), (b), len) != 0)

/**
 * Verifies if a memory section is filled with just zeros.
 *
 * Useful to check whether a memory location has been cleared.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_zeros("abcd", 2);        // Fails
 * runit_zeros("\0\0cd", 2);      // Passes
 * runit_zeros("\0\0cd", 4);      // Fails
 * runit_zeros("\0\0\0\0", 4);    // Passes
 * runit_zeros("\0\0\0\0", 100);  // UNDEFINED as exceeding known memory
 * ```
*/
#define runit_zeros(x, len) do { \
        for (size_t __runit_idx = 0; __runit_idx < (size_t)(len); __runit_idx++) \
        { runit_eq(((uint8_t*)(x))[__runit_idx], 0); } \
    } while(0)

/**
 * Verifies if a memory section is not completely filled with zeros
 * (there is at least one non-zero byte).
 *
 * Useful to check whether a memory location has been initialised to non-zero
 * values, especially for random strings or data from an outside
 * source we don't know the exact format of.
 *
 * Otherwise stops the test case and reports on standard output.
 *
 * Example:
 * ```
 * runit_nzeros("abcd", 2);        // Passes
 * runit_nzeros("\0\0cd", 2);      // Fails
 * runit_nzeros("\0\0cd", 4);      // Passes
 * runit_nzeros("\0\0\0\0", 4);    // Fails
 * runit_nzeros("\0\0c\0", 4);     // Passes
 * runit_nzeros("a\0c\0", 3);      // Passes
 * runit_nzeros("\0\0\0\0", 100);  // UNDEFINED as exceeding known memory
 * ```
*/
#define runit_nzeros(x, len) do { \
        uint8_t __runit_all_zero = 1; \
        for (size_t __runit_idx = 0; __runit_idx < (size_t)(len); __runit_idx++) \
        { if(((uint8_t*)(x))[__runit_idx] != 0) \
            { __runit_all_zero = 0; break; } \
        } \
        runit_false(__runit_all_zero); \
    } while(0)

/**
 * Forces a failure of the test case, stopping it and reporting on standard
 * output.
 */
#define runit_fail() runit_assert(0)

#ifdef __cplusplus
}
#endif

#endif  /* RUNIT_H */
