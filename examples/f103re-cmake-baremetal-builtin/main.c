#include <stm32f103xe.h>
#include "stdio.h"
#include "runit.h"

static volatile uint64_t s_ticks;  // Milliseconds since boot
void                     SysTick_Handler(void)
{  // SyStick IRQ handler, triggered every 1ms
    s_ticks++;
}

static size_t expected_failures_counter = 0;

#define SHOULD_FAIL(failing)      \
    printf("Expected failure: "); \
    expected_failures_counter++;  \
    failing

static void test_initially_no_test_have_failed(void)
{
    runit_eq(runit_at_least_one_fail, 0);
}

static void test_assert(void)
{
    runit_assert(1);
    SHOULD_FAIL(runit_assert(0));
}

static void test_true(void)
{
    runit_true(1);
    SHOULD_FAIL(runit_true(0));
}

static void test_false(void)
{
    runit_false(3000 < 0);
    runit_false(0);
    SHOULD_FAIL(runit_false(1));
}

static void test_eq(void)
{
    runit_eq(12, 12);
    runit_eq(12.0f, 12U);
    SHOULD_FAIL(runit_eq(100, 1));
}

static void test_neq(void)
{
    runit_neq(100, 1);
    SHOULD_FAIL(runit_neq(12, 12));
}

static void test_neq_casting(void)
{
    SHOULD_FAIL(runit_neq(12.0f, 12U));
}

static void test_gt(void)
{
    runit_gt(100, 1);
    SHOULD_FAIL(runit_gt(1, 100));
}

static void test_gt_equality(void)
{
    SHOULD_FAIL(runit_gt(100, 100));
}

static void test_ge(void)
{
    runit_ge(100, 1);
    SHOULD_FAIL(runit_ge(1, 100));
}

static void test_ge_equality(void)
{
    runit_ge(100, 100);
}

static void test_lt(void)
{
    runit_lt(1, 100);
    SHOULD_FAIL(runit_lt(100, 1));
}

static void test_lt_equality(void)
{
    SHOULD_FAIL(runit_lt(100, 100));
}

static void test_le(void)
{
    runit_le(1, 100);
    SHOULD_FAIL(runit_le(100, 1));
}

static void test_le_equality(void)
{
    runit_le(100, 100);
}

static void test_fapprox(void)
{
    runit_fapprox(1.0f, 1.0f);
    runit_fapprox(1.0f, 1U);
    runit_fapprox(1.0f, 1.000001f);
    SHOULD_FAIL(runit_fapprox(1.0f, 1.1f));
}

static void test_fdelta(void)
{
    runit_fdelta(1.0f, 1.0f, 0.01f);
    runit_fdelta(1.0f, 1.1f, 0.15f);
    runit_fdelta(1.0f, 1.000001f, 0.01f);
    runit_fdelta(1.0f, 1.00000001f, 0.1f);
    SHOULD_FAIL(runit_fdelta(1.0f, 1.1f, 0.01f));
}

static void test_fdelta_negatives(void)
{
    runit_fdelta(-1.0f, -1.0f, 0.01f);
    runit_fdelta(-1.0f, -1.0f, -0.01f);
    runit_fdelta(-1.0f, -1.000001f, 0.01f);
    runit_fdelta(-1.0f, -1.000001f, -0.01f);
    runit_fdelta(-1.0000001f, -1.0f, 0.01f);
    runit_fdelta(-1.0000001f, -1.0f, -0.01f);
    SHOULD_FAIL(runit_fdelta(-1.0f, -1.1f, 0.01f));
}

static void test_dapprox(void)
{
    runit_dapprox(1.0, 1.0);
    runit_dapprox(1.0, 1U);
    runit_dapprox(1.0, 1.00000001);
    SHOULD_FAIL(runit_dapprox(1.0, 1.1));
}

static void test_ddelta(void)
{
    runit_ddelta(1.0, 1.1, 0.15);
    runit_ddelta(1.0, 1.0, 0.01);
    runit_ddelta(1.0, 1.000001, 0.01);
    SHOULD_FAIL(runit_ddelta(1.0, 1.1, 0.01));
}

static void test_ddelta_negatives(void)
{
    runit_ddelta(-1.0, -1.0, 0.01);
    runit_ddelta(-1.0, -1.0, -0.01);
    runit_ddelta(-1.0, -1.000001, 0.01);
    runit_ddelta(-1.0, -1.000001, -0.01);
    runit_ddelta(-1.0000001, -1.0, 0.01);
    runit_ddelta(-1.0000001, -1.0, -0.01);
    SHOULD_FAIL(runit_ddelta(-1.0, -1.1, 0.01));
}

static void test_nan(void)
{
    runit_nan(NAN);
    runit_nan(nan(""));
    runit_nan(nanf(""));
}

static void test_nan_finite_float(void)
{
    SHOULD_FAIL(runit_nan(1.0f));
}

static void test_nan_finite_double(void)
{
    SHOULD_FAIL(runit_nan(1.0));
}

static void test_nan_infinity(void)
{
    SHOULD_FAIL(runit_nan(INFINITY));
}

static void test_inf(void)
{
    runit_inf(INFINITY);
    runit_inf(+INFINITY);
    runit_inf(-INFINITY);
}

static void test_inf_finite_float(void)
{
    SHOULD_FAIL(runit_inf(1.0f));
}

static void test_inf_finite_double(void)
{
    SHOULD_FAIL(runit_inf(1.0));
}

static void test_inf_nan(void)
{
    SHOULD_FAIL(runit_inf(NAN));
}

static void test_plusinf(void)
{
    runit_plusinf(INFINITY);
    runit_plusinf(+INFINITY);
    SHOULD_FAIL(runit_plusinf(-INFINITY));
}

static void test_plusinf_finite_float(void)
{
    SHOULD_FAIL(runit_plusinf(1.0f));
}

static void test_plusinf_finite_double(void)
{
    SHOULD_FAIL(runit_plusinf(1.0));
}

static void test_plusinf_nan(void)
{
    SHOULD_FAIL(runit_plusinf(NAN));
}

static void test_minusinf(void)
{
    runit_minusinf(-INFINITY);
    SHOULD_FAIL(runit_minusinf(INFINITY));
}

static void test_minusinf_finite_float(void)
{
    SHOULD_FAIL(runit_minusinf(1.0f));
}

static void test_minusinf_finite_double(void)
{
    SHOULD_FAIL(runit_minusinf(1.0));
}

static void test_minusinf_nan(void)
{
    SHOULD_FAIL(runit_minusinf(NAN));
}

static void test_notfinite(void)
{
    runit_notfinite(INFINITY);
    runit_notfinite(+INFINITY);
    runit_notfinite(-INFINITY);
    runit_notfinite(NAN);
    runit_notfinite(nanf(""));
    runit_notfinite(nan(""));
}

static void test_notfinite_finite_float(void)
{
    SHOULD_FAIL(runit_notfinite(1.0f));
}

static void test_notfinite_finite_double(void)
{
    SHOULD_FAIL(runit_notfinite(1.0));
}

static void test_finite(void)
{
    runit_finite(0.0f);
    runit_finite(-0.0f);
    runit_finite(0.0);
    runit_finite(-0.0);
    runit_finite(1.0f);
    runit_finite(1.0);
    runit_finite(-1.0f);
    runit_finite(-1.0);
}

static void test_finite_plusinf(void)
{
    SHOULD_FAIL(runit_finite(INFINITY));
}

static void test_finite_minusinf(void)
{
    SHOULD_FAIL(runit_finite(-INFINITY));
}

static void test_finite_nan_macro(void)
{
    SHOULD_FAIL(runit_finite(NAN));
}

static void test_finite_nan_call(void)
{
    SHOULD_FAIL(runit_finite(nan("")));
}

static void test_finite_nanf_call(void)
{
    SHOULD_FAIL(runit_finite(nanf("")));
}

static void test_flag(void)
{
    runit_flag(1, 1);
    runit_flag(0xFF, 1);
    runit_flag(0xFF, 2);
    runit_flag(0xFF, 3);
    runit_flag(0xFF, 0x0F);
    runit_flag(0xFF, 0xFF);
    runit_flag(0x18, 0x08);
    runit_flag(0x18, 0x10);
    runit_flag(0x07, 1 << 1);
    runit_flag(0x07, 0x04);
    runit_flag(0x07, 0x06);
    SHOULD_FAIL(runit_flag(0x07, 0xF0));
}

static void test_flag_when_none(void)
{
    /* runit_flag() checks for flag presence. To check for their absence,
     * runit_noflag() should be used instead; or even runit_eq(flags, 0). */
    SHOULD_FAIL(runit_flag(0, 0));
}

static void test_noflag(void)
{
    runit_noflag(0, 1);
    runit_noflag(2, 1);
    runit_noflag(3, 8);
    runit_noflag(0x0F, 0xF0);
    runit_noflag(0xFF, 0);
    runit_noflag(0, 0);
    runit_noflag(0x07, 1U << 5U);
    runit_noflag(0x07, 0xF8);
    SHOULD_FAIL(runit_noflag(0x07, 0x04));
}

static void test_streq(void)
{
    const char a[] = "hello";
    const char b[] = "hello";
    const char c[] = "HELLO";

    runit_streq(a, b, 0);
    runit_streq(a, b, 1);
    runit_streq(a, b, 2);
    runit_streq(a, b, 3);
    runit_streq(a, b, 4);
    runit_streq(a, b, 5);
    runit_eq(a[5], b[5]);
    runit_streq(a, b, 6);  // Null terminator
    runit_streq("", "", 0);
    SHOULD_FAIL(runit_streq(a, c, 5));
}

static void test_memeq(void)
{
    const uint8_t a[] = {255, 255, 255, 255, 255};
    const uint8_t b[] = {255, 255, 255, 255, 255};
    const uint8_t c[] = {255, 255, 255, 255, 55};

    runit_memeq(a, b, 0);
    runit_memeq(a, b, 1);
    runit_memeq(a, b, 2);
    runit_memeq(a, b, 3);
    runit_memeq(a, b, 4);
    runit_memeq(a, b, 5);
    runit_memeq("", "", 0);
    runit_memeq("abcd", "abcd", 2);
    runit_memeq("abcd", "abcd", 4);
    SHOULD_FAIL(runit_memeq(c, a, 5));
}

static void test_memneq(void)
{
    const uint8_t a[] = {255, 255, 255, 255, 255};
    const uint8_t b[] = {255, 255, 255, 255, 255};
    const uint8_t c[] = {11, 22, 33, 44, 55};

    runit_memneq(a, c, 1);
    runit_memneq(a, c, 2);
    runit_memneq(a, c, 3);
    runit_memneq(a, c, 4);
    runit_memneq(a, c, 5);
    runit_memneq("abcd", "ABcd", 2);
    runit_memneq("abcd", "ABcd", 4);
    runit_memneq("abcd", "abcD", 4);
    SHOULD_FAIL(runit_memneq(a, b, 5));
}

static void test_zeros(void)
{
    const uint8_t  a[] = {0, 0, 0, 0, 0};
    const uint8_t  b[] = {0, 0, 255, 255, 255};
    const uint32_t d[] = {0, 0, 0};

    runit_zeros(a, 0U);
    runit_zeros(a, 1U);
    runit_zeros(a, 2U);
    runit_zeros(a, 3U);
    runit_zeros(a, 4U);
    runit_zeros(a, 5U);
    runit_zeros(b, 2U);
    runit_zeros(d, 3U * sizeof(uint32_t));
    runit_zeros("\0\0\0", 3U);
    SHOULD_FAIL(runit_zeros(b, 5U));
}

static void test_nzeros(void)
{
    const uint8_t a[] = {0, 0, 0, 0, 0};
    const uint8_t b[] = {0, 0, 255, 255, 255};
    const uint8_t c[] = {11, 22, 33, 0, 0};

    runit_nzeros(b, 3U);
    runit_nzeros(c, 3U);
    runit_nzeros(c, 4U);
    runit_nzeros(c, 5U);
    runit_nzeros(&c[2], 1U);
    runit_nzeros(&c[2], 2U);
    runit_nzeros(&c[2], 3U);
    runit_nzeros("\0\0c\0", 4U);
    runit_nzeros("a\0c\0", 4U);
    runit_report();  // Dummy report somewhere to check it's working properly
    SHOULD_FAIL(runit_nzeros(a, 5U));
}

static void test_fail(void)
{
    SHOULD_FAIL(runit_fail());
}

static void test_at_the_end_some_tests_have_failed(void)
{
    runit_eq(runit_at_least_one_fail, 1);
}

static void start_self_tests(void)
{
    test_initially_no_test_have_failed();
    test_assert();
    test_true();
    test_false();
    test_eq();
    test_neq();
    test_neq_casting();
    test_gt();
    test_gt_equality();
    test_ge();
    test_ge_equality();
    test_lt();
    test_lt_equality();
    test_le();
    test_le_equality();
    test_fapprox();
    test_fdelta();
    test_fdelta_negatives();
    test_dapprox();
    test_ddelta();
    test_ddelta_negatives();
    test_nan();
    test_nan_finite_float();
    test_nan_finite_double();
    test_nan_infinity();
    test_inf();
    test_inf_finite_float();
    test_inf_finite_double();
    test_inf_nan();
    test_plusinf();
    test_plusinf_finite_float();
    test_plusinf_finite_double();
    test_plusinf_nan();
    test_minusinf();
    test_minusinf_finite_float();
    test_minusinf_finite_double();
    test_minusinf_nan();
    test_notfinite();
    test_notfinite_finite_float();
    test_notfinite_finite_double();
    test_finite();
    test_finite_plusinf();
    test_finite_minusinf();
    test_finite_nan_macro();
    test_finite_nanf_call();
    test_finite_nan_call();
    test_flag();
    test_flag_when_none();
    test_noflag();
    test_streq();
    test_memeq();
    test_memneq();
    test_zeros();
    test_nzeros();
    test_fail();
    test_at_the_end_some_tests_have_failed();
    runit_report();
}

int main(void)
{
    start_self_tests();
    
    if (expected_failures_counter != runit_counter_assert_failures)
        printf("Expected %u failures, but got %u\n", expected_failures_counter, runit_counter_assert_failures);
    else
        printf("All tests passed successfully!\n");

    for (;;)
    {
    }
    return 0;
}
