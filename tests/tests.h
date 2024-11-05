#ifndef TESTS_H
#define TESTS_H

#define PASS_STRING GREEN("[PASS] ") "%s: %s == %s\n"
#define FAIL_STRING RED("[FAIL] ") "%s: %s != %s\n\t(Expected: \"%s\", Got: \"%s\")\n"

#include "../src/util/defines.h"
#define EXPECT_STRING(name, expected, actual) (strcmp((expected), (actual)) == 0 ? \
    (printf(PASS_STRING, name, #expected, #actual), 1) : \
    (printf(FAIL_STRING , name, #expected, #actual, (expected), (actual)), 0))

#define EXPECT_TRUE(value, name) value != 0 ? \
    (printf(GREEN("[PASS] ") "%s\n", name), 1) : \
    (printf(RED("[FAIL] ") "%s\n", name), 0)

#define RUN_TEST(function) do { \
        puts(CYAN("RUNNING TEST: " #function)); \
        int total = 0; \
        int passed = function(&total); \
        printf(CYAN(#function " PASSED [%d/%d] TESTS.\n\n"), passed, total); \
    } while (0)
#endif
