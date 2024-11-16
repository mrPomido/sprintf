/**
 * @file s21_sprintf.c
 * @brief Implementation of formatted string output function similar to sprintf.
 *
 * This file defines the function s21_sprintf, which formats a string according
 * to a format string and variable arguments, similar to the standard sprintf
 * function.
 *
 * Function Overview:
 * - s21_sprintf: Formats a string based on the provided format string and
 * variable arguments.
 *
 * Inside s21_sprintf:
 * - Initializes formatting options and variables using s21_initialize_options.
 * - Processes each character of the format string:
 *   - Copies non-format characters directly to the output buffer.
 *   - Handles format specifiers by parsing options, extracting arguments,
 *     and formatting them using s21_process_format_specifier.
 * - Returns the total number of characters written to the buffer, excluding the
 * null-terminator.
 *
 * This function ensures handling of format specifiers (%d, %f, %s, etc.),
 * manages memory allocation, and handles various edge cases typical of
 * formatted string functions. It is designed to be used in applications where
 * custom formatted string output is required, offering flexibility and
 * efficiency similar to the standard sprintf function.
 *
 *  @note All functions are implemented to closely mimic their standard library
 * counterparts, with necessary adjustments and additional functionalities where
 * needed.
 */
#include "s21_sprintf.h"
/**
 * @brief Formats a string and writes the result to the buffer 'str'
 *
 * @param str Pointer to the buffer where the formatted string will be stored
 * @param format Pointer to the format string that specifies how to format the
 * data
 * @param ... Variable arguments to be formatted according to the format string
 * @return int The number of characters written to the buffer, excluding the
 * null-terminator
 */
int s21_sprintf(char *str, const char *format, ...) {
  int n = 0;
  opt options;
  va_list var_arg;
  var variables = {0};
  va_start(var_arg, format);
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      n += 1;
    } else {
      s21_initialize_options(&options);
      s21_options(&format, &options, var_arg);
      s21_process_format_specifier(&str, options, var_arg, &n, &variables);
    }
  }
  va_end(var_arg);
  *str = '\0';
  return n;
}
// __Initialization__
/**
 * @brief Initializes the format options structure with default values
 *
 * @param options Pointer to the options structure to be initialized
 */
void s21_initialize_options(opt *options) {
  options->flags = (flag_type){0};
  options->min_width = -1;
  options->precision = -1;
  options->length_spec = 0;
  options->format_spec = 0;
}
// __Options__
/**
 * @brief Extracts and sets the formatting options from the format string
 *
 * @param format Pointer to the format string (updated as options are read)
 * @param options Pointer to the options structure to store the extracted
 * options
 * @param var_arg Variable argument list containing the additional arguments
 */
void s21_options(const char **format, opt *options, va_list var_arg) {
  s21_set_format_flags(format, options);
  s21_min_width(format, options, var_arg);
  s21_precision(format, options, var_arg);
  s21_length_spec(format, options);
  s21_format_spec(format, options);
}
/**
 * @brief Processes flags in the format string and sets corresponding flags in
 * options.
 *
 * @param format Pointer to the format string (updated to skip processed flags)
 * @param options Pointer to the options struct where flags are set
 */
void s21_set_format_flags(const char **format, opt *options) {
  int next = 1;
  while (next && *(*format + 1)) {
    *format += 1;
    switch (**format) {
      case '-':
        options->flags.MINUS = 1;
        break;
      case '+':
        options->flags.PLUS = 1;
        break;
      case '0':
        options->flags.ZERO = 1;
        break;
      case '#':
        options->flags.SHARP = 1;
        break;
      case ' ':
        options->flags.SPACE = 1;
        break;
      default:
        next = 0;
    }
  }
}
/**
 * @brief Processes minimum width field in the format string and sets
 * corresponding value in options.
 *
 * @param format Pointer to the format string (updated to skip processed width
 * field)
 * @param options Pointer to the options struct where min_width is set
 * @param var_arg va_list for retrieving arguments (used when width is specified
 * as '*')
 */
void s21_min_width(const char **format, opt *options, va_list var_arg) {
  int min_width = -1;
  if ((**format > 47) && (**format < 58)) {
    min_width = s21_atoi(format);
  } else if (**format == '*') {
    min_width = va_arg(var_arg, int);
    if (min_width < 0) {
      min_width *= -1;
      options->flags.MINUS = 1;
    }
    *format += 1;
  }
  options->min_width = min_width;
}
/**
 * @brief Processes precision field in the format string and sets corresponding
 * value in options.
 *
 * @param format Pointer to the format string (updated to skip processed
 * precision field)
 * @param options Pointer to the options struct where precision is set
 * @param var_arg va_list for retrieving arguments (used when precision is
 * specified as '*')
 */
void s21_precision(const char **format, opt *options, va_list var_arg) {
  int precision = -1;
  if ((**format == '.') && *(*format + 1)) {
    *format += 1;
    if ((**format > 47) && (**format < 58)) {
      precision = s21_atoi(format);
    } else if (**format == '*') {
      precision = va_arg(var_arg, int);
      *format += 1;
    } else {
      precision = 0;
    }
  }
  options->precision = precision;
}
/**
 * @brief Processes length specifier in the format string and sets corresponding
 * value in options.
 *
 * @param format Pointer to the format string (updated to skip processed length
 * specifier)
 * @param options Pointer to the options struct where length specifier is set
 */
void s21_length_spec(const char **format, opt *options) {
  char specifiers[] = "hlL";
  char *spec_ptr = s21_NULL;
  if (**format) {
    spec_ptr = s21_strchr(specifiers, **format);
    if (spec_ptr) {
      options->length_spec = (spec_ptr - specifiers + 1);
      *format += 1;
    }
  }
}
/**
 * @brief Processes format specifier in the format string and sets corresponding
 * value in options.
 *
 * @param format Pointer to the format string (updated to skip processed format
 * specifier)
 * @param options Pointer to the options struct where format specifier is set
 */
void s21_format_spec(const char **format, opt *options) {
  char specifiers[] = "cdifsu%gGeExXonp";
  char *spec_ptr = s21_NULL;
  if (**format) {
    spec_ptr = s21_strchr(specifiers, **format);
    if (spec_ptr) {
      options->format_spec = (spec_ptr - specifiers + 1);
      *format += 1;
    }
  }
}
// __Additional__
/**
 * @brief Converts a string of digits to an integer.
 *
 * @param str Pointer to a pointer to the string to convert (updated to skip
 * processed digits).
 * @return The integer value represented by the string of digits.
 */
int s21_atoi(const char **str) {
  int res = 0;
  while ((**str > 47) && (**str < 58)) {
    res = res * 10 + (int)**str - 48;
    *str += 1;
  }
  return res;
}
// __Process__
/**
 * @brief Processes the format specifier and updates the output string and
 * character count
 *
 * @param str Pointer to the output string (updated as formatted output is
 * written)
 * @param options Struct containing the parsed format options
 * @param var_arg Variable argument list containing the additional arguments
 * @param n Pointer to the character count (updated as formatted output is
 * written)
 * @param variables Struct containing additional variables used in formatting
 */
void s21_process_format_specifier(char **str, opt options, va_list var_arg,
                                  int *n, var *variables) {
  if (options.format_spec == CHAR_SPECIFIER) {
    *n += s21_c_specifier(str, options, var_arg, variables);
  } else if (options.format_spec == STRING_SPECIFIER) {
    *n += s21_s_specifier(str, options, var_arg, variables);
  } else if (s21_is_spec_int(options.format_spec)) {
    *n += s21_int_specifiers(str, options, var_arg, variables);
  } else if (s21_is_spec_float(options.format_spec)) {
    long double double_var = 0L;
    double_var = s21_double_variable(options, var_arg);
    if (options.format_spec == FLOAT_SPECIFIER) {
      *n += s21_f_specifier(str, options, double_var, variables);
    } else if (options.format_spec == FLOAT_EXP_LOW_SPECIFIER ||
               options.format_spec == FLOAT_EXP_UP_SPECIFIER) {
      *n += s21_e_specifiers(str, options, double_var, variables);
    } else {
      *n += s21_g_specifiers(str, &options, double_var, variables);
    }
  } else if (options.format_spec == PERCENT_SPECIFIER) {
    *n += s21_perc_specifier(str, options, variables);
  } else if (options.format_spec == COUNT_SPECIFIER) {
    s21_n_specifier(options, var_arg, *n);
  } else {
    **str = '\0';
  }
}
/**
 * @brief Handles the %c format specifier for character in sprintf function.
 *
 * @param str Pointer to the destination string where the formatted output is
 * stored.
 * @param options Formatting options (flags, width, precision, etc.).
 * @param var_arg Arguments list containing the character argument.
 * @param variables Struct to hold intermediate values during formatting.
 * @return Number of characters written to the destination string.
 */
int s21_c_specifier(char **str, opt options, va_list var_arg, var *variables) {
  variables->char_buffer = calloc(2, sizeof(*variables->char_buffer));
  s21_mem_check(variables->char_buffer, variables);
  s21_handle_char_specifier(options, var_arg, variables);
  s21_apply_width(&(variables->char_buffer), options, variables);
  return (s21_buffer_to_string(str, variables));
}
/**
 * @brief Handles the %s format specifier for string in sprintf function.
 *
 * @param str Pointer to the destination string where the formatted output is
 * stored.
 * @param options Formatting options (flags, width, precision, etc.).
 * @param var_arg Arguments list containing the string argument.
 * @param variables Struct to hold intermediate values during formatting.
 * @return Number of characters written to the destination string.
 */
int s21_s_specifier(char **str, opt options, va_list var_arg, var *variables) {
  s21_handle_format_specifier(options, var_arg, variables);
  s21_apply_width(&(variables->char_buffer), options, variables);
  return (s21_buffer_to_string(str, variables));
}
/**
 * @brief Checks if the given specifier type is an integer specifier.
 *
 * @param spec The specifier type to check.
 * @return 1 if the specifier is an integer specifier, otherwise 0.
 */
int s21_is_spec_int(specifier_type spec) {
  int res = 0;
  if (spec == INT_HEX_SPECIFIER || spec == INT_DEC_SPECIFIER ||
      spec == UNSIGNED_SPECIFIER || spec == OCTAL_SPECIFIER ||
      spec == HEX_LOW_SPECIFIER || spec == HEX_UP_SPECIFIER ||
      spec == POINTER_SPECIFIER) {
    res = 1;
  }
  return res;
}
/**
 * @brief Handles integer specifiers in formatted output.
 *
 * @param str Pointer to the output string.
 * @param options Formatting options.
 * @param var_arg Variable argument list.
 * @param variables Additional variables for tracking.
 * @return Length of the formatted string.
 */
int s21_int_specifiers(char **str, opt options, va_list var_arg,
                       var *variables) {
  long unsigned u_var = 0;
  int len = 0, is_negative = 0, notation = 10;
  char *buf = s21_NULL;
  char sign = '\0';
  u_var = s21_unsigned_variable(options, var_arg, &is_negative);
  if (options.format_spec == OCTAL_SPECIFIER) {
    notation = 8;
  } else if (options.format_spec == HEX_LOW_SPECIFIER ||
             options.format_spec == HEX_UP_SPECIFIER ||
             options.format_spec == POINTER_SPECIFIER) {
    notation = 16;
  }
  s21_char_sign(is_negative, &sign, options);
  buf = s21_unsigned_to_str(u_var, notation,
                            options.format_spec == HEX_UP_SPECIFIER);
  if (options.precision != -1) {
    s21_apply_num_precision(&buf, options.precision);
  } else if (options.flags.ZERO && !options.flags.MINUS &&
             options.min_width > 0) {
    s21_apply_num_precision(&buf, options.min_width - (sign != '\0'));
  }
  if (u_var != 0 || options.format_spec == POINTER_SPECIFIER ||
      (u_var == 0 && options.format_spec == OCTAL_SPECIFIER &&
       options.precision == 0)) {
    s21_add_notation(&buf, options);
  }
  s21_add_sign(&buf, sign);
  if (options.flags.MINUS || !options.flags.ZERO) {
    s21_apply_width(&buf, options, variables);
  }
  if (buf) {
    variables->char_buffer = buf;
    len = s21_buffer_to_string(str, variables);
  }
  return len;
}
/**
 * @brief Checks if the specifier type is a floating point specifier.
 *
 * @param spec The specifier type to check.
 * @return 1 if the specifier is a floating point specifier, otherwise 0.
 */
int s21_is_spec_float(specifier_type spec) {
  int res = 0;
  if (spec == FLOAT_SPECIFIER || spec == FLOAT_EXP_LOW_SPECIFIER ||
      spec == FLOAT_EXP_UP_SPECIFIER || spec == EXP_LOW_SPECIFIER ||
      spec == EXP_UP_SPECIFIER) {
    res = 1;
  }
  return res;
}
/**
 * @brief Extracts a double or long double variable from va_list based on length
 * specifier.
 *
 * @param options The format options containing length specifier.
 * @param var_arg The va_list containing the variable arguments.
 * @return The extracted double or long double variable.
 */
long double s21_double_variable(opt options, va_list var_arg) {
  long double double_var = 0;
  if (options.length_spec == LONG_UPPERCASE_LEN_SPECIFIER) {
    double_var = va_arg(var_arg, long double);
  } else {
    double_var = va_arg(var_arg, double);
  }
  return double_var;
}
/**
 * @brief Handles the %f specifier for floating-point numbers in the custom
 * printf function.
 *
 * @param str Pointer to the output string.
 * @param options Format options containing flags, width, precision, etc.
 * @param double_var The floating-point variable to format.
 * @param variables Structure holding temporary variables and buffers.
 * @return The number of characters written to the output string.
 */
int s21_f_specifier(char **str, opt options, long double double_var,
                    var *variables) {
  int is_negative = 1, len = 0;
  long double tens = 0.;  // tens - старшие разряды числа (десятки и выше),
  long double ones = 0.;  // ones - младшие (единицы и ниже)
  char *ones_buf = s21_NULL, *tens_buf = s21_NULL, *buf = s21_NULL;
  char sign = '\0';
  is_negative = (double_var < 0) ? -1 : 1;
  s21_char_sign(is_negative, &sign, options);
  double_var = double_var * is_negative;
  if (double_var <= LDBL_MAX) {
    int next_digit = 0;
    s21_split_float(double_var, &ones, &tens);
    tens_buf = s21_float_to_str(tens);
    ones_buf = s21_mantissa_to_str(ones, &next_digit, options);
    buf = s21_sum_and_free_strings(tens_buf, ones_buf);
    s21_math_rounding(&buf, next_digit, s21_NULL, s21_NULL);
    s21_delete_trailing_zeros(&buf, options);
    if (options.flags.ZERO && !options.flags.MINUS && options.min_width > 0) {
      s21_apply_num_precision(&buf, options.min_width - (sign != '\0'));
    }
  } else {
    buf = s21_nan_inf(double_var, &sign, options.format_spec);
  }
  s21_add_sign(&buf, sign);
  if (options.flags.MINUS || !options.flags.ZERO) {
    s21_apply_width(&buf, options, variables);
  }
  if (buf) {
    variables->char_buffer = buf;
    len = s21_buffer_to_string(str, variables);
  }
  return len;
}
/**
 * @brief Handles the %e and %E specifiers for scientific notation in the custom
 * printf function.
 *
 * @param str Pointer to the output string.
 * @param options Format options containing flags, width, precision, etc.
 * @param double_var The floating-point variable to format.
 * @param variables Structure holding temporary variables and buffers.
 * @return The number of characters written to the output string.
 */
int s21_e_specifiers(char **str, opt options, long double double_var,
                     var *variables) {
  int is_negative = 1, len = 0;
  char *buf = s21_NULL, sign = '\0';
  is_negative = (double_var < 0) ? -1 : 1;
  s21_char_sign(is_negative, &sign, options);
  double_var = double_var * is_negative;
  if ((double_var <= LDBL_MAX &&
       options.length_spec == LONG_UPPERCASE_LEN_SPECIFIER) ||
      (double_var <= DBL_MAX &&
       options.length_spec != LONG_UPPERCASE_LEN_SPECIFIER)) {
    unsigned u_exponent = 0;
    long double mantissa = 0.;
    int next_digit = 0;
    char *mant_buf = s21_NULL, *exp_buf = s21_NULL, exp_sign = '\0',
         e_char = '\0';
    e_char = (options.format_spec == FLOAT_EXP_UP_SPECIFIER ||
              options.format_spec == EXP_UP_SPECIFIER)
                 ? 'E'
                 : 'e';
    mantissa = double_var;
    exp_sign = (mantissa < 1. && mantissa >= LDBL_TRUE_MIN) ? '-' : '+';
    u_exponent = s21_exponent(&mantissa);
    mant_buf = s21_mantissa_to_str(mantissa, &next_digit, options);
    s21_math_rounding(&mant_buf, next_digit, &exp_sign, &u_exponent);
    s21_delete_trailing_zeros(&mant_buf, options);
    exp_buf = s21_unsigned_to_str(u_exponent, 10, 0);
    s21_apply_num_precision(&exp_buf, 2);
    s21_add_sign(&exp_buf, exp_sign);
    s21_add_sign(&exp_buf, e_char);
    buf = s21_sum_and_free_strings(mant_buf, exp_buf);
    if (options.flags.ZERO && !options.flags.MINUS && options.min_width > 0) {
      s21_apply_num_precision(&buf, options.min_width - (sign != '\0'));
    }
  } else {
    buf = s21_nan_inf(double_var, &sign, options.format_spec);
  }
  s21_add_sign(&buf, sign);
  if (options.flags.MINUS || !options.flags.ZERO) {
    s21_apply_width(&buf, options, variables);
  }
  if (buf) {
    variables->char_buffer = buf;
    len = s21_buffer_to_string(str, variables);
  }
  return len;
}
/**
 * @brief Handles the %g and %G specifiers for formatting floating-point numbers
 *        in either fixed-point or scientific notation, based on the value and
 * precision.
 *
 * @param str Pointer to the output string.
 * @param options Pointer to the format options containing flags, width,
 * precision, etc.
 * @param double_var The floating-point variable to format.
 * @param variables Structure holding temporary variables and buffers.
 * @return The number of characters written to the output string.
 */
int s21_g_specifiers(char **str, opt *options, long double double_var,
                     var *variables) {
  int len = 0, exp_check = 0;
  long double double_var_temporary_buffer = 0;

  double_var_temporary_buffer = (double_var >= 0) ? double_var : -double_var;
  exp_check = s21_exponent(&double_var_temporary_buffer);
  double_var_temporary_buffer = fabsl(double_var);
  exp_check *= (double_var_temporary_buffer < 1. &&
                double_var_temporary_buffer >= LDBL_TRUE_MIN)
                   ? -1
                   : 1;

  if (options->precision < 0) {
    options->precision = 6;
  } else if (options->precision == 0) {
    options->precision = 1;
  }
  if ((-4 <= exp_check) && (exp_check < options->precision)) {
    options->precision = options->precision - 1 - exp_check;
    len = s21_f_specifier(str, *options, double_var, variables);

  } else {
    options->precision = options->precision - 1;
    len = s21_e_specifiers(str, *options, double_var, variables);
  }
  return len;
}
/**
 * @brief Handles the % specifier for formatting a percent sign in the output
 * string.
 *
 * @param str Pointer to the output string.
 * @param options The format options containing flags, width, precision, etc.
 * @param variables Structure holding temporary variables and buffers.
 * @return The number of characters written to the output string.
 */
int s21_perc_specifier(char **str, opt options, var *variables) {
  char *buf = s21_NULL;
  int len = 0;
  buf = (char *)malloc(sizeof(char) * 3);
  if (buf) {
    buf[0] = '\0';
    s21_strcat(buf, "%");
    if (options.flags.ZERO && !options.flags.MINUS && options.min_width > 0) {
      s21_apply_num_precision(&buf, options.min_width);
    }
    if (options.flags.MINUS || !options.flags.ZERO) {
      s21_apply_width(&buf, options, variables);
    }
    if (buf) {
      variables->char_buffer = buf;
      len = s21_buffer_to_string(str, variables);
    }
  }
  return len;
}
/**
 * @brief Handles the %n specifier for writing the number of characters written
 * so far.
 *
 * @param options The format options containing length specifier.
 * @param var_arg The va_list containing the variable arguments.
 * @param n The number of characters written so far.
 */
void s21_n_specifier(opt options, va_list var_arg, long int n) {
  if (options.length_spec == SHORT_LEN_SPECIFIER) {
    short int *variable = s21_NULL;
    variable = va_arg(var_arg, short int *);
    *variable = n;
  } else if (options.length_spec == LONG_LOWERCASE_LEN_SPECIFIER) {
    long int *variable = s21_NULL;
    variable = va_arg(var_arg, long int *);
    *variable = n;
  } else {
    int *variable = s21_NULL;
    variable = va_arg(var_arg, int *);
    *variable = n;
  }
}
// __Additional__
/**
 * @brief Checks for memory allocation errors and handles them.
 *
 * @param str The pointer to the allocated memory.
 * @param variables The structure containing program variables and error flag.
 */
void s21_mem_check(char *str, var *variables) {
  if (str == s21_NULL) {
    variables->error_flag = 1;
    s21_free_at_exit(variables);
    exit(1);
  }
}
/**
 * @brief Frees allocated memory at program exit.
 *
 * @param variables The structure containing program variables and allocated
 * memory pointers.
 */
void s21_free_at_exit(var *variables) {
  if (variables->char_buffer) {
    free(variables->char_buffer);
    variables->char_buffer = s21_NULL;
  }
  if (variables->temporary_buffer) {
    free(variables->temporary_buffer);
    variables->temporary_buffer = s21_NULL;
  }
  if (variables->char_buffer) {
    free(variables->char_buffer);
    variables->temporary_buffer = s21_NULL;
  }
}
/**
 * @brief Applies minimum width formatting to a buffer string.
 *
 * @param buf The buffer string to be formatted.
 * @param options The format options containing minimum width and flags.
 * @param variables The structure containing temporary buffers for memory
 * management.
 */
void s21_apply_width(char **buf, opt options, var *variables) {
  if (*buf && ((int)s21_strlen(*buf) < options.min_width)) {
    char filler = ' ';
    int n_fillers = 0;
    n_fillers = options.min_width - s21_strlen(*buf);
    if (!options.flags.MINUS && options.flags.ZERO) {
      filler = ' ';
    }
    variables->temporary_buffer = calloc((options.min_width + 2), sizeof(char));
    s21_mem_check(variables->temporary_buffer, variables);
    for (int i = 0; i < n_fillers; i++) {
      (variables->temporary_buffer)[i] = filler;
    }
    (variables->temporary_buffer)[n_fillers] = '\0';
    variables->temporary_pointer =
        (char *)realloc(*buf, sizeof(char) * (options.min_width + 2));
    s21_mem_check(variables->temporary_pointer, variables);
    if (variables->temporary_pointer) {
      *buf = variables->temporary_pointer;
      if (options.flags.MINUS) {
        s21_strcat(*buf, variables->temporary_buffer);
      } else {
        s21_strcat(variables->temporary_buffer, *buf);
        s21_strcpy(*buf, variables->temporary_buffer);
      }
    }
    free(variables->temporary_buffer);
    variables->temporary_buffer = s21_NULL;
  }
}
/**
 * @brief Handles the conversion and formatting for the %c specifier.
 *
 * @param options The format options containing length specifier.
 * @param var_arg The va_list containing the variable arguments.
 * @param variables The structure containing the char_buffer for storing
 * formatted output.
 */
void s21_handle_char_specifier(opt options, va_list var_arg, var *variables) {
  if (options.length_spec == LONG_LOWERCASE_LEN_SPECIFIER) {
    wchar_t wchar = va_arg(var_arg, wchar_t);
    wchar_t wstr[2] = {0};
    wstr[0] = wchar;
    s21_concatenate_wide_chars(variables->char_buffer, wstr, 1);
  } else {
    char sym = (char)va_arg(var_arg, int);
    (variables->char_buffer)[0] = sym;
    (variables->char_buffer)[1] = '\0';
  }
}
/**
 * @brief Handles the conversion and formatting for the %s specifier.
 *
 * @param options The format options containing length specifier.
 * @param var_arg The va_list containing the variable arguments.
 * @param variables The structure containing the char_buffer for storing
 * formatted output.
 */
void s21_handle_format_specifier(opt options, va_list var_arg, var *variables) {
  if (options.length_spec == LONG_LOWERCASE_LEN_SPECIFIER) {
    int wlen = 0;
    wchar_t *wstr = s21_NULL;
    wstr = va_arg(var_arg, wchar_t *);
    wlen = s21_wchar_string_length(wstr);
    s21_apply_precision_limit(&wlen, options);
    variables->char_buffer = calloc(wlen + 1, sizeof(*variables->char_buffer));
    s21_mem_check(variables->char_buffer, variables);
    s21_concatenate_wide_chars(variables->char_buffer, wstr, wlen);
  } else {
    int len = 0;
    char *Usstr = s21_NULL;
    Usstr = va_arg(var_arg, char *);
    len = s21_strlen(Usstr);
    s21_apply_precision_limit(&len, options);
    variables->char_buffer = calloc(len + 1, sizeof(*variables->char_buffer));
    s21_mem_check(variables->char_buffer, variables);
    for (int i = 0; i < len; i++) {
      (variables->char_buffer)[i] = Usstr[i];
    }
  }
}
/**
 * @brief Calculates the length of a wide string (wchar_t *) excluding the
 * null-terminator.
 *
 * @param wstr The wide string for which length needs to be calculated.
 * @return The length of the wide string, excluding the null-terminator.
 */
size_t s21_wchar_string_length(wchar_t *wstr) {
  wchar_t *p = s21_NULL;
  p = wstr;
  for (; *p; p++)
    ;
  return p - wstr;
}
/**
 * @brief Copies wide characters from wchar_t string to the end of a char
 * string.
 *
 * @param str The destination char string where wide characters are copied.
 * @param wstr The source wchar_t string containing wide characters to copy.
 * @param len The number of wide characters to copy from wstr to str.
 */
void s21_concatenate_wide_chars(char *str, wchar_t *wstr, int len) {
  int buflen = 0;
  char *p = s21_NULL;
  buflen = s21_strlen(str);
  p = str + buflen;
  for (int i = 0; i < len; i++) {
    *(p++) = (char)*(wstr++);
  }
  *p = '\0';
}
/**
 * @brief Adjusts the length of a string based on precision settings.
 *
 * If the length of the string (*wlen) exceeds the precision specified in
 * options and options.precision is greater than 0, *wlen is adjusted to match
 * options.precision. If options.precision is 0, *wlen is set to 0, effectively
 * truncating the string length.
 *
 * @param wlen Pointer to the length of the string to adjust.
 * @param options The format options specifying the precision.
 */
void s21_apply_precision_limit(int *wlen, opt options) {
  if (((*wlen) > options.precision) && (options.precision > 0)) {
    (*wlen) = options.precision;
  } else if (options.precision == 0) {
    *wlen = 0;
  }
}
/**
 * @brief Retrieves an unsigned integer variable from va_list based on format
 * options.
 *
 * Depending on the format specifier and length specifier in options, retrieves
 * the corresponding unsigned integer variable from va_list var_arg. Updates the
 * is_negative flag if applicable.
 *
 * @param options The format options specifying the format and length
 * specifiers.
 * @param var_arg The va_list containing the variable arguments.
 * @param is_negative Pointer to an integer flag indicating if the variable is
 * negative.
 * @return The unsigned integer variable extracted from var_arg.
 */
long unsigned s21_unsigned_variable(opt options, va_list var_arg,
                                    int *is_negative) {
  long unsigned u_var = 0;
  if (options.format_spec == INT_DEC_SPECIFIER ||
      options.format_spec == INT_HEX_SPECIFIER) {
    long int int_var = 0;
    if (options.length_spec == LONG_LOWERCASE_LEN_SPECIFIER) {
      int_var = va_arg(var_arg, long int);
      u_var = labs(int_var);
    } else if (options.length_spec == SHORT_LEN_SPECIFIER) {
      int_var = (short)va_arg(var_arg, int);
      u_var = labs(int_var);
    } else {
      int_var = va_arg(var_arg, int);
      u_var = labs(int_var);
    }
    *is_negative = (int_var < 0) ? -1 : 1;
  } else if (options.format_spec == UNSIGNED_SPECIFIER ||
             options.format_spec == OCTAL_SPECIFIER ||
             options.format_spec == HEX_LOW_SPECIFIER ||
             options.format_spec == HEX_UP_SPECIFIER) {
    if (options.length_spec == LONG_LOWERCASE_LEN_SPECIFIER) {
      u_var = (unsigned long int)va_arg(var_arg, unsigned long int);
    } else if (options.length_spec == SHORT_LEN_SPECIFIER) {
      u_var = (unsigned short int)va_arg(var_arg, unsigned int);
    } else {
      u_var = (unsigned int)va_arg(var_arg, unsigned int);
    }
  } else if (options.format_spec == POINTER_SPECIFIER) {
    void *ptr = s21_NULL;
    ptr = va_arg(var_arg, void *);
    u_var = (unsigned long)ptr;
  }
  return u_var;
}
/**
 * @brief Determines the sign character for a formatted output based on
 * formatting options.
 *
 * @param is_negative Integer indicating if the value is negative (-1) or
 * positive (1).
 * @param sign Pointer to a character where the determined sign character will
 * be stored.
 * @param options The format options specifying the format specifier and
 * formatting flags.
 */
void s21_char_sign(int is_negative, char *sign, opt options) {
  if (options.format_spec != UNSIGNED_SPECIFIER &&
      options.format_spec != OCTAL_SPECIFIER &&
      options.format_spec != HEX_LOW_SPECIFIER &&
      options.format_spec != HEX_UP_SPECIFIER) {
    if (is_negative < 0) {
      *sign = '-';
    } else {
      if (options.flags.PLUS) {
        *sign = '+';
      } else if (options.flags.SPACE) {
        *sign = ' ';
      }
    }
  }
}
/**
 * @brief Splits a long double into its fractional and integer parts.
 *
 * @param double_var The long double value to split.
 * @param ones Pointer to a long double where the fractional part will be
 * stored.
 * @param tens Pointer to a long double where the integer part will be stored.
 */
void s21_split_float(long double double_var, long double *ones,
                     long double *tens) {
  *ones = fmodl(double_var, 10.L);
  *tens = (double_var - *ones) / 10.L;
}
/**
 * @brief Converts a long double number into a string representation.
 *
 * @param num The long double number to convert.
 * @return A dynamically allocated string containing the ASCII representation of
 * `num`. The caller is responsible for freeing this memory.
 */
char *s21_float_to_str(long double num) {
  char *ascii_str = s21_NULL, *reverse_str = s21_NULL;
  int buf_len = 0;
  int length = 0;
  long double tmp_num = 0;
  num *= (num < 0) ? -1 : 1;
  tmp_num = num;
  while (tmp_num > 1) {
    buf_len += 1;
    tmp_num /= 10;
  }
  buf_len = (buf_len > 0) ? buf_len : 2;
  ascii_str = (char *)calloc((buf_len + 5), sizeof(char));
  reverse_str = (char *)calloc((buf_len + 5), sizeof(char));
  if (ascii_str && reverse_str) {
    while (num >= 1L) {
      int digit = 0;
      digit = (int)fmodl(num, 10);
      num /= 10;
      reverse_str[length] = s21_convert_digit_to_char(digit, 0);
      length += 1;
    }
    reverse_str[length] = '\0';
    s21_invert_str(reverse_str, ascii_str);
  }
  if (reverse_str) {
    free(reverse_str);
  }
  return ascii_str;
}
/**
 * @brief Rounds the given numeric string based on the next digit and adjusts
 * for exponent notation.
 *
 * @param num_string Pointer to the numeric string to be rounded and possibly
 * modified.
 * @param next_digit The digit that determines whether rounding is necessary.
 * @param exp_sign Pointer to the sign of the exponent (if applicable).
 * @param u_exp Pointer to the exponent value (if applicable).
 */
void s21_math_rounding(char **num_string, int next_digit, char *exp_sign,
                       unsigned *u_exp) {
  if (*num_string && next_digit >= 5) {
    int not_rounded = 1, position = 0;
    position = s21_strlen(*num_string) - 1;
    while (position >= 0 && not_rounded) {
      if ((*num_string)[position] == '.') {
        position -= 1;
      }
      if ((*num_string)[position] == '9') {
        (*num_string)[position] = '0';
        position -= 1;
      } else {
        (*num_string)[position] = (char)((*num_string)[position] + 1);
        not_rounded = 0;
      }
    }
    if (position == -1) {
      char *tmp_ptr = s21_NULL, *tmp_array = s21_NULL;
      int num_string_len = 0;
      num_string_len = s21_strlen(*num_string);
      tmp_array = (char *)calloc(num_string_len + 1, sizeof(char));
      if (tmp_array) {
        s21_strcpy(tmp_array, *num_string);
        tmp_ptr =
            (char *)realloc(*num_string, sizeof(char) * (num_string_len + 5));
        if (tmp_ptr) {
          tmp_ptr[0] = '1';
          tmp_ptr[1] = '\0';
          s21_strcat(tmp_ptr, tmp_array);
          if (u_exp) {
            if (*exp_sign == '-') {
              *u_exp = *u_exp - 1;
              if (*u_exp == 0) {
                *exp_sign = '+';
              }
            } else {
              *u_exp = *u_exp + 1;
            }
            tmp_ptr[2] = tmp_ptr[1];
            tmp_ptr[1] = '.';
            tmp_ptr[s21_strlen(tmp_ptr) - 1] = '\0';
          }
          *num_string = tmp_ptr;
        }
        free(tmp_array);
      }
    }
  }
}
/**
 * @brief Deletes trailing zeros and decimal point from the numeric string if
 * specified conditions are met.
 *
 * @param num_string Pointer to the numeric string where trailing zeros and
 * decimal point need to be removed.
 * @param options Formatting options that determine the conditions under which
 * trailing zeros are deleted.
 */
void s21_delete_trailing_zeros(char **num_string, opt options) {
  if (*num_string && options.flags.SHARP == 0 &&
      (options.format_spec == EXP_UP_SPECIFIER ||
       options.format_spec == EXP_LOW_SPECIFIER)) {
    int position = 0;
    position = s21_strlen(*num_string) - 1;
    while ((position > 0) && ((*num_string)[position] == '0' ||
                              ((*num_string)[position] == '.'))) {
      (*num_string)[position] = '\0';
      position -= 1;
    }
  }
}
/**
 * @brief Generates a string representation for NaN (Not a Number) or Infinity
 * based on the given floating point variable.
 *
 * @param variable The floating point variable to check for NaN or Infinity.
 * @param sign Pointer to a character where the sign of the result (if any) will
 * be stored.
 * @param format_spec The specifier type indicating the format requested for NaN
 * or Infinity representation.
 * @return A dynamically allocated string buffer containing "NAN" or "INF", or
 * NULL if allocation fails.
 */
char *s21_nan_inf(long double variable, char *sign,
                  specifier_type format_spec) {
  char *buf = s21_NULL;
  buf = (char *)calloc(5, sizeof(char));
  if (buf) {
    if (variable != variable) {
      if (format_spec == EXP_UP_SPECIFIER ||
          format_spec == FLOAT_EXP_UP_SPECIFIER) {
        s21_strcpy(buf, "NAN");
      } else {
        s21_strcpy(buf, "nan");
      }
      *sign = '\0';
    } else {
      if (format_spec == EXP_UP_SPECIFIER ||
          format_spec == FLOAT_EXP_UP_SPECIFIER) {
        s21_strcpy(buf, "INF");
      } else {
        s21_strcpy(buf, "inf");
      }
    }
  }
  return buf;
}
/**
 * @brief Concatenates two strings and frees the memory allocated for them.
 *
 * @param string1 The first string to concatenate.
 * @param string2 The second string to concatenate.
 * @return A dynamically allocated string buffer containing the concatenated
 * `string1` and `string2`, or NULL if memory allocation fails.
 */
char *s21_sum_and_free_strings(char *string1, char *string2) {
  char *buf = s21_NULL;
  if (string1 && string2) {
    buf = (char *)calloc((s21_strlen(string1) + s21_strlen(string2) + 2),
                         sizeof(char));
    if (buf) {
      s21_strcat(buf, string1);
      s21_strcat(buf, string2);
    }
  }
  if (string1) {
    free(string1);
  }
  if (string2) {
    free(string2);
  }
  return buf;
}
/**
 * @brief Converts a digit to a character based on specified text case.
 *
 * @param digit The digit to convert (0-35).
 * @param text_case Determines whether to convert to lowercase (0) or uppercase
 * (non-zero).
 * @return The corresponding character representation of the digit.
 */
char s21_convert_digit_to_char(int digit, int text_case) {
  char ch = '\0';
  if (digit >= 0 && digit < 10) {
    ch = (char)(digit + 48);  // 0 - 9
  } else if (digit >= 10 && digit < 36) {
    if (text_case == 0) {
      ch = (char)(digit + 87);  // a - z
    } else {
      ch = (char)(digit + 55);  // A - Z
    }
  }
  return ch;
}
/**
 * @brief Converts an unsigned long integer to a string representation in the
 * specified notation.
 *
 * @param num The unsigned long integer to convert to string.
 * @param notation The notation (base) to use for the conversion (e.g., 10 for
 * decimal).
 * @param text_case Determines whether the output should be in uppercase (1) or
 * lowercase (0).
 * @return A dynamically allocated string buffer containing the converted string
 * representation of `num` in the specified `notation`, or NULL if memory
 * allocation fails.
 */
char *s21_unsigned_to_str(unsigned long int num, unsigned int notation,
                          int text_case) {
  char *ascii_str = s21_NULL, *reverse_str = s21_NULL;
  int length = 0;
  ascii_str = (char *)malloc(sizeof(char) * 50);
  reverse_str = (char *)malloc(sizeof(char) * 50);
  if (reverse_str && ascii_str) {
    if (num == 0) {
      reverse_str[0] = '0';
      length += 1;
    }
    while (num != 0) {
      int digit = num % notation;
      num /= notation;
      reverse_str[length] = s21_convert_digit_to_char(digit, text_case);
      length += 1;
    }
    reverse_str[length] = '\0';
    s21_invert_str(reverse_str, ascii_str);
  }
  if (reverse_str) {
    free(reverse_str);
  }
  return ascii_str;
}
/**
 * @brief Converts the mantissa of a long double number to a string
 * representation.
 *
 * @param num The long double number whose mantissa needs to be converted.
 * @param next_digit Pointer to an integer where the next significant digit will
 * be stored.
 * @param options Formatting options that include precision and flags.
 * @return A dynamically allocated string buffer containing the converted
 * mantissa in string representation, or NULL if memory allocation fails.
 */
char *s21_mantissa_to_str(long double num, int *next_digit, opt options) {
  char *ascii_str = s21_NULL;
  int length = 0, digit = 0, precision = -1;
  precision = (options.precision >= 0) ? options.precision : 6;

  num *= (num < 0) ? -1 : 1;
  digit = floorl(num);
  num = (num - digit) * 10;

  ascii_str = (char *)malloc(sizeof(char) * (precision + 5));
  if (ascii_str) {
    ascii_str[length++] = s21_convert_digit_to_char(digit, 0);
    if (precision > 0 || options.flags.SHARP) {
      ascii_str[length++] = '.';
    }
    for (int i = 0; i < precision; i++) {
      digit = (floorl(num) >= 0) ? (int)floorl(num) : 0;
      num = (num - digit) * 10;
      ascii_str[length++] = s21_convert_digit_to_char(digit, 0);
    }
    ascii_str[length] = '\0';
    *next_digit = (int)floorl(num);
  }
  return ascii_str;
}
/**
 * @brief Inverts the contents of a string and stores the result in another
 * string.
 *
 * @param origin The original string whose content will be inverted.
 * @param inverted The string where the inverted content will be stored.
 */
void s21_invert_str(char *origin, char *inverted) {
  if (origin && inverted) {
    int length = 0;
    length = s21_strlen(origin);
    for (int i = 0; i < length; i++) {
      inverted[i] = origin[length - 1 - i];
    }
    inverted[length] = '\0';
  }
}
/**
 * @brief Computes the exponent of a floating point number and normalizes the
 * mantissa.
 *
 * @param mantissa Pointer to the mantissa of the floating point number, which
 * will be normalized.
 * @return The absolute value of the exponent calculated during normalization.
 */
unsigned s21_exponent(long double *mantissa) {
  unsigned exponent = 0;
  while (*mantissa >= 10. && *mantissa <= LDBL_MAX) {
    *mantissa = *mantissa / 10.;
    exponent += 1;
  }
  while (*mantissa < 1. && *mantissa >= LDBL_TRUE_MIN) {
    *mantissa = *mantissa * 10.;
    exponent += 1;
  }
  return exponent;
}
/**
 * @brief Applies numerical precision to a string representation of a number.
 *
 * @param buf Pointer to a pointer to the string where precision will be
 * applied.
 * @param precision The desired precision to be applied.
 */
void s21_apply_num_precision(char **buf, int precision) {
  if (*buf) {
    int len_buf = 0;
    len_buf = s21_strlen(*buf);
    if ((precision == 0) && (s21_strcmp(*buf, "0") == 0)) {
      *buf[0] = '\0';
    } else if (len_buf < precision) {
      int n_zeros = 0;
      char *temporary_buffer = s21_NULL;
      n_zeros = precision - len_buf;
      temporary_buffer = (char *)malloc(sizeof(char) * (precision + 1));
      if (temporary_buffer) {
        char *tmp_ptr = s21_NULL;
        for (int i = 0; i < n_zeros; i++) {
          temporary_buffer[i] = '0';
        }
        temporary_buffer[n_zeros] = '\0';
        s21_strcat(temporary_buffer, *buf);
        tmp_ptr = (char *)realloc(*buf, sizeof(char) * (precision + 1));
        if (tmp_ptr) {
          *buf = tmp_ptr;
          s21_strcpy(*buf, temporary_buffer);
        }
        free(temporary_buffer);
      }
    }
  }
}
/**
 * @brief Adds a sign character to the beginning of a string.
 *
 * @param buf Pointer to a pointer to the string where the sign will be added.
 * @param sign The sign character to be added ('+', '-' or ' ').
 */
void s21_add_sign(char **buf, char sign) {
  if (sign && *buf) {
    int len_buf = 0;
    char *temporary_buffer = s21_NULL;
    len_buf = s21_strlen(*buf);
    temporary_buffer = (char *)malloc(sizeof(char) * (len_buf + 2));
    if (temporary_buffer) {
      char *tmp_ptr = s21_NULL;
      temporary_buffer[0] = sign;
      temporary_buffer[1] = '\0';
      s21_strcat(temporary_buffer, *buf);
      tmp_ptr = (char *)realloc(*buf, sizeof(char) * (len_buf + 2));
      if (tmp_ptr) {
        *buf = tmp_ptr;
        s21_strcpy(*buf, temporary_buffer);
      }
      free(temporary_buffer);
    }
  }
}
/**
 * @brief Adds notation prefixes (like "0x" for hexadecimal) to a string.
 *
 * @param buf Pointer to a pointer to the string where notation will be added.
 * @param options Formatting options that dictate which notation to add.
 */
void s21_add_notation(char **buf, opt options) {
  if (*buf &&
      ((options.flags.SHARP && (options.format_spec == OCTAL_SPECIFIER ||
                                options.format_spec == HEX_LOW_SPECIFIER ||
                                options.format_spec == HEX_UP_SPECIFIER)) ||
       options.format_spec == POINTER_SPECIFIER)) {
    int len_buf = 0;
    char *temporary_buffer = s21_NULL;
    len_buf = s21_strlen(*buf);
    temporary_buffer = (char *)malloc(sizeof(char) * (len_buf + 3));
    if (temporary_buffer) {
      char *tmp_ptr = s21_NULL;
      temporary_buffer[0] = '\0';
      if ((options.format_spec == OCTAL_SPECIFIER) && (**buf != '0')) {
        s21_strcat(temporary_buffer, "0");
      }
      if (options.format_spec == HEX_LOW_SPECIFIER ||
          options.format_spec == POINTER_SPECIFIER) {
        s21_strcat(temporary_buffer, "0x");
      }
      if (options.format_spec == HEX_UP_SPECIFIER) {
        s21_strcat(temporary_buffer, "0X");
      }
      s21_strcat(temporary_buffer, *buf);
      tmp_ptr = (char *)realloc(*buf, sizeof(char) * (len_buf + 3));
      if (tmp_ptr) {
        *buf = tmp_ptr;
        s21_strcpy(*buf, temporary_buffer);
      }
      free(temporary_buffer);
    }
  }
}
/**
 * @brief Records the content of variables->char_buffer into *str and frees the
 * buffer.
 *
 * @param str Pointer to a pointer where the content will be copied.
 * @param variables Pointer to the structure holding the char_buffer and other
 * variables.
 * @return Length of the recorded string.
 */
int s21_buffer_to_string(char **str, var *variables) {
  int length = 0;
  s21_strcpy(*str, variables->char_buffer);
  length = s21_strlen(variables->char_buffer);
  *str = *str + length;
  free(variables->char_buffer);
  variables->char_buffer = s21_NULL;
  return (length);
}