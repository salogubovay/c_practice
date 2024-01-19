#include "reg_testing.h"

static void read_int(int *pn) {
    int res = scanf("%d", pn);
    if ((res != 1) || (*pn <= 0)) {
        printf("Incorrect integer value\n");
        abort();
    }
}

static void read_test_info(struct Test *test) {
    FILE *testFile, *resultFile;
    check_file(test->pathToTestFile);
    check_file(test->pathToResultFile);
    testFile = fopen(test->pathToTestFile, "r");
    read_input_to_str(&test->input, testFile);
    fclose(testFile);
    resultFile = fopen(test->pathToResultFile, "r");
    read_input_to_str(&test->expectedOutput, resultFile);
    fclose(resultFile);
}

static void init_test(struct Test *test, const char *path, char *fileName) {
    test->pathToTestFile = generate_empty_string();
    test->pathToResultFile = generate_empty_string();
    test->fileName = fileName;
    test->input = generate_empty_string();
    test->expectedOutput = generate_empty_string();
    test->actualOutput = generate_empty_string();
    test->success = true;
    append_string(&test->pathToTestFile, path);
    append_string(&test->pathToTestFile, TEST_DIR);
    append_string(&test->pathToTestFile, fileName);
    append_string(&test->pathToResultFile, path);
    append_string(&test->pathToResultFile, RES_DIR);
    append_string(&test->pathToResultFile, fileName);
}

static void free_test(struct Test *test) {
    free(test->pathToTestFile);
    free(test->pathToResultFile);
    free(test->fileName);
    free(test->input);
    free(test->expectedOutput);
    free(test->actualOutput);
}

void read_regression_info(struct RegTesting *rt) {
    char *temp;
    rt->path = read_line(stdin); 
    rt->execFileName = read_line(stdin);
    read_int(&rt->testsCount);
    rt->tests = (struct Test *) calloc(rt->testsCount, sizeof(struct Test));
    rt->successCount = 0;
    temp = read_line(stdin);
    for (int i = 0; i < rt->testsCount; ++i) {
        init_test(&rt->tests[i], rt->path, read_line(stdin));
        read_test_info(&rt->tests[i]); 
    }
    free(temp);
}

void free_regression(struct RegTesting *rt) {
    free(rt->path);
    free(rt->execFileName);
    for (int i = 0; i < rt->testsCount; ++i) {
        free_test(&rt->tests[i]);
    }
    free(rt->tests);
}

static bool run_test(struct Test *test, const char* execFilePath) {
    FILE *testOutput;
    char *command;
    command = generate_empty_string();
    append_string(&command, execFilePath);
    append_string(&command, IREDIR);
    append_string(&command, test->pathToTestFile); 
    testOutput = popen(command, "r");
    read_input_to_str(&test->actualOutput, testOutput);
    pclose(testOutput);
    if (strcmp(test->expectedOutput, test->actualOutput) != 0) {
        test->success = false;
    }
    free(command);
    return test->success;
}

void run_all_tests(struct RegTesting *rt) {
    char *execFilePath = generate_empty_string();
    append_string(&execFilePath, rt->path);
    append_string(&execFilePath, rt->execFileName);
    for (int i = 0; i < rt->testsCount; ++i) {
        if (run_test(&rt->tests[i], execFilePath)) {
            rt->successCount++;
        }
    }
    free(execFilePath);
}

static void print_test_result(const struct Test *test) {
    printf("<Test file:> '%s'\n", test->pathToTestFile);
    printf("<Input:>\n%s\n", test->input);
    printf("<Actual output:>\n%s\n", test->actualOutput);
    printf("<Expected output:>\n%s\n", test->expectedOutput);
    printf("%s\n", SHORT_LINE);
    if (test->success) {
        printf("Result: SUCCESS|\n");
    } else {
        printf("Result: FAIL   |\n");
    }
    printf("%s\n", LONG_LINE);
}

void print_regression_results(const struct RegTesting *rt) {
    int failedCount;
    failedCount = rt->testsCount - rt->successCount;
    printf("%s\n", LONG_LINE);
    printf("REGRESSION TESTING REPORT\n"); 
    printf("%s\n", LONG_LINE);
    for (int i = 0; i < rt->testsCount; ++i) {
        printf("Test %2d        |\n", i + 1); 
        printf("%s\n", SHORT_LINE);
        print_test_result(&rt->tests[i]);
    }
    printf("Total Tests SUCCEEDED: %d FAILED: %d\n", rt->successCount, failedCount); 
    printf("%s\n", LONG_LINE);
}
