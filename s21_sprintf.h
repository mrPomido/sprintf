/**
 * @file s21_sprintf.h
 * @brief Header file defining functions for formatted string output.
 *
 * This header file declares functions and types used for formatted string
 * output similar to the standard C library's sprintf family. It includes
 * necessary standard headers such as float.h, limits.h, stdarg.h, math.h.
 *
 * Structures and Enumerations:
 * - flag_type: Structure defining flags for formatting options (MINUS, PLUS,
 * SPACE, ZERO, SHARP).
 * - specifier_type: Enumeration of format specifiers for different types (CHAR,
 * INT, FLOAT, STRING, etc.).
 * - len_type: Enumeration of length modifiers for numeric specifiers (SHORT,
 * LONG).
 * - opt: Structure containing formatting options (flags, width, precision,
 * length specifier, format specifier).
 * - var: Structure holding variables used during string formatting (error flag,
 * buffers).
 *
 * Included functionalities:
 *
 * This header file provides a interface for handling formatted string output,
 * implementing various format specifiers and ensuring proper memory management
 * during operation. It is intended to be included in source files where
 * formatted string handling is required, ensuring compatibility and
 * functionality similar to standard sprintf functions.
 */
#ifndef SRC_S21_SPRINTF_H_
#define SRC_S21_SPRINTF_H_

#include "s21_string.h"

typedef struct flags {
  int MINUS;
  int PLUS;
  int SPACE;
  int ZERO;
  int SHARP;
} flag_type;

typedef enum specifiers {
  NO_SPECIFIER,
  CHAR_SPECIFIER,
  INT_DEC_SPECIFIER,
  INT_HEX_SPECIFIER,
  FLOAT_SPECIFIER,
  STRING_SPECIFIER,
  UNSIGNED_SPECIFIER,
  PERCENT_SPECIFIER,
  EXP_LOW_SPECIFIER,
  EXP_UP_SPECIFIER,
  FLOAT_EXP_LOW_SPECIFIER,
  FLOAT_EXP_UP_SPECIFIER,
  HEX_LOW_SPECIFIER,
  HEX_UP_SPECIFIER,
  OCTAL_SPECIFIER,
  COUNT_SPECIFIER,
  POINTER_SPECIFIER
} specifier_type;

typedef enum len_specifier {
  NO_LEN_SPECIFIER,
  SHORT_LEN_SPECIFIER,
  LONG_LOWERCASE_LEN_SPECIFIER,
  LONG_UPPERCASE_LEN_SPECIFIER
} len_type;

typedef struct options {
  flag_type flags;
  int min_width;  // -1: undefined
  int precision;  // -1: undefined
  len_type length_spec;
  specifier_type format_spec;
} opt;

typedef struct variables {
  int error_flag;
  char *char_buffer;
  char *temporary_buffer;
  char *temporary_pointer;
} var;

// __Initialization__
void s21_initialize_options(opt *options);
// __Options__
void s21_options(const char **format, opt *options, va_list var_arg);
void s21_set_format_flags(const char **format, opt *options);
void s21_min_width(const char **format, opt *options, va_list var_arg);
void s21_precision(const char **format, opt *options, va_list var_arg);
void s21_length_spec(const char **format, opt *options);
void s21_format_spec(const char **format, opt *options);
// __Add conversions__
int s21_atoi(const char **str);
// __Process__
void s21_process_format_specifier(char **str, opt options, va_list var_arg,
                                  int *n_smb, var *variables);
int s21_int_specifiers(char **str, opt options, va_list var_arg,
                       var *variables);
int s21_e_specifiers(char **str, opt options, long double double_var,
                     var *variables);
int s21_f_specifier(char **str, opt options, long double double_var,
                    var *variables);
int s21_g_specifiers(char **str, opt *options, long double double_var,
                     var *variables);
int s21_perc_specifier(char **str, opt options, var *variables);
void s21_n_specifier(opt options, va_list var_arg, long int n_smb);
int s21_c_specifier(char **str, opt options, va_list var_arg, var *variables);
int s21_s_specifier(char **str, opt options, va_list var_arg, var *variables);
//  __Mem__
void s21_free_at_exit(var *variables);
void s21_mem_check(char *str, var *variables);
// conversions
char s21_convert_digit_to_char(int digit, int text_case);
char *s21_unsigned_to_str(unsigned long int num, unsigned int notation,
                          int text_case);
char *s21_mantissa_to_str(long double num, int *next_digit, opt options);
char *s21_float_to_str(long double num);
void s21_concatenate_wide_chars(char *str, wchar_t *wstr, int len);
void s21_invert_str(char *origin, char *inverted);
// obtainig values
void s21_split_float(long double double_var, long double *ones,
                     long double *tens);
long double s21_double_variable(opt options, va_list var_arg);
long unsigned s21_unsigned_variable(opt options, va_list var_arg,
                                    int *is_negative);
unsigned s21_exponent(long double *mantissa);
void s21_char_sign(int is_negative, char *sign, opt options);
void s21_apply_precision_limit(int *wlen, opt options);
// output formatting
void s21_apply_num_precision(char **buf, int precision);
void s21_add_notation(char **buf, opt options);
void s21_add_sign(char **buf, char sign);
void s21_apply_width(char **buf, opt options, var *variables);
void s21_delete_trailing_zeros(char **num_string, opt options);
void s21_math_rounding(char **num_string, int next_digit, char *exp_sign,
                       unsigned *u_exp);
// others
size_t s21_wchar_string_length(wchar_t *wstr);
int s21_buffer_to_string(char **str, var *variables);
char *s21_nan_inf(long double variable, char *sign, specifier_type format_spec);
char *s21_sum_and_free_strings(char *string1, char *string2);
int s21_is_spec_int(specifier_type spec);
int s21_is_spec_float(specifier_type spec);
void s21_handle_char_specifier(opt options, va_list var_arg, var *variables);
void s21_handle_format_specifier(opt options, va_list var_arg, var *variables);

#endif  // SRC_S21_SPRINTF_H_