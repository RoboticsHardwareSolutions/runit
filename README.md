runit - the C unit test framework
===============================================================================

runit is the simplest-to-use C unit test framework, in just one header file,
without `malloc()`, without `fork()`, without dependencies, ready for
embedded systems that can at least call `printf()` - and even those who cannot,
can easily adapt it!

Most probably you will understand most about runit if you just read its
header: [`runit.h`](src/runit.h). I promise it's not that long, if you exclude
the documentation.



Why should I use runit instead of another framework?
-------------------------------------------------------

The goal of a unit test is generally very simple: verify that the obtained
value equals to the expected one. More generally if a boolean expression is
true (e.g. a value is in the allowed range).

There are so many complex unit test frameworks to do this in C and C++
which just increases complexity of the whole project. Suppose you are just
writing a simple program and want to test it. Oh, boy! Start managing the
dependencies for the framework, check if you can install it, maybe you need
Docker etc. Some frameworks even require `fork()` - how could that work on
an embedded system?

The output is basic, but enough: indicates just where the test case failed, by
filename and line number. Open that file, go to that line. There is the error.
Start using the debugger around that point.

Passing tests are not printed, to avoid cluttering the output.
You can still explicitly request a status report at any point in the 
test suite codebase (e.g. after a cluster of testcases of similar nature)
with `runit_report()`.



Dependencies
----------------------------------------

Only the C standard library!

- `stdio.h`, for `printf()` - if your system does not have it, replace the
  **one** call of `printf()` in `runit.h` with something else!
- `math.h`, for `fabs()`, `fabsf()`, `isnan()`, `isinf()`, `isfinite()`
- `string.h`, for `strncmp()`, `memcmp()`
- `stddef.h` for `size_t`

**No `malloc()` or `fork()` required**



How to use
----------------------------------------

### Basic idea and example

Use the `runit_*` macros in your test cases.

Here is an example where we test the `sqrt()` function from `math.h`:

```c
#include <math.h>
#include "runit.h"

static void test_sqrt_valid_values(void)
{
    runit_eq(0.0, sqrt(0.0));
    runit_eq(1.0, sqrt(1.0));
    runit_eq(2.0, sqrt(4.0));
    runit_ddelta(1.4142, sqrt(2.0), 0.001);
    runit_ddelta(1.7321, sqrt(3.0), 1e-8); // This fails!
                                          // 1.7321 is not accurate enough
                                          // The test case stops here.
    runit_ddelta(2.2361, sqrt(5.0), 1e-8); // This is NOT executed!
}

static void test_sqrt_negative_values(void)
{
    runit_nan(sqrt(-1.0));
}

int main(void)
{
    test_sqrt_valid_values();
    test_sqrt_negative_values();
    runit_report();  // Print a small one-line status report
    // Looks approximately like this
    // REPORT | File: /path/to/my/file.c:27 | Test case: main | Passes: 5 | Failures: 1
    return runit_at_least_one_fail; // Non-zero in case of error to provide a proper exit-code
}
```

### Real-world examples




### Complete recipe for the creation of a test suite with runit

1. Add the files `runit.h`, `runit.c` to your project:
   - You can copy the whole `src` folder into your project.
     This is probably the fastest and simplest solution.
   - Alternatively, use `git subtree` or `git submodules` to include this repo into your own
2. Create a file with your tests, say `test.c`.
3. Add test case functions returning `void` to `test.c`, as the two
  `test_sqrt_*` functions in the example above.
4. In each test case call `runit_assert()`, `runit_eq()`, `runit_flag()` etc.
   to verify the values you are testing. On a fail, the test case is terminated
   early and the lines of code after that are not executed.
5. Add a `main()` function to `test.c` which returns `runit_at_least_one_fail`.
   By doing so, the exit code of the test executable will be `1` in case at 
   least one test failed. Particularly useful when running the test executables
   in a pipeline that should stop when something is broken. Of course on 
   embedded systems one can use `runit_at_least_one_fail` in different ways, as
   there is no returning from `main`; for example by transmitting it via UART.
6. Call the test case functions from the `main()`.
7. Compile `test.c` into an executable and run it.
8. Check its standard output and the process exit code for failed tests.
9. Bonus: add some calls to `runit_report()` wherever you want in the test suite
   code. At least one call at the very end (before the `main` returns)
   is suggested, but also after a set of similar test cases is a good choice
   to see where something is making the test suite crash, in case so happens.


### A test case is failing. Now what?

The output will contain one or more lines like:

```
FAIL | File: /path/to/some_project/test.c:182 | Test case: test_valid_input_length
```

1. Open the file `/path/to/some_project/test.c`
2. Go to line 182 (use some keyboard shortcut), which is in the function
   `test_valid_input_length()`
   
   Note: sometimes the `path:linenumber` pattern is identified as
   clickable by your IDE. Maybe that speeds up your search.
3. The assertion on that specific line failed. Now up to you to debug why.
   The easiest way is to place a debugger breakpoint earlier in the test case.
   
Once the standard output does not contain any `FAIL` lines and the exit code
of the process is `0`, you are good to go!

