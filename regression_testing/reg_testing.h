#pragma once
#include <sys/types.h>
#include <stdbool.h>
#include "../string/string_lib.h"

#define TEST_DIR "tests/"
#define RES_DIR "results/"
#define IREDIR " < "
#define LONG_LINE "-----------------------------------------------------------------------------------------"
#define SHORT_LINE "----------------"

struct Test {
    char *pathToTestFile;
    char *pathToResultFile;
    char *fileName;
    char *input;
    char *expectedOutput;
    char *actualOutput;
    bool success;
};

struct RegTesting {
    char *path;
    char *execFileName;
    int testsCount;
    struct Test *tests;
    int successCount;
};

void read_regression_info(struct RegTesting *rt);
void free_regression(struct RegTesting *rt);
void run_all_tests(struct RegTesting *rt);
void print_regression_results(const struct RegTesting *rt);
