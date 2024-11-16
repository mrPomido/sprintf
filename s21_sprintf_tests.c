/**
 * @file:  test_s21_sprintf.c
 * @brief Unit tests for s21_sprintf function.
 *
 * Description:
 *   This file tests the implementation of s21_sprintf, covering formatting
 * accuracy for various data types and edge cases. Tests include basic
 * formatting, precision handling, string manipulation, and buffer overflow
 * scenarios.
 *
 *   Test cases ensure s21_sprintf functions correctly under diverse inputs,
 *   including large inputs, precision limits, and specific formatting
 * directives. The Check testing framework provides detailed failure diagnostics
 * to aid in identifying and resolving issues.
 *
 */
#include "s21_tests.h"

START_TEST(width_char) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *formats[] = {
      "This is a simple wide char %5c",
      "This is a simple wide char %-5c",
      "This is a simple wide char %.2c",
      "This is a simple wide char %10c",
  };
  char characters[] = {'c', 'A', '$', '\n'};
  int num_formats = sizeof(formats) / sizeof(formats[0]);
  int num_chars = sizeof(characters) / sizeof(characters[0]);
  for (int i = 0; i < num_formats; ++i) {
    for (int j = 0; j < num_chars; ++j) {
      char w = characters[j];
      char *format = formats[i];

      int a = s21_sprintf(str1, format, w);
      int b = sprintf(str2, format, w);

      ck_assert_str_eq(str1, str2);
      ck_assert_int_eq(a, b);
    }
  }
}
END_TEST

START_TEST(sprintf_NULL_wchar) {
  char str1[100];
  char str2[100];
  char *format = "%lc";
  wchar_t *stringLong = s21_NULL;
  s21_sprintf(str1, format, stringLong);
  sprintf(str2, format, stringLong);
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(sprintf_wstr) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  wchar_t s = 0x77;
  wchar_t str[10] = {0};
  str[0] = s;
  str[1] = s;
  char *format = "%-10ls";
  s21_sprintf(str1, format, str);
  sprintf(str2, format, str);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_wchar) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%3lc";
  wchar_t longChar = 0x77;
  s21_sprintf(str1, format, longChar);
  sprintf(str2, format, longChar);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_string) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "b%*sbr";
  char str[] = "ooooooo";
  s21_sprintf(str1, format, 5, str);
  sprintf(str2, format, 5, str);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_char) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "b%*sbr";
  char c = 'c';
  s21_sprintf(str1, format, c);
  sprintf(str2, format, c);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(sprintf_char_spec_both) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "This is %4c simple %-1s in %2c %*s";
  char c = 'a';
  char str[] = "bobr";
  s21_sprintf(str1, format, c, str, c, 9, str);
  sprintf(str2, format, c, str, c, 9, str);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(d_256) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  char format[] = "Decimal %hd, %d, %ld of different sizes.";
  short var1 = 0;
  int var2 = 0;
  long int var3 = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2, var3),
                   sprintf(str2, format, var1, var2, var3));
  ck_assert_str_eq(str1, str2);
  var1 = (short)INT32_MAX;
  var2 = INT32_MAX;
  var3 = INT64_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2, var3),
                   sprintf(str2, format, var1, var2, var3));
  ck_assert_str_eq(str1, str2);
  var1 = (short)INT32_MIN;
  var2 = INT32_MIN;
  var3 = INT64_MIN;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2, var3),
                   sprintf(str2, format, var1, var2, var3));
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(s21_sprintf(str1, format, var3, var3, var3),
                   sprintf(str2, format, var3, var3, var3));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(d_flags) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  int var = 21;
  char *formats[] = {"Let's test flag combinations %+-0 40d.",
                     "Let's test flag combinations %+-040d.",
                     "Let's test flag combinations %+040d.",
                     "Let's test flag combinations %040d.",
                     "Let's test flag combinations %-40d.",
                     "Let's test flag combinations %+d."};
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    ck_assert_int_eq(s21_sprintf(str1, formats[i], var),
                     sprintf(str2, formats[i], var));
    ck_assert_str_eq(str1, str2);
  }
}
END_TEST

START_TEST(d_width) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  int var = 42;
  char *formats[] = {"Min field width %20d", "Min field width %*d",
                     "Min field width %+*d", "Min field width % *d",
                     "Min field width %-*d", "Min field width %0*d",
                     "Min field width %10d", "Min field width %-10d"};
  int widths[] = {0, 20, 20, 20, 10, 10, 0, 0};
  int vars[] = {var, var, var, var, var, var, -3, -3};
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    if (widths[i] == 0) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], vars[i]),
                       sprintf(str2, formats[i], vars[i]));
      ck_assert_str_eq(str1, str2);
    } else {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], widths[i], vars[i]),
                       sprintf(str2, formats[i], widths[i], vars[i]));
      ck_assert_str_eq(str1, str2);
    }
  }
}
END_TEST

START_TEST(d_precision) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  int var = 42;
  char *formats[] = {"Integer precision %.20d", "Integer precision %.*d",
                     "Integer precision %.*d",  "Integer precision %.*d",
                     "Integer precision %.*d",  "Integer precision %0.*d",
                     "Integer precision %+.*d", "Integer precision % .*d",
                     "Integer precision %.10d"};
  int precisions[] = {0, 20, 0, -20, 0, 10, 10, 10, 0};
  int vars[] = {var, var, var, var, 0, 0, 0, 0, -3};
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    if (precisions[i] == 0) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], vars[i]),
                       sprintf(str2, formats[i], vars[i]));
      ck_assert_str_eq(str1, str2);
    } else {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], precisions[i], vars[i]),
                       sprintf(str2, formats[i], precisions[i], vars[i]));
      ck_assert_str_eq(str1, str2);
    }
  }
}
END_TEST

START_TEST(ld_hd_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%+0.*ld";
  long val1 = 999999999999;
  ck_assert_int_eq(s21_sprintf(str1, format, 10, val1),
                   sprintf(str2, format, 10, val1));
  ck_assert_str_eq(str1, str2);
  format = "%010hd";
  short val2 = -999;
  ck_assert_int_eq(s21_sprintf(str1, format, 10, val2),
                   sprintf(str2, format, 10, val2));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(i_random) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Integer precicion %.20i";
  int var = 42;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Here is long %+0.*li (same as d)";
  long var1 = 999999999999;
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var1),
                   sprintf(str2, format, 10, var1));
  ck_assert_str_eq(str1, str2);

  format = "Everythind at once %+-*.*i, %i";
  ck_assert_int_eq(s21_sprintf(str1, format, 20, 10, var, var),
                   sprintf(str2, format, 20, 10, var, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(u_256) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  char *format = "%lu, %u, %hu";
  long unsigned vals1[] = {ULONG_MAX, ULONG_MAX, ULONG_MAX, USHRT_MAX, 0};
  unsigned vals2[] = {UINT_MAX, UINT_MAX, USHRT_MAX, USHRT_MAX, 0};
  short unsigned vals3[] = {USHRT_MAX, USHRT_MAX, USHRT_MAX, USHRT_MAX, 0};
  for (size_t i = 0; i < sizeof(vals1) / sizeof(vals1[0]); i++) {
    ck_assert_int_eq(s21_sprintf(str1, format, vals1[i], vals2[i], vals3[i]),
                     sprintf(str2, format, vals1[i], vals2[i], vals3[i]));
    ck_assert_str_eq(str1, str2);
  }
}
END_TEST

START_TEST(u_flags) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  char *formats[] = {"Flags %+u unsigned.",      "Flags % u unsigned.",
                     "Flags %+-0 40u unsigned.", "Flags %+-040u unsigned.",
                     "Flags %+040u unsigned.",   "Flags %040u unsigned.",
                     "Flags %-40u unsigned."};
  unsigned var = 32;
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    ck_assert_int_eq(s21_sprintf(str1, formats[i], var),
                     sprintf(str2, formats[i], var));
    ck_assert_str_eq(str1, str2);
  }
}
END_TEST

START_TEST(u_width) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  char *formats[] = {"Min field width %20u", "Min field width %*u",
                     "Min field width %+*u", "Min field width % *u",
                     "Min field width %-*u", "Min field width %0*u"};
  unsigned var = 42;
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    if (i == 1 || i == 2 || i == 3) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], 20, var),
                       sprintf(str2, formats[i], 20, var));
    } else if (i == 4) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], 10, var),
                       sprintf(str2, formats[i], 10, var));
    } else {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], 10, var),
                       sprintf(str2, formats[i], 10, var));
    }
    ck_assert_str_eq(str1, str2);
  }
}
END_TEST

START_TEST(u_precision) {
  char str1[BUFFERSIZE], str2[BUFFERSIZE];
  char *formats[] = {"Unsigned precision %.20u", "Unsigned precision %.*u",
                     "Unsigned precision %0.*u", "Unsigned precision %+.*u",
                     "Unsigned precision % .*u", "Unsigned precision %*.*u"};
  unsigned var = 42;
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); i++) {
    if (i == 1 || i == 2 || i == 3 || i == 4) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], 10, var),
                       sprintf(str2, formats[i], 10, var));
    } else if (i == 5) {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], 20, 10, var),
                       sprintf(str2, formats[i], 20, 10, var));
    } else {
      ck_assert_int_eq(s21_sprintf(str1, formats[i], var),
                       sprintf(str2, formats[i], var));
    }
    ck_assert_str_eq(str1, str2);
  }
}
END_TEST

START_TEST(o_256) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%lo, %o, %ho";
  long int val1 = LONG_MAX;
  int val2 = INT_MAX;
  short int val3 = SHRT_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val1, val1),
                   sprintf(str2, format, val1, val1, val1));
  ck_assert_str_eq(str1, str2);
  val1 = LONG_MIN;
  val2 = INT_MIN;
  val3 = SHRT_MIN;
  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val1, val1),
                   sprintf(str2, format, val1, val1, val1));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, val3, val3, val3),
                   sprintf(str2, format, val3, val3, val3));
  ck_assert_str_eq(str1, str2);
  val1 = 0;
  val2 = 0;
  val3 = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);
  long unsigned uval1 = ULONG_MAX;
  unsigned uval2 = UINT_MAX;
  short unsigned uval3 = USHRT_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, uval1, uval2, uval3),
                   sprintf(str2, format, uval1, uval2, uval3));
  ck_assert_str_eq(str1, str2);
  ck_assert_int_eq(s21_sprintf(str1, format, uval1, uval1, uval1),
                   sprintf(str2, format, uval1, uval1, uval1));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(o_width_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  int var = 0;
  // int var = INT_MAX;
  int widths[] = {-1, 10, 0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
  char *formats[] = {//"Min field width %20o" (20),
                     "Min field width %-o",    "Min field width %*o",
                     "Min field width %*o",    "Min field width %+*o",
                     "Min field width % *o",   "Min field width %+#*o",
                     "Min field width % #*o",  "Min field width %- #*o",
                     "Min field width %- #0o", "Min field width %*o",
                     "Min field width %-*o",   "Min field width %0*o",
                     "Min field width %0#*o",  "Min field width %+#*o",
                     "Min field width % #*o",  "Min field width %- #*o"};
  for (size_t i = 0; i < sizeof(formats) / sizeof(formats[0]); ++i) {
    int ret_s21 = s21_sprintf(str1, formats[i], var);
    int ret_sprintf = sprintf(str2, formats[i], var);
    ck_assert_msg(
        ret_s21 == ret_sprintf,
        "Format: %s, s21_sprintf return value: %d, sprintf return value: %d",
        formats[i], ret_s21, ret_sprintf);
    ck_assert_msg(strcmp(str1, str2) == 0,
                  "Format: %s, s21_sprintf output: %s, sprintf output: %s",
                  formats[i], str1, str2);
  }
  for (size_t i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
    int ret_s21 = s21_sprintf(str1, "Min field width %*o", widths[i], var);
    int ret_sprintf = sprintf(str2, "Min field width %*o", widths[i], var);
    ck_assert_msg(
        ret_s21 == ret_sprintf,
        "Width: %d, s21_sprintf return value: %d, sprintf return value: %d",
        widths[i], ret_s21, ret_sprintf);
    ck_assert_msg(strcmp(str1, str2) == 0,
                  "Width: %d, s21_sprintf output: %s, sprintf output: %s",
                  widths[i], str1, str2);
  }
}
END_TEST

START_TEST(o_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  unsigned var = 42;
  int widths[] = {20, -1, 10, 0, 11, 12, 13, 14, 15};
  int precisions[] = {20, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
  for (size_t i = 0; i < sizeof(widths) / sizeof(widths[0]); ++i) {
    for (size_t j = 0; j < sizeof(precisions) / sizeof(precisions[0]); ++j) {
      int ret_s21 = s21_sprintf(str1, "Octal precision %*.*o", widths[i],
                                precisions[j], var);
      int ret_sprintf =
          sprintf(str2, "Octal precision %*.*o", widths[i], precisions[j], var);
      ck_assert_msg(ret_s21 == ret_sprintf,
                    "Width: %d, Precision: %d, s21_sprintf return value: %d, "
                    "sprintf return value: %d",
                    widths[i], precisions[j], ret_s21, ret_sprintf);
      ck_assert_msg(strcmp(str1, str2) == 0,
                    "Width: %d, Precision: %d, s21_sprintf output: %s, sprintf "
                    "output: %s",
                    widths[i], precisions[j], str1, str2);
    }
  }
}
END_TEST

START_TEST(x_256) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%lx, %x, %hx";
  long int val1 = LONG_MAX;
  int val2 = INT_MAX;
  short int val3 = SHRT_MAX;

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);

  format = "%lX, %X, %hx";

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val1, val1),
                   sprintf(str2, format, val1, val1, val1));
  ck_assert_str_eq(str1, str2);

  val1 = LONG_MIN;
  val2 = INT_MIN;
  val3 = SHRT_MIN;

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, val1, val1, val1),
                   sprintf(str2, format, val1, val1, val1));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, val3, val3, val3),
                   sprintf(str2, format, val3, val3, val3));
  ck_assert_str_eq(str1, str2);

  val1 = 0;
  val2 = 0;
  val3 = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, val1, val2, val3),
                   sprintf(str2, format, val1, val2, val3));
  ck_assert_str_eq(str1, str2);

  long unsigned uval1 = ULONG_MAX;
  unsigned uval2 = UINT_MAX;
  short unsigned uval3 = USHRT_MAX;

  ck_assert_int_eq(s21_sprintf(str1, format, uval1, uval2, uval3),
                   sprintf(str2, format, uval1, uval2, uval3));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, uval1, uval1, uval1),
                   sprintf(str2, format, uval1, uval1, uval1));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(x_width_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Min field width %20x";
  int var = INT_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %-x";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var),
                   sprintf(str2, format, 10, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 11, var),
                   sprintf(str2, format, 11, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width % *X";
  ck_assert_int_eq(s21_sprintf(str1, format, 12, var),
                   sprintf(str2, format, 12, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+#*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 13, var),
                   sprintf(str2, format, 13, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width % #*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 14, var),
                   sprintf(str2, format, 14, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %- #*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 15, var),
                   sprintf(str2, format, 15, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %- #0X";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = 0;

  format = "Min field width %*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %-*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 16, var),
                   sprintf(str2, format, 16, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %0*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 17, var),
                   sprintf(str2, format, 17, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %0#*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+#*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 19, var),
                   sprintf(str2, format, 19, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width % #*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 20, var),
                   sprintf(str2, format, 20, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %- #*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 21, var),
                   sprintf(str2, format, 21, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(x_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Hexadecimal precicion %.20x";
  unsigned var = 42;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %.*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 20, var),
                   sprintf(str2, format, 20, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %0.*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %#.*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %.*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var),
                   sprintf(str2, format, 10, var));
  ck_assert_str_eq(str1, str2);

  var = UINT_MAX;
  format = "Hexadecimal precicion %0.*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 11, var),
                   sprintf(str2, format, 11, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %+.*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 12, var),
                   sprintf(str2, format, 12, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion % .*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 13, var),
                   sprintf(str2, format, 13, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %# .*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 14, var),
                   sprintf(str2, format, 14, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %#+.*X";
  ck_assert_int_eq(s21_sprintf(str1, format, 15, var),
                   sprintf(str2, format, 15, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %*.*x";
  ck_assert_int_eq(s21_sprintf(str1, format, 20, 10, var),
                   sprintf(str2, format, 20, 10, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %20.10x";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Hexadecimal precicion %-#20.10X";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_256) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%e, %Le";
  double var1 = 0;
  long double var2 = 0;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MAX;
  var2 = LDBL_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MAX * 1e20;
  var2 = LDBL_MAX * 1e20;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MIN / 2;
  var2 = LDBL_MIN / 2;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, var2, var1),
                   sprintf(str2, format, var2, var1));
  ck_assert_str_eq(str1, str2);

  var1 = 1 / 3;
  var2 = 1 / 6;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_inf) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%E, %Le";
  double var1 = DBL_MAX * 1e20;
  long double var2 = LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%+e, %+LE";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "% E, % Le";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%e, %LE";
  var1 = -DBL_MAX * 1e20;
  var2 = -LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = NAN;
  var2 = NAN;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "% 50E";
  double var = DBL_MAX / 3;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+40e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%040E";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%-40e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+-40E";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%e% -40E";
  ck_assert_int_eq(s21_sprintf(str1, format, var, var),
                   sprintf(str2, format, var, var));
  ck_assert_str_eq(str1, str2);

  format = "%-040e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+ 40e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "% -+040e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%#+-40E";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Min field width %40e";
  double var = -DBL_MIN;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %*E";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, -40, var),
                   sprintf(str2, format, -40, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %-*E";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+-*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 41, var),
                   sprintf(str2, format, 41, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %0*E";
  ck_assert_int_eq(s21_sprintf(str1, format, 42, var),
                   sprintf(str2, format, 42, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(e_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%.18e";
  double var = 42.1;
  long double lvar = 4;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "%#.*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var),
                   sprintf(str2, format, 10, var));
  ck_assert_str_eq(str1, str2);

  format = "%0.*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 11, var),
                   sprintf(str2, format, 11, var));
  ck_assert_str_eq(str1, str2);

  format = "%+.*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 12, var),
                   sprintf(str2, format, 12, var));
  ck_assert_str_eq(str1, str2);

  format = "% .*e";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  var = -3;
  format = "%.10e";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = 15.35;
  format = "%.17Le";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.0Le";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.Le";
  lvar = 12.000002121;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.15e";
  var = 0.000000000000000111;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.17Le";
  lvar = -14.4224422424422449999;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(f_256) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%f, %Lf";
  double var1 = 0;
  long double var2 = 0;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = 1 / 3;
  var2 = 1 / 6;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%Lf";
  var2 = 513515.131513515151351;
  ck_assert_int_eq(s21_sprintf(str1, format, var2),
                   sprintf(str2, format, var2));
  ck_assert_str_eq(str1, str2);

  format = "%10Lf";
  var2 = 15.35;
  ck_assert_int_eq(s21_sprintf(str1, format, var2),
                   sprintf(str2, format, var2));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, "%f", 0.0001),
                   sprintf(str2, "%f", 0.0001));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(f_inf) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%f, %Lf";
  double var1 = DBL_MAX * 1e20;
  long double var2 = LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%+f, %+Lf";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "% f, % Lf";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%f, %Lf";
  var1 = -DBL_MAX * 1e20;
  var2 = -LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = NAN;
  var2 = NAN;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(f_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "% f";
  double var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "% 20f";
  var = 99.9999999;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+40f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%040f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = -99.9999999;
  format = "%-40f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+-40f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%f% -40f";
  ck_assert_int_eq(s21_sprintf(str1, format, -var, var),
                   sprintf(str2, format, -var, var));
  ck_assert_str_eq(str1, str2);

  format = "%-040f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+ 40f";
  ck_assert_int_eq(s21_sprintf(str1, format, -var),
                   sprintf(str2, format, -var));
  ck_assert_str_eq(str1, str2);

  format = "% -+040f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(f_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Min field width %40f";
  double var = -DBL_MIN;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, -40, var),
                   sprintf(str2, format, -40, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %-*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+-*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 41, var),
                   sprintf(str2, format, 41, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %0*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 42, var),
                   sprintf(str2, format, 42, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(f_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%.20f";
  double var = 42;
  long double lvar = 4;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, -10, var),
                   sprintf(str2, format, -10, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var),
                   sprintf(str2, format, 10, var));
  ck_assert_str_eq(str1, str2);

  format = "%0.*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 11, var),
                   sprintf(str2, format, 11, var));
  ck_assert_str_eq(str1, str2);

  format = "%+.*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 12, var),
                   sprintf(str2, format, 12, var));
  ck_assert_str_eq(str1, str2);

  format = "%#+.*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 13, var),
                   sprintf(str2, format, 13, var));
  ck_assert_str_eq(str1, str2);

  format = "% .*f";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  var = -3;
  format = "%.10f";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = 15.35;
  format = "%.17Lf";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.0Lf";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.Lf";
  lvar = 12.000002121;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.15f";
  var = 0.000000000000000111;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.17Lf";
  lvar = -14.4224422424422449999;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.10Lf";
  lvar = -99.99999999999999;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_256) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%g, %LG";
  double var1 = 0;
  long double var2 = 0;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MAX;
  var2 = LDBL_MAX;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MAX * 1e20;
  var2 = LDBL_MAX * 1e20;
  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = DBL_MIN / 2;
  var2 = LDBL_MIN / 2;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, var2, var1),
                   sprintf(str2, format, var2, var1));
  ck_assert_str_eq(str1, str2);

  var1 = 1 / 3;
  var2 = 1 / 6;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%LG";
  var2 = 0.000005;
  ck_assert_int_eq(s21_sprintf(str1, format, var2),
                   sprintf(str2, format, var2));
  ck_assert_str_eq(str1, str2);

  format = "%g";
  var1 = 0.005;
  ck_assert_int_eq(s21_sprintf(str1, format, var1),
                   sprintf(str2, format, var1));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_inf) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%G, %Lg";
  double var1 = DBL_MAX * 1e20;
  long double var2 = LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%+g, %+Lg";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "% G, % LG";

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  format = "%g, %Lg";
  var1 = -DBL_MAX * 1e20;
  var2 = -LDBL_MAX * 1e20;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);

  var1 = NAN;
  var2 = NAN;

  ck_assert_int_eq(s21_sprintf(str1, format, var1, var2),
                   sprintf(str2, format, var1, var2));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "% 50G";
  double var = DBL_MAX / 3;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+40g";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%040G";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%-40g";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+-40G";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%e% -40G";
  ck_assert_int_eq(s21_sprintf(str1, format, var, var),
                   sprintf(str2, format, var, var));
  ck_assert_str_eq(str1, str2);

  format = "%-040g";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%+ 40g";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "% -+040G";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%g";
  var = 0.0000005;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "Min field width %40g";
  double var = -DBL_MIN;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %*G";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, -40, var),
                   sprintf(str2, format, -40, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %-*g";
  ck_assert_int_eq(s21_sprintf(str1, format, 40, var),
                   sprintf(str2, format, 40, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %+-*G";
  ck_assert_int_eq(s21_sprintf(str1, format, 41, var),
                   sprintf(str2, format, 41, var));
  ck_assert_str_eq(str1, str2);

  format = "Min field width %0*g";
  ck_assert_int_eq(s21_sprintf(str1, format, 42, var),
                   sprintf(str2, format, 42, var));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  char *format = "%.18G";
  double var = 42.1;
  long double lvar = 4;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.5g";
  var = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.g";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = 0.0004;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = -0.0004;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*g";
  var = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  ck_assert_int_eq(s21_sprintf(str1, format, -18, var),
                   sprintf(str2, format, -18, var));
  ck_assert_str_eq(str1, str2);

  var = 0;
  ck_assert_int_eq(s21_sprintf(str1, format, 0, var),
                   sprintf(str2, format, 0, var));
  ck_assert_str_eq(str1, str2);

  format = "%.*G";
  ck_assert_int_eq(s21_sprintf(str1, format, 10, var),
                   sprintf(str2, format, 10, var));
  ck_assert_str_eq(str1, str2);

  format = "%0.*g";
  ck_assert_int_eq(s21_sprintf(str1, format, 11, var),
                   sprintf(str2, format, 11, var));
  ck_assert_str_eq(str1, str2);

  format = "%+.*G";
  ck_assert_int_eq(s21_sprintf(str1, format, 12, var),
                   sprintf(str2, format, 12, var));
  ck_assert_str_eq(str1, str2);

  format = "% .*g";
  ck_assert_int_eq(s21_sprintf(str1, format, 18, var),
                   sprintf(str2, format, 18, var));
  ck_assert_str_eq(str1, str2);

  var = -3;
  format = "%.10G";
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  var = 15.35;
  format = "%.17Lg";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.0LG";
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.Lg";
  lvar = 12.000002121;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);

  format = "%.15G";
  var = 0.000000000000000111;
  ck_assert_int_eq(s21_sprintf(str1, format, var), sprintf(str2, format, var));
  ck_assert_str_eq(str1, str2);

  format = "%.17Lg";
  lvar = -14.4224422424422449999;
  ck_assert_int_eq(s21_sprintf(str1, format, lvar),
                   sprintf(str2, format, lvar));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_trailing_zero) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%g";
  double hex = 0.50300;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_large) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%g";
  double hex = 5131.43141;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_small) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%g";
  double hex = 0.123000;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(g_mantiss_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%5.8g";
  double hex = 0.0000005;
  ck_assert_int_eq(s21_sprintf(str1, format, hex), sprintf(str2, format, hex));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(empty_format_and_parameters) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  ck_assert_int_eq(s21_sprintf(str1, "abc"), sprintf(str2, "abc"));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(n_spec) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%n";
  int n_smb1 = 0;
  int n_smb2 = 0;
  str1[0] = '\0';
  str2[0] = '\0';

  ck_assert_int_eq(s21_sprintf(str1, format, &n_smb1),
                   sprintf(str2, format, &n_smb2));
  ck_assert_int_eq(n_smb1, n_smb2);
  ck_assert_str_eq(str1, str2);

  format = "Once upon a midnight dreary, while I pondered, weak and weary%n";
  ck_assert_int_eq(s21_sprintf(str1, format, &n_smb1),
                   sprintf(str2, format, &n_smb2));
  ck_assert_int_eq(n_smb1, n_smb2);
  ck_assert_str_eq(str1, str2);

  format = "%dce upon a %02d:%02d dreary, while I pondered, weak and weary%n";
  ck_assert_int_eq(s21_sprintf(str1, format, 1, 0, 0, &n_smb1),
                   sprintf(str2, format, 1, 0, 0, &n_smb2));
  ck_assert_int_eq(n_smb1, n_smb2);
  ck_assert_str_eq(str1, str2);

  format = "%ln";
  long int ln_smb1 = 0;
  long int ln_smb2 = 0;

  ck_assert_int_eq(s21_sprintf(str1, format, &ln_smb1),
                   sprintf(str2, format, &ln_smb2));
  ck_assert_int_eq(n_smb1, n_smb2);
  ck_assert_str_eq(str1, str2);

  format = "%hn";
  short int hn_smb1 = 0;
  short int hn_smb2 = 0;

  ck_assert_int_eq(s21_sprintf(str1, format, &hn_smb1),
                   sprintf(str2, format, &hn_smb2));
  ck_assert_int_eq(n_smb1, n_smb2);
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(perc_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];

  ck_assert_int_eq(s21_sprintf(str1, "abc%%abc"), sprintf(str2, "abc%%abc"));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(char_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%.5c";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(char_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "% -5c";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);

  format = "%-5.3c";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);

  format = "%-5.3c";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(char_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%15c";
  char val = 'c';
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%s";
  char *val = "0nce upon a midnight dreary, while I pondered, weak and weary";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%.15s";
  char *val = "Over many a quaint and curious volume of forgotten lore --";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%15s";
  char *val = "While I nodded, nearly napping, suddenly there came a tapping,";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_flags) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%-15.9s";
  char *val = "As of some one gently rapping, rapping at my chamber door—";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(string_long) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%s";
  char *val =
      "<<'Tis some visitor,>> I muttered, << tapping at my chamber door —";
  ck_assert_int_eq(s21_sprintf(str1, format, val), sprintf(str2, format, val));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(ptr) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(ptr_width) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%15p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(ptr_precision) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char *format = "%.5p";
  ck_assert_int_eq(s21_sprintf(str1, format, format),
                   sprintf(str2, format, format));
  ck_assert_str_eq(str1, str2);
}
END_TEST
// нужен ли этот тест?
// START_TEST(null_ptr) {
//   char str1[BUFFERSIZE];
//   char str2[BUFFERSIZE];
//   char *format = "%p";
//   char *ptr = s21_NULL;
//   ck_assert_int_eq(s21_sprintf(str1, format, ptr), sprintf(str2, format,
//   ptr)); ck_assert_str_eq(str1, str2); ptr = NULL;
//   ck_assert_int_eq(s21_sprintf(str1, format, ptr), sprintf(str2, format,
//   ptr)); ck_assert_str_eq(str1, str2);
// }
// END_TEST

START_TEST(test_sprintf42) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%#5p";
  int a = 5;
  ck_assert_int_eq(s21_sprintf(str1, format, &a), sprintf(str2, format, &a));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_one_char) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  ck_assert_int_eq(s21_sprintf(str1, "%c", '\t'), sprintf(str2, "%c", '\t'));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_one_string) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  ck_assert_int_eq(s21_sprintf(str1, "%s", "Drop Sega PLS"),
                   sprintf(str2, "%s", "Drop Sega PLS"));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_one_char_with_alignment_left) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  ck_assert_int_eq(s21_sprintf(str1, "%8c", '\t'), sprintf(str2, "%8c", '\t'));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_one_char_with_alignment_right) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  ck_assert_int_eq(s21_sprintf(str1, "%-8c", '\t'),
                   sprintf(str2, "%-8c", '\t'));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf2) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%1.1f";
  ck_assert_int_eq(s21_sprintf(str1, format, 1.1), sprintf(str2, format, 1.1));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf3) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%8.3c";
  ck_assert_int_eq(s21_sprintf(str1, format, 'a'), sprintf(str2, format, 'a'));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf6) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%7.7f";
  ck_assert_int_eq(s21_sprintf(str1, format, 11.123456),
                   sprintf(str2, format, 11.123456));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf7) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%7.4s";
  ck_assert_int_eq(s21_sprintf(str1, format, "aboba floppa"),
                   sprintf(str2, format, "aboba floppa"));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf16) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "% c";
  ck_assert_int_eq(s21_sprintf(str1, format, 'a'), sprintf(str2, format, 'a'));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf17) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "% s";
  ck_assert_int_eq(s21_sprintf(str1, format, "aboba likes floppa"),
                   sprintf(str2, format, "aboba likes floppa"));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_sprintf25) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%.f";
  ck_assert_int_eq(s21_sprintf(str1, format, 121.123),
                   sprintf(str2, format, 121.123));
  ck_assert_str_eq(str1, str2);
}
END_TEST
START_TEST(test_sprintf29) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%-.f";
  ck_assert_int_eq(s21_sprintf(str1, format, 111.111),
                   sprintf(str2, format, 111.111));
  ck_assert_str_eq(str1, str2);
}
END_TEST

START_TEST(test_sprintf41) {
  char str1[BUFFERSIZE];
  char str2[BUFFERSIZE];
  char format[] = "%.Lf";
  long double a = 7.9418438184;
  ck_assert_int_eq(s21_sprintf(str1, format, a), sprintf(str2, format, a));
  ck_assert_str_eq(str1, str2);
}
END_TEST

Suite *s21_suite_sprintf(void) {
  Suite *s;
  TCase *tc;

  s = suite_create("s21_sprintf tests");
  tc = tcase_create("SPRINTF TESTS");

  tcase_add_test(tc, empty_format_and_parameters);

  tcase_add_test(tc, sprintf_NULL_wchar);
  tcase_add_test(tc, sprintf_wstr);
  tcase_add_test(tc, sprintf_wchar);
  tcase_add_test(tc, sprintf_string);
  tcase_add_test(tc, sprintf_char);
  tcase_add_test(tc, sprintf_char_spec_both);
  tcase_add_test(tc, width_char);

  tcase_add_test(tc, d_256);
  tcase_add_test(tc, d_flags);
  tcase_add_test(tc, d_width);
  tcase_add_test(tc, d_precision);
  tcase_add_test(tc, ld_hd_flags);

  tcase_add_test(tc, i_random);

  tcase_add_test(tc, u_256);
  tcase_add_test(tc, u_flags);
  tcase_add_test(tc, u_width);
  tcase_add_test(tc, u_precision);

  tcase_add_test(tc, o_256);
  tcase_add_test(tc, o_width_flags);
  tcase_add_test(tc, o_precision);

  tcase_add_test(tc, x_256);
  tcase_add_test(tc, x_width_flags);
  tcase_add_test(tc, x_precision);

  tcase_add_test(tc, e_256);
  tcase_add_test(tc, e_inf);
  tcase_add_test(tc, e_flags);
  tcase_add_test(tc, e_width);
  tcase_add_test(tc, e_precision);

  tcase_add_test(tc, f_256);
  tcase_add_test(tc, f_inf);
  tcase_add_test(tc, f_flags);
  tcase_add_test(tc, f_width);
  tcase_add_test(tc, f_precision);

  tcase_add_test(tc, g_256);
  tcase_add_test(tc, g_inf);
  tcase_add_test(tc, g_flags);
  tcase_add_test(tc, g_width);
  tcase_add_test(tc, g_precision);
  tcase_add_test(tc, g_trailing_zero);
  tcase_add_test(tc, g_large);
  tcase_add_test(tc, g_small);
  tcase_add_test(tc, g_mantiss_flags);

  tcase_add_test(tc, perc_width);

  tcase_add_test(tc, n_spec);

  tcase_add_test(tc, ptr);
  tcase_add_test(tc, ptr_width);
  tcase_add_test(tc, ptr_precision);
  // tcase_add_test(tc, null_ptr);
  tcase_add_test(tc, char_precision);
  tcase_add_test(tc, char_flags);
  tcase_add_test(tc, char_width);
  tcase_add_test(tc, string);
  tcase_add_test(tc, string_precision);
  tcase_add_test(tc, string_width);
  tcase_add_test(tc, string_flags);
  tcase_add_test(tc, string_long);
  tcase_add_test(tc, test_one_char);
  tcase_add_test(tc, test_one_string);
  tcase_add_test(tc, test_one_char_with_alignment_left);
  tcase_add_test(tc, test_one_char_with_alignment_right);
  tcase_add_test(tc, test_sprintf2);
  tcase_add_test(tc, test_sprintf3);
  tcase_add_test(tc, test_sprintf6);
  tcase_add_test(tc, test_sprintf7);
  tcase_add_test(tc, test_sprintf16);
  tcase_add_test(tc, test_sprintf17);
  tcase_add_test(tc, test_sprintf25);
  tcase_add_test(tc, test_sprintf29);
  tcase_add_test(tc, test_sprintf41);
  tcase_add_test(tc, test_sprintf42);
  suite_add_tcase(s, tc);
  return s;
}