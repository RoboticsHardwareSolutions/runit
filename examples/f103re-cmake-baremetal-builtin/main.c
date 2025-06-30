#include "hal.h"
#include "stdio.h"
#include "runit.h"

static volatile uint64_t s_ticks;  // Milliseconds since boot
void                     SysTick_Handler(void)
{  // SyStick IRQ handler, triggered every 1ms
    s_ticks++;
}

uint64_t mg_millis(void)
{                    // Let Mongoose use our uptime function
    return s_ticks;  // Return number of milliseconds since boot
}

static void test_initially_no_test_have_failed(void)
{
    runit_eq(runit_at_least_one_fail, 0);
}

void self_tests(void)
{
    test_initially_no_test_have_failed();
}

int main(void)
{
    self_tests();
    runit_report();

    for (;;)
    {
    }

    return 0;
}
