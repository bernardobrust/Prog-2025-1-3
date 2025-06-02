// * A simple test framework (astf) for C

/*
BSD 3-Clause License

Copyright (c) 2025, Bernardo Brust

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASTF_MAX_TESTS 1000
#define ASTF_MAX_MESSAGE_SIZE 256

// Colors
#define astf_output_fail "\x1b[31m"
#define astf_output_pass "\x1b[32m"
#define astf_output_warn "\x1b[33m"
#define astf_output_info "\x1b[36m"
#define astf_output_normal "\x1b[0m"

#define ASTF_PASSED astf_output_pass "Test passed" astf_output_normal

// Test counters
static int astf_tests_to_run = {0};
static int astf_tests_finished = {0};
static int astf_tests_failed = {0};
static int astf_tests_passed = {0};

// * Test list
// ---------------------------------------------------------------
typedef struct astf_test_list_t astf_test_list_t, *add_test, *get_test_message;
struct astf_test_list_t {
  bool exists; // If the list already has the function pointers attached
  int size;    // Number of tests
  int test_id[ASTF_MAX_TESTS];        // Test id's
  char *test_message[ASTF_MAX_TESTS]; // Messages (ASTF_PASSED or the error
                                      // message)
  char name[ASTF_MAX_MESSAGE_SIZE];   // Test suite name

  void (*add_test)(astf_test_list_t *list, char *message);
  char *(*get_test_message)(astf_test_list_t *list, const int test_id);
};

/**
 * @brief Internal usage, adds a test  to the list
 *
 * @param list The current test suite
 * @param message ASTF_PASSED or the error message
 */
static inline void astf_test_list_add_test(astf_test_list_t *list,
                                           char *message) {
  list->test_id[list->size] = list->size;
  list->test_message[list->size] = (char *)malloc(strlen(message) + 1);
  strcpy(list->test_message[list->size], message);
  list->size++;
}
/**
 * @brief Internal usage, gets the message from the test
 *
 * @param list The current test suite
 * @param test_id The number of the test in the suite
 * @return char* The message of given test
 */
static inline char *astf_test_list_get_test_message(astf_test_list_t *list,
                                                    const int test_id) {
  return list->test_message[test_id];
}

static astf_test_list_t astf_test_list = {};

/**
 * @brief Internal usage, resets the test list after the test suite ends
 *
 * @param list The current test suite
 */
static inline void astf_reset_test_list(astf_test_list_t *list) {
  // Clearing the ids
  memset(astf_test_list.test_id, 0, (unsigned long)astf_test_list.size);

  // Freeing messages
  for (int i = 0; i < astf_test_list.size; i++) {
    // ! Never forget freedom
    free(astf_test_list.test_message[i]);
  }

  // Reseting size
  list->size = 0;

  // Reseting the counters
  astf_tests_to_run = 0;
  astf_tests_finished = 0;
  astf_tests_failed = 0;
  astf_tests_passed = 0;

  // Reseting the suite name
  memset(astf_test_list.name, 0, ASTF_MAX_MESSAGE_SIZE);
}

/**
 * @brief Internal usage, Initialises the test suite
 *
 * @param suite_name Name of the test suite
 */
inline static void astf_init_test_list(const char *suite_name) {
  if (!astf_test_list.exists) {
    astf_test_list.add_test = astf_test_list_add_test;
    astf_test_list.get_test_message = astf_test_list_get_test_message;
    astf_test_list.exists = true;
  } else {
    astf_reset_test_list(&astf_test_list);
  }
  memcpy(astf_test_list.name, suite_name, strlen(suite_name) + 1);
}

/**
 * @brief Start a new test suite
 *
 * @param suite_name Name of the test suite
 */
static inline void astf_start_test_suite(const char *suite_name) {
  astf_init_test_list(suite_name);
  printf("\n");
  printf(astf_output_info "Running tests...");
  printf("\n");
}

/**
 * @brief Finishes the current test suite and prints the results
 *
 */
static inline void astf_retrieve_results() {
  printf(astf_output_info "Running tests from suite '%s'\n",
         astf_test_list.name);
  printf(astf_output_info "Finished running %d tests\n", astf_tests_finished);

  // Print amount of tests passed and failed
  if (astf_tests_passed == astf_tests_to_run) {
    printf(astf_output_pass "All tests passed!\n");
  } else if (astf_tests_failed == astf_tests_to_run) {
    printf(astf_output_fail "Literally everything went wrong.\n");
    printf(astf_output_fail "Like, every single test failed.\n");
    printf("\n");

    for (int i = 0; i < astf_test_list.size; i++) {
      char *message = astf_test_list.get_test_message(&astf_test_list, i);
      printf(astf_output_fail "|> %s\n", message);
    }
  } else {
    printf(astf_output_pass "|> %d tests passed!\n", astf_tests_passed);
    printf(astf_output_fail "|> %d tests failed ;-;\n", astf_tests_failed);
    printf("\n");

    for (int i = 0; i < astf_test_list.size; i++) {
      char *message = astf_test_list.get_test_message(&astf_test_list, i);

      if (strcmp(message, ASTF_PASSED) != 0) {
        printf(astf_output_fail "|> %s\n", message);
      }
    }
  }
  printf(astf_output_normal "\n");

  // Reseting the test list
  astf_reset_test_list(&astf_test_list);
}

// ? 2 variable tests
// ---------------------------------------------------------------
// ? Equality test
/**
 * @brief Asserts two ints are equal
 *
 * @param a Int produced
 * @param b Expected result
 */
static inline void astf_AE_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a == b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%d' and '%d' are different",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts two floats are equal
 *
 * @param a Float produced
 * @param b Expected result
 */
static inline void astf_AE_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (fabsf(a - b) < 0.0001f) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%f' and '%f' are different",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts two strings are equal
 *
 * @param a String produced
 * @param b Expected result
 */
static inline void astf_AE_string(const char *a, const char *b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (strcmp(a, b) == 0) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%s' and '%s' are different",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
// ---------------------------------------------------------------

// ? Inequality test
// ---------------------------------------------------------------
/**
 * @brief Asserts two int are different
 *
 * @param a Int produced
 * @param b Expected result
 */
static inline void astf_ANE_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a != b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%d' and '%d' are equal",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts two floats are different
 *
 * @param a Float produced
 * @param b Expected result
 */
static inline void astf_ANE_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (fabsf(a - b) > 0.0001f) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%f' and '%f' are equal",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts two strings are different
 *
 * @param a String produced
 * @param b Expected result
 */
static inline void astf_ANE_string(const char *a, const char *b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (strcmp(a, b) != 0) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%s' and '%s' are equal",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
// ---------------------------------------------------------------

// ? Comparaison tests
// ---------------------------------------------------------------
/**
 * @brief Asserts int a is grater than int b
 *
 * @param a Int expected to be bigger
 * @param b Int expected to be smaller or equal
 */
static inline void astf_AG_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a > b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;
  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail
             "Test %d failed, '%d' is smaller than or equal to '%d'",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts float a is grater than float b
 *
 * @param a Float expected to be bigger
 * @param b Float expected to be smaller or equal
 */
static inline void astf_AG_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a > b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail
             "Test %d failed, '%f' is smaller than or equal to '%f'",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts int a is grater than or equal to int b
 *
 * @param a Int expected to be bigger or equal
 * @param b Int expected to be smaller
 */
static inline void astf_AGE_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a >= b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;
  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%d' is smaller than '%d'",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts float a is grater than or equal to float b
 *
 * @param a Float expected to be bigger or equal
 * @param b Float expected to be smaller
 */
static inline void astf_AGE_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a >= b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;
  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%f' is smaller than '%f'",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts int a is lesser than int b
 *
 * @param a Int expected to be smaller
 * @param b Int expected to be bigger or equal
 */
static inline void astf_AL_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a < b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }
  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail
             "Test %d failed, '%d' is greater than or equal to '%d'",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts float a is lesser than float b
 *
 * @param a Float expected to be smaller
 * @param b Float expected to be bigger or equal
 */
static inline void astf_AL_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a < b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail
             "Test %d failed, '%f' is greater than or equal to '%f'",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts int a is lesser than or equal to int b
 *
 * @param a Int expected to be smaller or equal
 * @param b Int expected to be bigger
 */
static inline void astf_ALE_int(const int a, const int b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a <= b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%d' is greater than '%d'",
             astf_tests_finished, a, b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts float a is lesser than or equal to float b
 *
 * @param a Float expected to be smaller or equal
 * @param b Float expected to be bigger
 */
static inline void astf_ALE_float(const float a, const float b) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a <= b) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, '%f' is greater '%f'",
             astf_tests_finished, (double)a, (double)b);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
// ---------------------------------------------------------------

// ? 1 variable tests
// ---------------------------------------------------------------
// ? Truth test
/**
 * @brief Asserts a condition is true
 *
 * @param a The condition
 */
static inline void astf_AT(const bool a) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len, astf_output_fail "Test %d failed, value is false",
             astf_tests_finished);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts a condition is false
 *
 * @param a The condition
 */
static inline void astf_AF(const bool a) {
  bool failed = {false};

  astf_tests_to_run++;

  if (!a) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len, astf_output_fail "Test %d failed, value is true",
             astf_tests_finished);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}

// ? Null test
/**
 * @brief Asserts a pointer is null
 *
 * @param a The pointer
 */
static inline void astf_A_null(const void *a) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a == NULL) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, pinter '%p' is not null",
             astf_tests_finished, a);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
/**
 * @brief Asserts a pointer is not null
 *
 * @param a The pointer
 */
static inline void astf_AN_null(const void *a) {
  bool failed = {false};

  astf_tests_to_run++;

  if (a != NULL) {
    astf_tests_passed++;
  } else {
    astf_tests_failed++;
    failed = true;
  }

  astf_tests_finished++;

  if (failed) {
    char buffer[ASTF_MAX_MESSAGE_SIZE];
    unsigned long max_len = sizeof(buffer);
    snprintf(buffer, max_len,
             astf_output_fail "Test %d failed, pointer is null",
             astf_tests_finished);
    astf_test_list.add_test(&astf_test_list, buffer);
  } else {
    astf_test_list.add_test(&astf_test_list, ASTF_PASSED);
  }
}
// ---------------------------------------------------------------
