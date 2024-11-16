/**
 * @file s21_sscanf.c
 * @brief Implementation of formatted string input function similar to sscanf.
 *
 * This file defines the function `s21_sscanf`, which reads data from a string
 * according to a format string and stores the results in the specified
 * variables. The function is similar to the standard `sscanf` function found
 * in the C library.
 *
 * Function Overview:
 * - `sscanf`: Reads data from a string based on the provided format string and
 * stores the results in the specified variables.
 *
 * @param str String to read data from
 * @param format Format string that controls how data is interpreted
 * @param ... Variable list of pointers where the parsed data will be stored
 * @return Number of successfully parsed items
 *
 * @note The return value may differ from the standard `sscanf` due to different
 * implementation details and error handling strategies.
 *
 * @warning It is crucial that the number of variables in the variable list
 * matches the number of specifiers in the format string, as improper usage can
 * lead to undefined behavior.
 */
#include "s21_sscanf.h"
/**
 * @brief The s21_sscanf function performs formatted input from a string
 *   according to the format specifications.
 *
 * @param str Pointer to the line to enter.
 * @param format Pointer to the format string that defines the data entry.
 * @return Returns the number of successfully processed specifications, or 0 if
 * an error occurred.
 */
int s21_sscanf(const char *str, const char *format, ...) {
  int result = 0, parsing_status = 0, width = 0;
  char *temp_str = (char *)str, *temp_format = (char *)format;
  char whitespace[7] = " \f\n\r\t\v";
  va_list argument_pointer;
  int processing_state = 1;
  int missing_specs_count = 0;
  va_start(argument_pointer, format);
  while (*temp_format && !parsing_status) {
    parsing_status = s21_parse_and_match(&temp_str, &temp_format);
    if ((processing_state && *temp_str) ||
        (processing_state && *temp_format == '%'))
      result = 0;

    if (*temp_format == '%') {
      temp_format++;
      s21_asterisk(&temp_format, &missing_specs_count);
      if (*temp_format >= '0' && *temp_format <= '9') {
        width = s21_convert_string_to_unsigned_long_long(&temp_format, 0,
                                                         &parsing_status, 117);
      }
      s21_asterisk(&temp_format, &missing_specs_count);
    }
    int assignment_target_type = 0;
    s21_handle_length_modifier(&temp_format, &assignment_target_type);
    if (!parsing_status) {
      parsing_status = s21_handle_specifier(
          &temp_str, &temp_format, argument_pointer, &result,
          &missing_specs_count, &processing_state, &parsing_status, width,
          assignment_target_type, whitespace, str);
      width = 0;
    }
    if (result) processing_state = 0;
    if (processing_state != 2) processing_state = 0;
  }
  va_end(argument_pointer);
  return result;
}
/**
 * @brief Handles various format specifiers for a custom formatting function.
 *
 * @param temp_str A pointer to the current position in the input string.
 * @param temp_format A pointer to the current position in the format string.
 * @param argument_pointer The list of arguments passed to the custom form.
 * function.
 * @param result A pointer to the variable storing the cumulative result of the
 * form.
 * @param missing_specs_count A pointer to the counter for missing format
 * specifiers.
 * @param processing_state A pointer to the variable representing the current
 * processing state.
 * @param parsing_status A pointer to the variable indicating the parsing
 * status.
 * @param width The width specifier for formatting.
 * @param assignment_target_type Additional state or information used for
 * certain specifiers.
 * @param whitespace A string containing whitespace characters to be used in
 * formatting.
 * @param str The original input string.
 * @return An integer indicating the parsing status
 */
int s21_handle_specifier(char **temp_str, char **temp_format,
                         va_list argument_pointer, int *result,
                         int *missing_specs_count, int *processing_state,
                         int *parsing_status, int width,
                         int assignment_target_type, char *whitespace,
                         const char *str) {
  char temp_result[1024] = {0};
  int s21_len = s21_strlen(*temp_str);
  int lens = 0;
  int e = 0;
  switch (**temp_format) {
    case 'c':
      s21_handle_char_conversion(temp_str, argument_pointer, result,
                                 missing_specs_count, processing_state,
                                 parsing_status, width, s21_len);
      break;
    case 'd':
    case 'u':
      s21_handle_int_conversion(temp_str, argument_pointer, result,
                                missing_specs_count, processing_state,
                                parsing_status, **temp_format, width,
                                assignment_target_type, whitespace);
      break;
    case 'i':
    case 'o':
    case 'x':
    case 'X':
      s21_handle_base_conversion(temp_str, argument_pointer, result,
                                 missing_specs_count, processing_state,
                                 parsing_status, **temp_format, width,
                                 assignment_target_type, whitespace);
      break;
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'f':
      s21_handle_float_conversion(temp_str, argument_pointer, result,
                                  missing_specs_count, processing_state,
                                  parsing_status, **temp_format, width,
                                  assignment_target_type, e, whitespace);
      break;
    case 's':
      s21_handle_string_conversion(temp_str, argument_pointer, result,
                                   missing_specs_count, processing_state,
                                   parsing_status, width, temp_result,
                                   whitespace, lens);
      break;
    case 'p':
      s21_handle_pointer_conversion(temp_str, argument_pointer, result,
                                    missing_specs_count, processing_state,
                                    parsing_status, width,
                                    assignment_target_type, whitespace);
      break;
    case 'n':
      s21_handle_n_conversion(temp_str, argument_pointer, str,
                              missing_specs_count, processing_state,
                              assignment_target_type);
      break;
    case '%':
      s21_handle_percent_conversion(temp_str, parsing_status, whitespace);
      break;
    default:
      *parsing_status = 1;
      break;
  }
  if (**temp_format) (*temp_format)++;
  return *parsing_status;
}
/**
 * @brief Handles the 'c' conversion specifier for character arguments.
 *
 * @param temp_str A pointer to the current position in the input string.
 * @param argument_pointer The va_list containing the arg. passed to the
 * form. func.
 * @param result A pointer to the variable storing the cumulative result of the
 * form. process.
 * @param missing_specs_count A pointer to the counter for missing format
 * specifiers.
 * @param processing_state A pointer to the variable representing the current
 * processing state.
 * @param parsing_status A pointer to the variable indicating the parsing
 * status.
 * @param width The width specifier for formatting.
 * @param s21_len The length of the current position in the input string
 * (*temp_str).
 */
void s21_handle_char_conversion(char **temp_str, va_list argument_pointer,
                                int *result, int *missing_specs_count,
                                int *processing_state, int *parsing_status,
                                int width, int s21_len) {
  if (*temp_str) {
    if (!(*missing_specs_count)) {
      *va_arg(argument_pointer, char *) = **temp_str;
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
    if (width == 0)
      (*temp_str) += 1;
    else if (width <= s21_len)
      (*temp_str) += width;
    else
      (*temp_str) += s21_len;
  } else {
    *parsing_status = 1;
    if (*processing_state) *result = -1;
  }
}
/**
 * @brief Handles integer conversion from a string based on specifier.
 *
 * @param temp_str Pointer to pointer to current position in string to process.
 *                Updated to skip leading whitespace.
 * @param argument_pointer List of arguments for variable argument processing.
 * @param result Pointer to integer storing number of successful conversions.
 *               Incremented on success, set to -1 on failure.
 * @param missing_specs_count Pointer to integer counting missing
 * specifications. If zero, processes conversion; sets `processing_state` to 2
 * otherwise.
 * @param processing_state Pointer to integer representing current processing
 * state: 0 - No ongoing processing, 1 - Ongoing processing, 2 - State
 * indicating missing specifications. Updated based on conversion and
 * `missing_specs_count`.
 * @param parsing_status Pointer to integer indicating parsing status:
 *                       0 - Parsing successful,
 *                       1 - Parsing failed.
 *                       Determines execution flow within the function.
 * @param specifier Character specifying conversion type ('d', 'u', etc.).
 * @param width Width of field for conversion.
 * @param assignment_target_type Integer used in conversion functions
 *                               `s21_assign_unsigned_result_by_width_specifier`
 * or `s21_assign_result_by_width_specifier`.
 * @param whitespace String of characters considered whitespace, used to skip
 * initial spaces.
 */
void s21_handle_int_conversion(char **temp_str, va_list argument_pointer,
                               int *result, int *missing_specs_count,
                               int *processing_state, int *parsing_status,
                               char specifier, int width,
                               int assignment_target_type, char *whitespace) {
  *temp_str += s21_strspn(*temp_str, whitespace);
  unsigned long long sum = s21_convert_string_to_unsigned_long_long(
      temp_str, width, parsing_status, specifier);
  if (!*parsing_status) {
    if (!*missing_specs_count) {
      if (specifier == 'u') {
        s21_assign_unsigned_result_by_width_specifier(&sum, argument_pointer,
                                                      &assignment_target_type);
      } else {
        s21_assign_result_by_width_specifier(&sum, argument_pointer,
                                             &assignment_target_type);
      }
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
  } else if (*processing_state && !**temp_str) {
    *result = -1;
    *processing_state = 0;
  }
}
/**
 * @brief Handles integer base conversion specifiers ('i', 'o', 'x', 'X') in a
 * format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param argument_pointer Pointer to va_list for variadic arguments.
 * @param result Pointer to result count.
 * @param missing_specs_count Pointer to count of missing specifiers.
 * @param processing_state Pointer to processing state.
 * @param parsing_status Pointer to parsing status.
 * @param specifier Conversion specifier character ('i', 'o', 'x', 'X').
 * @param width Width specifier for parsing.
 * @param assignment_target_type Type specifier for assignment.
 * @param whitespace String containing whitespace characters to skip.
 */
void s21_handle_base_conversion(char **temp_str, va_list argument_pointer,
                                int *result, int *missing_specs_count,
                                int *processing_state, int *parsing_status,
                                char specifier, int width,
                                int assignment_target_type, char *whitespace) {
  unsigned base = 0;
  if (specifier == 'i')
    base = 10;
  else if (specifier == 'o')
    base = 8;
  else
    base = 16;
  *temp_str += s21_strspn(*temp_str, whitespace);
  unsigned long long sum;
  if (specifier == 'i')
    sum =
        s21_convert_string_to_long_long(temp_str, width, parsing_status, base);
  else
    sum = s21_convert_string_to_unsigned_long_long_base(temp_str, width,
                                                        parsing_status, base);
  if (!*parsing_status) {
    if (!*missing_specs_count) {
      if (specifier == 'i') {
        s21_assign_result_by_width_specifier(&sum, argument_pointer,
                                             &assignment_target_type);
      } else {
        s21_assign_unsigned_result_by_width_specifier(&sum, argument_pointer,
                                                      &assignment_target_type);
      }
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
  } else if (*processing_state && !**temp_str) {
    *result = -1;
    *processing_state = 0;
  }
}
/**
 * @brief Handles the floating-point conversion specifier ('f', 'F', 'e', 'E',
 * 'g', 'G') in a format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param argument_pointer Pointer to va_list for variadic arguments.
 * @param result Pointer to result count.
 * @param missing_specs_count Pointer to count of missing specifiers.
 * @param processing_state Pointer to processing state.
 * @param parsing_status Pointer to parsing status.
 * @param specifier Conversion specifier character ('f', 'F', 'e', 'E', 'g',
 * 'G').
 * @param width Width specifier for parsing.
 * @param assignment_target_type Type specifier for assignment.
 * @param e Flag indicating presence of exponent in format.
 * @param whitespace String containing whitespace characters to skip.
 */
void s21_handle_float_conversion(char **temp_str, va_list argument_pointer,
                                 int *result, int *missing_specs_count,
                                 int *processing_state, int *parsing_status,
                                 char specifier, int width,
                                 int assignment_target_type, int e,
                                 char *whitespace) {
  (void)specifier;
  *temp_str += s21_strspn(*temp_str, whitespace);
  long double converted_float = s21_parse_string_to_long_double_with_exponent(
      temp_str, width, parsing_status, e);

  if (!*parsing_status) {
    if (!*missing_specs_count) {
      s21_format_long_double_result_with_width(
          &converted_float, argument_pointer, &assignment_target_type);
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
  } else if (*processing_state && !**temp_str) {
    *result = -1;
    *processing_state = 0;
  }
}
/**
 * @brief Handles the string conversion specifier ('s') in a format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param argument_pointer Pointer to va_list for variadic arguments.
 * @param result Pointer to result count.
 * @param missing_specs_count Pointer to count of missing specifiers.
 * @param processing_state Pointer to processing state.
 * @param parsing_status Pointer to parsing status.
 * @param width Width specifier for parsing.
 * @param temp_result Buffer to store parsed string.
 * @param whitespace String containing whitespace characters to skip.
 * @param lens Length of parsed string.
 */
void s21_handle_string_conversion(char **temp_str, va_list argument_pointer,
                                  int *result, int *missing_specs_count,
                                  int *processing_state, int *parsing_status,
                                  int width, char *temp_result,
                                  char *whitespace, int lens) {
  *temp_str += s21_strspn(*temp_str, whitespace);
  s21_parse_and_copy(*temp_str, &lens, width, parsing_status, temp_result);
  *temp_str += lens;
  lens = 0;

  if (!*parsing_status) {
    if (!*missing_specs_count) {
      s21_strcpy(va_arg(argument_pointer, char *), temp_result);
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
  } else if (*processing_state && !**temp_str) {
    *result = -1;
    *processing_state = 0;
  }
}
/**
 * @brief Handles the pointer conversion specifier ('p') in a format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param argument_pointer Pointer to va_list for variadic arguments.
 * @param result Pointer to result count.
 * @param missing_specs_count Pointer to count of missing specifiers.
 * @param processing_state Pointer to processing state.
 * @param parsing_status Pointer to parsing status.
 * @param width Width specifier for parsing.
 * @param assignment_target_type Type of assignment target.
 * @param whitespace String containing whitespace characters to skip.
 */
void s21_handle_pointer_conversion(char **temp_str, va_list argument_pointer,
                                   int *result, int *missing_specs_count,
                                   int *processing_state, int *parsing_status,
                                   int width, int assignment_target_type,
                                   char *whitespace) {
  unsigned base = 16;
  *temp_str += s21_strspn(*temp_str, whitespace);
  unsigned long long sum = s21_convert_string_to_unsigned_long_long_base(
      temp_str, width, parsing_status, base);

  if (!*parsing_status) {
    if (!*missing_specs_count) {
      assignment_target_type = 3;
      s21_assign_unsigned_result_by_width_specifier(&sum, argument_pointer,
                                                    &assignment_target_type);
      (*result)++;
    } else {
      *missing_specs_count = 0;
      *processing_state = 2;
    }
  } else if (*processing_state && !**temp_str) {
    *result = -1;
    *processing_state = 0;
  }
}
/**
 * @brief Handles the 'n' conversion specifier in a format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param argument_pointer Pointer to va_list for variadic arguments.
 * @param str Pointer to the format string.
 * @param missing_specs_count Pointer to count of missing specifiers.
 * @param processing_state Pointer to processing state.
 * @param assignment_target_type Type of assignment target.
 */
void s21_handle_n_conversion(char **temp_str, va_list argument_pointer,
                             const char *str, int *missing_specs_count,
                             int *processing_state,
                             int assignment_target_type) {
  if (!*missing_specs_count) {
    unsigned long long sum = *temp_str - str;
    s21_assign_result_by_width_specifier(&sum, argument_pointer,
                                         &assignment_target_type);
  } else {
    *missing_specs_count = 0;
    *processing_state = 2;
  }
}
/**
 * @brief Handles the '%' conversion specifier in a format string.
 *
 * @param temp_str Pointer to current position in the format string.
 * @param parsing_status Pointer to integer storing parsing status (0: success,
 * 1: failure).
 * @param whitespace String containing characters considered as whitespace.
 */
void s21_handle_percent_conversion(char **temp_str, int *parsing_status,
                                   char *whitespace) {
  *temp_str += s21_strspn(*temp_str, whitespace);
  if (**temp_str == '%')
    (*temp_str)++;
  else
    *parsing_status = 1;
}
/**
 * @brief Handles the length modifier in a format string.
 *
 * @param temp_format Pointer to current position in the format string.
 * @param assignment_target_type Pointer to integer storing the type based on
 * length modifier.
 */
void s21_handle_length_modifier(char **temp_format,
                                int *assignment_target_type) {
  if (**temp_format == 'h' && *(*temp_format + 1) == 'h') {
    *assignment_target_type = 1;
    *temp_format += 2;
  } else if (**temp_format == 'h') {
    *assignment_target_type = 2;
    (*temp_format)++;
  } else if (**temp_format == 'l' && *(*temp_format + 1) == 'l') {
    *assignment_target_type = 4;
    *temp_format += 2;
  } else if (**temp_format == 'l') {
    *assignment_target_type = 3;
    (*temp_format)++;
  } else if (**temp_format == 'L') {
    *assignment_target_type = 5;
    (*temp_format)++;
  }
}
/**
 * @brief Parses and matches substrings between `*str` and `*format`.
 *
 * @param str Pointer to current position in the input string.
 * @param format Pointer to current position in the format string.
 * @return Integer indicating parsing and matching status (0: success, 1:
 * failure).
 */
int s21_parse_and_match(char **str, char **format) {
  char whitespace[7] = " \f\n\r\t\v";
  char del[8] = " \f\n\r\t\v\%";
  int parsing_status = 0;
  while ((**format && **format != 37) && !parsing_status) {
    int len = 0;
    if (s21_strchr(whitespace, **format) != s21_NULL) {
      len = (int)s21_strspn(*format, whitespace);
      *format += len;
      len = (int)s21_strspn(*str, whitespace);
      *str += len;
    }
    len = (int)s21_strcspn(*format, del);
    if (!s21_strncmp(*str, *format, len)) {
      *str += len;
      *format += len;
    } else {
      parsing_status = 1;
    }
  }
  return parsing_status;
}
/**
 * @brief Parses and copies a substring from `temp_str` into `temp_result`.
 *
 * @param temp_str Input string to parse from.
 * @param lens Pointer to an integer storing the length of copied substring.
 * @param width Maximum width of the substring to copy.
 * @param parsing_status Pointer to an integer indicating parsing success (0:
 * success, 1: failure).
 * @param temp_result Buffer to store the copied substring.
 * @return Pointer to `temp_result` containing the copied substring.
 */
char *s21_parse_and_copy(char *temp_str, int *lens, int width,
                         int *parsing_status, char *temp_result) {
  char whitespace[7] = " \f\n\r\t\v";
  int len = s21_strcspn(temp_str, whitespace);

  if (width == 0) width = INT_MAX;
  int i = 0;
  for (; *(temp_str + i) && i < len && i < width; i++, (*lens)++) {
    *(temp_result + i) = *(temp_str + i);
  }
  *(temp_result + i) = '\0';
  if (i == 0) *parsing_status = 1;
  return temp_result;
}
/**
 * @brief Converts a sequence of s21_digits from a string to an integer based on
 * the specifier.
 *
 * @param str Pointer to current position in the input string.
 * @param width Maximum width of the input string to parse.
 * @param parsing_status Pointer to an integer indicating parsing success (0:
 * success, 1: failure).
 * @param c Specifier character ('u' for unsigned, 'd' for signed).
 * @return Converted unsigned long long integer from the string.
 */
unsigned long long int s21_convert_string_to_unsigned_long_long(
    char **str, int width, int *parsing_status, int c) {
  unsigned long long int result = 0;
  int sign = 1, base = 10;
  if (width == 0) width = INT_MAX;
  s21_process_sign_character_in_input(str, &width, &sign);
  int i = 0;
  int full = 0;
  for (; i < width && s21_digit(**str); (*str)++, i++) {
    if (!full && ((c == 117 && result * base + (**str - 48) >= result) ||
                  (c == 100 && result * base + (**str - 48) <= LONG_MAX)))
      result = result * base + (**str - 48);
    else if (!full) {
      if (c == 117) {
        result = ULONG_MAX;
      } else if (c == 100) {
        sign == 1 ? (result = LONG_MAX) : (result = LONG_MIN);
      }
      full = 1;
    }
  }
  if (i == 0) *parsing_status = 1;
  return full ? result : result * sign;
}
/**
 * @brief Converts a string to a long long integer based on a specified base.
 *
 * @param str Pointer to current position in the input string.
 * @param width Maximum width of the input string to parse.
 * @param parsing_status Pointer to an integer indicating parsing success (0:
 * success, 1: failure).
 * @param base Base of the numeric system to interpret the string (e.g., 8, 10,
 * 16).
 * @return Converted long long integer from the string.
 */
long long int s21_convert_string_to_long_long(char **str, int width,
                                              int *parsing_status, int base) {
  long long int result = 0;
  int sign = 1, temp = 0;

  if (width == 0) width = INT_MAX;
  s21_process_sign_character_in_input(str, &width, &sign);

  int i = 0;
  s21_parse_and_handle_base(str, &i, width, base);

  if (i == 1 && base == 10)
    base = 8;
  else if (i == 2)
    base = 16;

  int full = 0;
  if (base != 16) {
    for (; i < width && (**str >= 48 && **str <= 48 + base - 1);
         (*str)++, i++) {
      if (!full && result * base + (**str - 48) >= result)
        result = result * base + (**str - 48);
      else if (!full) {
        sign == 1 ? (result = LONG_MAX) : (result = LONG_MIN);
        full = 1;
      }
    }
  } else {
    for (; i < width && (s21_digit(**str) || s21_letter(**str));
         (*str)++, i++) {
      if (s21_digit(**str)) {
        temp = (**str) - 48;
      } else {
        temp = (**str) % 32 + 9;
      }

      if (!full && result * base + temp >= result) {
        result = result * base + temp;
      } else if (!full) {
        sign == 1 ? (result = LONG_MAX) : (result = LONG_MIN);
        full = 1;
      }
    }
  }
  if (i == 0) *parsing_status = 1;
  return full ? result : result * sign;
}
/**
 * @brief Converts a string to an unsigned long long integer based on a
 * specified base.
 *
 * @param str Pointer to current position in the input string.
 * @param width Maximum width of the input string to parse.
 * @param parsing_status Pointer to an integer indicating parsing success (0:
 * success, 1: failure).
 * @param base Base of the numeric system to interpret the string (e.g., 8, 10,
 * 16).
 * @return Converted unsigned long long integer from the string.
 */
unsigned long long int s21_convert_string_to_unsigned_long_long_base(
    char **str, int width, int *parsing_status, int base) {
  unsigned long long int result = 0;
  int sign = 1, temp = 0;

  if (width == 0) width = INT_MAX;
  s21_process_sign_character_in_input(str, &width, &sign);

  int i = 0;
  s21_parse_and_handle_base(str, &i, width, base);

  if (i == 1 && base == 10)
    base = 8;
  else if (i == 2)
    base = 16;

  int full = 0;

  if (base != 16) {
    for (; i < width && (**str >= 48 && **str <= 48 + base - 1);
         (*str)++, i++) {
      if (!full && result * base + (**str - 48) >= result)
        result = result * base + (**str - 48);
      else if (!full) {
        result = ULONG_MAX;
        full = 1;
      }
    }
  } else {
    for (; i < width && (s21_digit(**str) || s21_letter(**str));
         (*str)++, i++) {
      if (s21_digit(**str)) {
        temp = (**str) - 48;
      } else {
        temp = (**str) % 32 + 9;
      }
      if (!full && result * base + temp >= result) {
        result = result * base + temp;
      } else if (!full) {
        result = ULONG_MAX;
        full = 1;
      }
    }
  }
  if (i == 0) *parsing_status = 1;

  return full ? result : result * sign;
}
/**
 * @brief Parses a string to a long double with exponent notation support.
 *
 * @param str Pointer to the current position in the input string.
 * @param width Maximum width of the input string to parse.
 * @param parsing_status Pointer to an integer indicating parsing success (0:
 * success, 1: failure).
 * @param e Indicator for handling exponent notation (0: ignore, 1: handle).
 * @return Parsed long double value from the string.
 */
long double s21_parse_string_to_long_double_with_exponent(char **str, int width,
                                                          int *parsing_status,
                                                          int e) {
  long double result = 0, del = 1;  // inf = 1.0/0.0, nan = 0.0/0.0;
  int point = 0, sign = 1, base = 10, check = 0, temp = 0;
  unsigned long long int p = 1;
  if (width == 0) width = INT_MAX;
  s21_process_sign_character_in_input(str, &width, &sign);
  check = s21_detect_and_handle_infinity_nan(str, &result, &width, &sign);
  int i = 0;
  if (!check) {
    if (**str == '0' && width > 0) {
      i = 1;
      (*str)++;
      if (s21_is_x(**str) && width - i > 0) {
        i = 2;
        base = 16;
        e = 0;
        if (width - i > 0 &&
            (s21_letter(*(*str + 1)) || s21_digit(*(*str + 1)))) {
          (*str)++;
        } else if (width - i > 0 && *(*str + 1) == 46) {
          point = 1, i = 3;
          if (width - i > 0 &&
              (s21_letter(*(*str + 2)) || s21_digit(*(*str + 2))))
            (*str) += 2;
        }
      }
    }
    for (; i < width && (e == 0 || (e == 1 && !s21_is_e(**str))) &&
           (((s21_digit(**str)) || (s21_letter(**str) && base == 16)) ||
            (**str == 46 && point == 0));
         i++, (*str)++) {
      if (point < 2) {
        if (**str != '.') {
          if (s21_digit(**str)) {
            temp = (**str) - 48;
          } else {
            temp = (**str) % 32 + 9;
          }
          result = result * base + temp;
          if (point == 1) del *= base;
        } else {
          point++;
        }
      }
    }
    result /= del;
    result *= sign;
    if (point == 1 && i == 1) *parsing_status = 1;
    if (i < width && s21_is_e(**str)) {
      s21_handle_exponent(str, &width, &result, &i, &sign, &p);
    }
    if (i == 0 && check == 0) *parsing_status = 1;
  }
  return result;
}
/**
 * @brief Handles exponent notation in a string and adjusts a long double
 * result.
 *
 * @param str Pointer to current position in the input string, may be updated to
 * skip patterns.
 * @param width Pointer to remaining width of the input string to process,
 * restricting pattern length.
 * @param result Pointer to a long double to be adjusted based on the exponent.
 * @param i Pointer to an integer tracking the current position in the string.
 * @param sign Pointer to the sign of the exponent: -1 if negative, 0 if none, 1
 * if positive.
 * @param p Pointer to an unsigned long long storing the exponent value.
 */
void s21_handle_exponent(char **str, int *width, long double *result, int *i,
                         int *sign, unsigned long long int *p) {
  if (((*(*str + 1) == 43 || *(*str + 1) == 45) && s21_digit(*(*str + 2)) &&
       *i + 2 < *width) ||
      (*i + 1 < *width && s21_digit(*(*str + 1)))) {
    (*str)++, (*i)++;  // исправлено: использование результата инкремента
    *sign = 0;
    s21_process_sign_character_in_input(str, width, sign);
    if (*sign != 0)
      (*i)++;
    else
      *sign = 1;

    *p = 0;
    for (; *i < *width && s21_digit(**str); (*str)++, (*i)++) {
      *p = *p * 10 + (**str - 48);
    }
    for (; *p > 0;
         (*p)--) {  // исправлено: использование результата выражения *p--
      if (*sign == -1) {
        *result /= 10;
        if (*result == 0) *p = 1;
      } else {
        *result *= 10;
        if (isinf(*result)) *p = 1;
        if (*result == 0) *p = 1;
      }
    }
  }
}
/**
 * @brief Formats and assigns a long double result to various floating-point
 * types.
 *
 * @param result Pointer to a long double containing the result.
 * @param argument_pointer Argument pointer for variable arguments to retrieve
 * target variables.
 * @param assignment_target_type Pointer to an integer indicating the target
 * type
 */
void s21_format_long_double_result_with_width(long double *result,
                                              va_list argument_pointer,
                                              int *assignment_target_type) {
  if (*assignment_target_type == 0 || *assignment_target_type == 2) {
    *va_arg(argument_pointer, float *) = (float)*result;
  } else if (*assignment_target_type == 3) {
    *va_arg(argument_pointer, double *) = (double)*result;
  } else if (*assignment_target_type == 4) {
    va_arg(argument_pointer, float *);
  } else if (*assignment_target_type == 5) {
    *va_arg(argument_pointer, long double *) = *result;
  }
  *assignment_target_type = 0;
}
/**
 * @brief Assigns a result to various integer types based on a width specifier.
 *
 * @param result Pointer to an unsigned long long integer containing the result.
 * @param argument_pointer Argument pointer for variable arguments to retrieve
 * target variables.
 * @param assignment_target_type Pointer to an integer indicating the target
 * type:
 */
void s21_assign_result_by_width_specifier(unsigned long long int *result,
                                          va_list argument_pointer,
                                          int *assignment_target_type) {
  if (*assignment_target_type == 0) {
    *va_arg(argument_pointer, int *) = (int)*result;
  } else if (*assignment_target_type == 1) {
    *va_arg(argument_pointer, char *) = (char)*result;
  } else if (*assignment_target_type == 2) {
    *va_arg(argument_pointer, short int *) = (short int)*result;
  } else if (*assignment_target_type == 3) {
    *va_arg(argument_pointer, long int *) = (long int)*result;
  } else if (*assignment_target_type == 4) {
    *va_arg(argument_pointer, long long int *) = (long long int)*result;
  }
  *assignment_target_type = 0;
}
/**
 * @brief Assigns an unsigned integer based on a width specifier.
 *
 * @param result Pointer to an unsigned long long integer containing the result.
 * @param argument_pointer Argument pointer for variable arguments to retrieve
 * target variables.
 * @param assignment_target_type Pointer to an integer indicating the target
 * type:
 */
void s21_assign_unsigned_result_by_width_specifier(
    unsigned long long int *result, va_list argument_pointer,
    int *assignment_target_type) {
  if (*assignment_target_type == 0) {
    *va_arg(argument_pointer, unsigned int *) = (unsigned int)*result;
  } else if (*assignment_target_type == 1) {
    *va_arg(argument_pointer, unsigned char *) = (unsigned char)*result;
  } else if (*assignment_target_type == 2) {
    *va_arg(argument_pointer, unsigned short int *) =
        (unsigned short int)*result;
  } else if (*assignment_target_type == 3) {
    *va_arg(argument_pointer, unsigned long int *) = (unsigned long int)*result;
  } else if (*assignment_target_type == 4) {
    *va_arg(argument_pointer, unsigned long long int *) =
        (unsigned long long int)*result;
  }
  *assignment_target_type = 0;
}
/**
 * @brief Detects and handles "NaN" or "Infinity" in a string.
 *
 * @param str Pointer to current position in the input string, updated to skip
 * recognized patterns.
 * @param res Pointer to a long double to store detected value (NaN or
 * infinity).
 * @param width Pointer to remaining width of the input string, restricting
 * recognized patterns.
 * @param sign Pointer to sign of the number: -1 if negative, 1 if positive.
 * @return 1 if "NaN" or "Infinity" is detected, otherwise 0.
 */
int s21_detect_and_handle_infinity_nan(char **str, long double *res, int *width,
                                       int *sign) {
  char temp[20] = {0};
  int check = 1;
  s21_strncpy(temp, *str, 10);
  s21_str_tolower(temp);

  if (*width > 2 && !s21_strncmp(temp, "nan", 3)) {
    *res = 0.0 / 0.0;
    *str += 3;
    *width -= 3;
  } else if (*width > 7 && !s21_strncmp(temp, "infinity", 8)) {
    *res = 1.0 / 0.0;
    *str += 8;
    *width -= 8;
  } else if (*width > 2 && !s21_strncmp(temp, "inf", 3)) {
    *res = 1.0 / 0.0;
    *str += 3;
    *width -= 3;
  } else {
    check = 0;
  }
  if (check) *res *= *sign;

  return check;
}
/**
 * @brief Converts a string to lowercase.
 *
 * @param str Pointer to the string to be converted to lowercase.
 */
void s21_str_tolower(char *str) {
  for (int i = 0; *(str + i); i++) {
    if (*(str + i) >= 65 && *(str + i) <= 90) *(str + i) += 32;
  }
}
/**
 * @brief Handles s21_asterisk ('*') characters in a string and updates a
 * counter.
 *
 * @param temp Pointer to current position in the string, updated to skip '*'
 * characters.
 * @param missing_specs_count Pointer to a counter set to 1 if '*' is found.
 */

void s21_asterisk(char **temp, int *missing_specs_count) {
  if (**temp == '*') *missing_specs_count = 1;
  while (**temp == '*') (*temp)++;
}
/**
 * @brief Checks if a character represents a s21_digit ('0'-'9').
 *
 * @param c ASCII value of the character to check.
 * @return 1 if `c` is within the range of '0'-'9', otherwise returns 0.
 */
int s21_digit(int c) { return (c >= 48 && c <= 57) ? 1 : 0; }
/**
 * @brief Checks if a character a hexadecimal s21_letter ('A/a'-'F/f').
 *
 * @param c ASCII value of the character to check.
 * @return 1 if `c` is within the range of 'A'-'F' or 'a'-'f'.
 */
int s21_letter(int c) {
  return ((c >= 65 && c <= 70) || (c >= 97 && c <= 102)) ? 1 : 0;
}
/**
 * @brief Checks if a character represents the ASCII value of 'X' or 'x'.
 *
 * @param c ASCII value of the character to check.
 * @return 1 if `c` is 88 (for 'X') or 120 (for 'x'), otherwise returns 0.
 */
int s21_is_x(int c) { return ((c == 88) || (c == 120)) ? 1 : 0; }
/**
 * @brief Checks if a character is 'E' or 'e'.
 *
 * @param c The ASCII value of the character to check.
 * @return 1 if `c` is 'E' or 'e', 0 otherwise.
 */
int s21_is_e(int c) { return ((c == 69) || (c == 101)) ? 1 : 0; }
/**
 * @brief Parses and handles base prefixes in a string.
 *
 * @param str Pointer to current position in the input string, updated to skip
 * base prefix.
 * @param i Pointer to the number of characters consumed by the prefix, updated
 * if a prefix is detected.
 * @param width Remaining width of the input string to consider for the prefix.
 * @param base Specifies the base (e.g., '0x' for hex) to determine valid
 * prefixes.
 */
void s21_parse_and_handle_base(char **str, int *i, int width, int base) {
  if (**str == '0' && width > 0) {
    *i = 1;
    (*str)++;
    if (s21_is_x(**str) && width - *i > 0 && base != 8) {
      *i = 2;
      if (width - *i > 0 && (s21_letter(*(*str + 1)) || s21_digit(*(*str + 1))))
        (*str)++;
    }
  }
}
/**
 * @brief Processes a sign character ('+' or '-') at the beginning of a string.
 *
 * @param str Pointer to current position in the input string, updated to skip
 * sign.
 * @param width Pointer to remaining width of the input string, decremented if
 * sign is processed.
 * @param sign Pointer to store sign of the number: -1 if negative, 1 if
 * positive.
 */
void s21_process_sign_character_in_input(char **str, int *width, int *sign) {
  if (**str == '-') {
    *sign = -1;
    (*str)++;
    (*width)--;
  } else if (**str == '+') {
    (*str)++;
    (*width)--;
    *sign = 1;
  }
}