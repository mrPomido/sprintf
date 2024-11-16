/**
 * @brief Function for formatted reading from a string
 *
 * * This function is similar to the `sscanf` function from the standard C
 * library, but has the prefix `s21_` to avoid name conflicts. It reads the data
 * from the string `str` according to the format `format` and puts the results
 * in the variables specified in `arguments`.
 *
 * @param str The string from which the data will be read
 * @param format Format string defining the data structure to be read
 * @param arguments A list of arguments in which the read data will be placed
 * @return The number of successfully read items
 *
 * @* @note The return value of the `s21_sscanf` function may differ from the
 * return value of `sscanf`, as the implementation may use different algorithms
 * for error handling and formats.
 *
 * @warning It is necessary to ensure that the number of arguments in
 * `arguments` matches the number of specifications in `format`, otherwise the
 * behavior of the function may be undefined.
 */
#ifndef SRC_S21_SSCANF_H_
#define SRC_S21_SSCANF_H_

#include "s21_string.h"

// __Parsing functions__
int s21_parse_and_match(char **str, char **format);
unsigned long long int s21_convert_string_to_unsigned_long_long(char **, int,
                                                                int *, int);
long long int s21_convert_string_to_long_long(char **, int, int *, int);
unsigned long long int s21_convert_string_to_unsigned_long_long_base(char **,
                                                                     int, int *,
                                                                     int);
long double s21_parse_string_to_long_double_with_exponent(char **, int, int *,
                                                          int);
char *s21_parse_and_copy(char *temp_str, int *lens, int width,
                         int *parsing_status, char *temp_result);
// Assignment functions
void s21_assign_result_by_width_specifier(unsigned long long int *result,
                                          va_list argument_pointer,
                                          int *assignment_target_type);
void s21_assign_unsigned_result_by_width_specifier(
    unsigned long long int *result, va_list argument_pointer,
    int *assignment_target_type);
void s21_format_long_double_result_with_width(long double *result,
                                              va_list argument_pointer,
                                              int *assignment_target_type);
// Processing functions
void s21_process_sign_character_in_input(char **str, int *width, int *sign);
int s21_detect_and_handle_infinity_nan(char **str, long double *res, int *width,
                                       int *sign);
void s21_str_tolower(char *str);
// Character classification functions
int s21_digit(int c);
int s21_letter(int c);
int s21_is_x(int c);
int s21_is_e(int c);
// Conversion and parsing helper functions
void s21_asterisk(char **temp, int *missing_specs_count);
void s21_parse_and_handle_base(char **, int *i, int width, int base);
void s21_handle_length_modifier(char **temp_format,
                                int *assignment_target_type);
// Conversion specifier handling functions
void s21_handle_char_conversion(char **temp_str, va_list argument_pointer,
                                int *result, int *missing_specs_count,
                                int *processing_state, int *parsing_status,
                                int width, int s21_len);
void s21_handle_int_conversion(char **temp_str, va_list argument_pointer,
                               int *result, int *missing_specs_count,
                               int *processing_state, int *parsing_status,
                               char specifier, int width,
                               int assignment_target_type, char *whitespace);
void s21_handle_base_conversion(char **temp_str, va_list argument_pointer,
                                int *result, int *missing_specs_count,
                                int *processing_state, int *parsing_status,
                                char specifier, int width,
                                int assignment_target_type, char *whitespace);
void s21_handle_float_conversion(char **temp_str, va_list argument_pointer,
                                 int *result, int *missing_specs_count,
                                 int *processing_state, int *parsing_status,
                                 char specifier, int width,
                                 int assignment_target_type, int e,
                                 char *whitespace);
void s21_handle_string_conversion(char **temp_str, va_list argument_pointer,
                                  int *result, int *missing_specs_count,
                                  int *processing_state, int *parsing_status,
                                  int width, char *temp_result,
                                  char *whitespace, int lens);
void s21_handle_pointer_conversion(char **temp_str, va_list argument_pointer,
                                   int *result, int *missing_specs_count,
                                   int *processing_state, int *parsing_status,
                                   int width, int assignment_target_type,
                                   char *whitespace);
void s21_handle_n_conversion(char **temp_str, va_list argument_pointer,
                             const char *str, int *missing_specs_count,
                             int *processing_state, int assignment_target_type);
void s21_handle_percent_conversion(char **temp_str, int *parsing_status,
                                   char *whitespace);
int s21_handle_specifier(char **temp_str, char **temp_format,
                         va_list argument_pointer, int *result,
                         int *missing_specs_count, int *processing_state,
                         int *parsing_status, int width,
                         int assignment_target_type, char *whitespace,
                         const char *str);
void s21_handle_exponent(char **str, int *width, long double *result, int *i,
                         int *sign, unsigned long long int *p);
#endif  //  SRC_S21_SSCANF_H_