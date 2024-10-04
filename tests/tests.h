#ifndef TESTS_H
#define TESTS_H

#include "../src/util/defines.h"
#define EXPECT_INT(expected, actual) ((expected) == (actual) ? \
    (printf("[PASS] %s == %s\n", #expected, #actual), 1) : \
    (printf("[FAIL] %s != %s (Expected: %d, Got: %d)\n", #expected, #actual, (expected), (actual)), 0))

#define EXPECT_STRING(name, expected, actual) (strcmp((expected), (actual)) == 0 ? \
    (printf(GREEN("[PASS] ") "%s: %s == %s\n", name, #expected, #actual), 1) : \
    (printf(RED("[FAIL] ") "%s: %s != %s (Expected: \"%s\", Got: \"%s\")\n",name , #expected, #actual, (expected), (actual)), 0))

#define RUN_TEST(function) do { \
        puts(CYAN("RUNNING TEST: " #function)); \
        int total = 0; \
        int passed = function(&total); \
        printf(CYAN(#function " PASSED [%d/%d] TESTS.\n\n"), passed, total); \
    } while (0)
#endif
