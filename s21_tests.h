/**
 * @file: test_s21_string.h
 * @brief Header file for unit testing the s21_string library.
 *
 * Description:
 *   This header file defines declarations related to unit testing of functions
 *   from s21_string.h and related modules. It includes necessary headers from
 *   the Check testing framework and specific header files for the functions
 *   being tested.
 *
 * Purpose:
 *   This header file is intended to be included in test files
 *   where unit tests for the s21_string library are defined. It ensures that
 * necessary dependencies and declarations are present to execute tests using
 * the Check framework. Each test suite focuses on different categories of
 * functions, aiming to verify their correctness and robustness under various
 * input conditions and edge cases.
 */
#ifndef S21_STRING_TESTS_H
#define S21_STRING_TESTS_H
#include <check.h>
#include <string.h>

#include "s21_sprintf.h"
#include "s21_sscanf.h"
#include "s21_string.h"

#define BUFFERSIZE 1024
#define EPS 0.000001f

Suite *s21_string_suite(void);
Suite *s21_suite_sprintf(void);
Suite *s21_sscanf_test(void);
#endif  // S21_STRING_TESTS_H
        /*
         *   Test Suite Functions:
         *     - Suite *s21_string_suite(void): Declaration for the main test suite of
         *       s21_string.h functions, covering a comprehensive set of tests for copy,
         *       search, transformation, utility, comparison, and calculation functions.
         *     - Suite *s21_suite_sprintf(void): Declaration for a specific test suite,
         *       if included, for functions related to formatted output (sprintf).
         *     - Suite *s21_sscanf_test(void): Declaration for a specific test suite,
         *       if included, for functions related to formatted input (sscanf).
         */