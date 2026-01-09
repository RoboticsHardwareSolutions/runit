/**
 * @file
 * @internal
 * runit - the  C unit test framework
 *
 */

#include "stddef.h"

char         runit_at_least_one_fail       = 0;
unsigned int runit_counter_assert_failures = 0;
unsigned int runit_counter_assert_passes   = 0;
