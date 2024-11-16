/**
 * @file s21_string.c
 * @brief Implementation of custom string manipulation and memory management
 * functions
 *
 * This file provides implementations of various string and memory manipulation
 * functions. These functions are analogous to the standard C library functions
 * and are designed to offer similar functionality. Each function is prefixed
 * with 's21_' to avoid naming conflicts with standard library functions.
 *
 * Usage:
 * To use these functions, include the "s21_string.h" header file in your source
 * code.
 *
 * @note All functions are implemented to closely mimic their standard library
 * counterparts, with necessary adjustments and additional functionalities where
 * needed.
 */
#include "s21_string.h"
// Copy functions
/**
 * @brief s21_memcpy Copies n bytes from memory area src to memory area dest
 *
 * @param dest Pointer to the destination array where the content is to be
 * copied
 * @param src Pointer to the source of data to be copied
 * @param n Number of bytes to copy
 * @return void* Returns a pointer to dest */
void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  s21_size_t i = 0;
  while (i < n) {
    *((unsigned char *)dest + i) = *((unsigned char *)src + i);
    i++;
  }
  return dest;
}
/**
 * @brief Fills the first n bytes of the memory area pointed to
 *        by str with the constant byte c
 *
 * @param str Pointer to the memory area to be filled
 * @param c The byte value to be set
 * @param n Number of bytes to be set to the value
 * @return void* Returns a pointer to the memory area str
 */
void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *strCop = (unsigned char *)str;
  s21_size_t i = 0;
  while (i < n) {
    strCop[i] = (unsigned char)c;
    i++;
  }
  return strCop;
}
/**
 * @brief Copies the string pointed to by src, including
 *        the terminating null byte ('\0'),
 *        to the buffer pointed to by dest.
 *
 * @param dest Pointer to the destination buffer where the content is to be
 * copied
 * @param src Pointer to the null-terminated string to be copied
 * @return char* Returns a pointer to dest
 */
char *s21_strcpy(char *dest, const char *src) {
  char *copy_dest = dest;
  for (; *src; src++, copy_dest++) {
    *copy_dest = *src;
  }
  *copy_dest = '\0';
  return dest;
}
/**
 * @brief Copies up to n characters from the string pointed to by src to dest.
 *        If the length of src is less than n, the remainder of dest will be
 * padded with null bytes.
 *
 * @param dest Pointer to the destination buffer where the content is to be
 * copied
 * @param src Pointer to the null-terminated string to be copied
 * @param n Maximum number of characters to be copied from src
 * @return char* Returns a pointer to dest
 */
char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *copy_dest = dest;
  if (n) {
    for (; *src && n; src++, copy_dest++, n--) {
      *copy_dest = *src;
    }
  }
  for (; n; n--, copy_dest++) {
    *copy_dest = '\0';
  }
  return dest;
}
// search functions
/**
 * @brief Scans the initial n bytes of the memory area pointed to by
 *        str for the first instance of the character c
 *
 * @param str Pointer to the memory area to be scanned
 * @param c The character to be searched for, interpreted as an unsigned char
 * @param n The number of bytes to be analyzed
 * @return void* Returns a pointer to the matching byte or NULL if the character
 * does not occur in the given memory area
 */
// void *s21_memchr(const void *str, int c, s21_size_t n) {
void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = s21_NULL;
  for (s21_size_t i = 0; i < n; i++) {
    if ((*((unsigned char *)str + i)) == (unsigned char)c) {
      result = (char *)str + i;
      i = n;
    }
  }
  return result;
}
/**
 * @brief Locates the first occurrence of the character c in the string pointed
 * to by str
 *
 * @param str Pointer to the null-terminated string to be analyzed
 * @param c The character to be located, interpreted as an unsigned char
 * @return char* Returns a pointer to the first occurrence of the character c in
 * the string, or NULL if the character is not found
 */
char *s21_strchr(const char *str, int c) {
  char *result = s21_NULL;
  while (*str && *str != (char)c) {
    str++;
  }
  if (*str == (char)c) {
    result = (char *)str;
  }
  return result;
}
/**
 * @brief Locates the first occurrence in the string str1 of any of the bytes in
 * the string str2
 *
 * @param str1 Pointer to the null-terminated string to be analyzed
 * @param str2 Pointer to the null-terminated string containing the bytes to
 * match
 * @return char* Returns a pointer to the byte in str1 that matches one of the
 * bytes in str2, or NULL if no such byte is found
 */
char *s21_strpbrk(const char *str1, const char *str2) {
  s21_size_t n = s21_strlen(str2);
  char *result = s21_NULL;
  while (*str1 && !result) {
    for (s21_size_t i = 0; i < n && !result; i++) {
      if (*str1 == str2[i]) {
        result = (char *)str1;
      }
    }
    ++str1;
  }
  return result;
}
/**
 * @brief Locates the last occurrence of the character c in the string pointed
 * to by str
 *
 * @param str Pointer to the null-terminated string to be analyzed
 * @param c The character to be located, interpreted as an unsigned char
 * @return char* Returns a pointer to the last occurrence of the character c in
 * the string, or NULL if the character is not found
 */
char *s21_strrchr(const char *str, int c) {
  char *result = s21_NULL;
  int i = 0;
  for (; *(str + i); i++) {
    if (*((unsigned char *)str + i) == c) result = (char *)(str + i);
  }
  if (c == 0 && *(str + i) == 0) result = (char *)(str + i);
  return result;
}
/**
 * @brief Finds the first occurrence of the substring needle in the string
 * haystack
 *
 * @param haystack Pointer to the null-terminated string to be scanned
 * @param needle Pointer to the null-terminated substring to be searched for
 * @return char* Returns a pointer to the beginning of the located substring,
 *         or NULL if the substring is not found
 */
char *s21_strstr(const char *haystack, const char *needle) {
  char *result = s21_NULL;
  s21_size_t haystack_len = s21_strlen(haystack);
  s21_size_t needle_len = s21_strlen(needle);
  s21_size_t haystack_search = 0;
  s21_size_t needle_search = 0;
  s21_size_t match_counter = 0;
  while (haystack_len >= needle_len &&
         haystack_len - haystack_search >= needle_len - match_counter &&
         match_counter != needle_len) {
    if (haystack[haystack_search] == needle[needle_search]) {
      ++match_counter;
      ++needle_search;
    } else {
      needle_search = 0;
      match_counter = (haystack[haystack_search] == needle[needle_search]);
    }
    ++haystack_search;
  }
  if (match_counter == needle_len) {
    result = (char *)haystack + haystack_search - needle_len;
  }
  return result;
}
// processing functions
/**
 * @brief Converts all lowercase letters in the string to uppercase
 *
 * @param str Pointer to the null-terminated string to be converted
 * @return void* Returns a pointer to the newly allocated string with all
 * lowercase letters converted to uppercase, or NULL if the input string is NULL
 */
void *s21_to_upper(const char *str) {
  char *result = s21_NULL;
  s21_size_t len = str ? s21_strlen(str) : 0;
  if (str) result = (char *)calloc((len + 1), sizeof(char));
  if (result && len) {
    for (s21_size_t i = 0; *(str + i); ++i)
      result[i] = (str[i] > 96 && str[i] < 123) ? str[i] - 32 : str[i];
  }
  return result;
}
/**
 * @brief Converts all uppercase letters in the string to lowercase
 *
 * @param str Pointer to the null-terminated string to be converted
 * @return void* Returns a pointer to the newly allocated string with all
 * uppercase letters converted to lowercase, or NULL if the input string is NULL
 */
void *s21_to_lower(const char *str) {
  char *result = s21_NULL;
  s21_size_t len = str ? s21_strlen(str) : 0;
  if (str) result = (char *)calloc((len + 1), sizeof(char));
  if (result && len) {
    for (s21_size_t i = 0; *(str + i); ++i)
      result[i] = (str[i] > 64 && str[i] < 91) ? str[i] + 32 : str[i];
  }
  return result;
}
/**
 * @brief Inserts one string into another at a specified index
 *
 * @param src Pointer to the null-terminated string where the insertion will
 * occur
 * @param str Pointer to the null-terminated string to be inserted
 * @param start_index Index in `src` where `str` will be inserted
 * @return void* Returns a pointer to the newly allocated string resulting from
 * the insertion, or NULL if memory allocation fails or start_index is out of
 * bounds
 */
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  s21_size_t len_src = 0, len_str = 0;
  if (src != s21_NULL)
    len_src = s21_strlen(src);
  else
    len_src = 0;
  if (str != s21_NULL)
    len_str = s21_strlen(str);
  else
    len_str = 0;
  s21_size_t len = len_src + len_str;
  if (start_index > len_src) return s21_NULL;
  char *res = (char *)calloc(len + 1, sizeof(char));
  if (res) {
    for (s21_size_t i = 0; i < len; i++) {
      if (i < start_index)
        res[i] = src[i];
      else if (i < start_index + len_str)
        res[i] = str[i - start_index];
      else
        res[i] = src[i - len_str];
    }
  }
  res[len_src + len_str] = '\0';
  return (void *)res;
}
/**
 * @brief Trims leading and trailing characters specified in 'trim_chars'
 from
 * the string 'src'
 *
 * @param src Pointer to the null-terminated string to be trimmed
 * @param trim_chars Pointer to the null-terminated string containing
 characters
 * to be trimmed If NULL or empty, defaults to whitespace characters:
 * "\t\n\v\r\f "
 * @return void* Returns a pointer to the newly allocated trimmed string, or
 * NULL if memory allocation fails
 */
void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;
  s21_size_t len = src ? s21_strlen(src) : 0;
  if (src) result = (char *)calloc((len + 1), sizeof(char));
  if (result && trim_chars) {
    s21_size_t i = 0, j = 0;
    for (; i < len && s21_strchr(trim_chars, src[i]); ++i)
      ;
    for (; i < len; ++j) result[j] = src[i++];
    for (; j != 0 && s21_strchr(trim_chars, result[--j]) && j < len;)
      result[j] = '\0';
  }
  return result;
}
// additional functions
/**
 * @brief Concatenates the string 'src' to the end of the string 'dest'
 *
 * @param dest Pointer to the null-terminated destination string
 * @param src Pointer to the null-terminated source string to be appended
 * @return char* Returns a pointer to the destination string 'dest' after
 * concatenation
 */
char *s21_strcat(char *dest, const char *src) {
  char *temp = dest + s21_strlen(dest);
  for (; *src; src++) {
    *temp = *src;
    temp++;
  }
  *temp = '\0';
  return dest;
}
/**
 * @brief Concatenates up to 'n' characters from the string 'src' to the end of
 * the string 'dest'
 *
 * @param dest Pointer to the null-terminated destination string
 * @param src Pointer to the null-terminated source string to be appended
 * @param n Maximum number of characters to concatenate from 'src'
 * @return Returns a pointer to the destination string 'dest' after
 * concatenation
 */
char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dest_inside = dest;
  while (*dest_inside != '\0') dest_inside++;
  while (n > 0 && *src != '\0') {
    *dest_inside = *src;
    dest_inside++;
    src++;
    n--;
  }
  *dest_inside = '\0';
  return dest;
}
/**
 * @brief Retrieves the error message string corresponding to the error number
 * 'errnum'
 *
 * @param errnum Integer representing the error number
 * @return char* Pointer to the error message string corresponding to 'errnum'
 *         If 'errnum' is out of range, returns a static string indicating
 * unknown error
 */
char *s21_strerror(int errnum) {
  static char *strerr[] = s21_error;
  static char s21_undef[ERRS_COUNTER] = {'\0'};
  char *res = s21_NULL;
  if (errnum < 0 || errnum >= ERRS_COUNTER) {
    s21_sprintf(s21_undef, "Unknown error %d", errnum);
    res = s21_undef;
  } else {
    res = strerr[errnum];
  }
  return res;
}
/**
 * @brief Extracts tokens from the string 'str' based on the delimiter 'delim'
 *
 * @param str Pointer to the null-terminated string to be tokenized
 *            If str is NULL, function continues from the last token found
 * @param delim Pointer to the null-terminated string of delimiter characters
 * @return Pointer to the next token found in 'str', or NULL if no more
 * tokens are found
 */
char *s21_strtok(char *str, const char *delim) {
  static char *nextToken = 0;
  static char *toReturn = 0;
  if (str != s21_NULL) nextToken = str;
  if (nextToken != s21_NULL && *nextToken != 0) {
    toReturn = nextToken;
    int slider = 0;
    int delimLen = s21_strlen(delim);
    int wasFound = 0;
    while (*(nextToken) != 0 && !wasFound) {
      slider = 0;
      while (slider < delimLen && !wasFound) {
        if (delim[slider] == *nextToken) {
          wasFound = 1;
          *nextToken = 0;
          nextToken++;
        }
        slider++;
      }
      if (!wasFound) {
        nextToken++;
      }
    }
    if (s21_strlen(toReturn) == 0) {
      s21_strtok(s21_NULL, delim);
    }
  } else {
    toReturn = s21_NULL;
  }
  return toReturn;
}
// comparison functions
/**
 * @brief Compares two memory blocks 'str1' and 'str2' up to 'n' bytes
 *
 * @param str1 Pointer to the first memory block
 * @param str2 Pointer to the second memory block
 * @param n Number of bytes to compare
 * @return int 0 if the first 'n' bytes of 'str1' and 'str2' are identical,
 *             < 0 if 'str1' is less than 'str2',
 *             > 0 if 'str1' is greater than 'str2'
 */
int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int result = 0;
  for (int i = 0; i < (int)n; i++) {
    if (((unsigned char *)str1)[i] != ((unsigned char *)str2)[i]) {
      result = ((unsigned char *)str1)[i] - ((unsigned char *)str2)[i];
      break;
    }
  }
  return result;
}
/**
 * @brief Compares two strings 'str1' and 'str2'
 *
 * @param str1 Pointer to the first string
 * @param str2 Pointer to the second string
 * @return int 0 if 'str1' is equal to 'str2',
 *             < 0 if 'str1' is less than 'str2',
 *             > 0 if 'str1' is greater than 'str2'
 */
int s21_strcmp(const char *str1, const char *str2) {
  while (*str1 && (*str1 == *str2)) {
    str1++;
    str2++;
  }
  return *(unsigned char *)str1 - *(unsigned char *)str2;
}
/**
 * @brief Compares up to 'n' characters of two strings 'str1' and 'str2'
 *
 * @param str1 Pointer to the first string
 * @param str2 Pointer to the second string
 * @param n Maximum number of characters to compare
 * @return int 0 if 'str1' is equal to 'str2',
 *             < 0 if 'str1' is less than 'str2',
 *             > 0 if 'str1' is greater than 'str2'
 */
int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  int i = 0;
  int result = 0;
  for (; *str1 && *str1 == *str2 && i < (int)n - 1; str1++, str2++) {
    i++;
  }
  if ((!*str1 && !*str2) || (n == 0))
    result = 0;
  else
    result = *str1 - *str2;
  return result;
}
// calculation functions
/**
 * @brief Calculates the length of the string 'str'
 *
 * @param str Pointer to the string whose length is to be calculated
 * @return size_t Length of the string 'str'
 */
s21_size_t s21_strlen(const char *str) {
  s21_size_t chLen = 0;
  while (str[chLen] != 0) chLen++;
  return chLen;
}
/**
 * @brief Calculates the length of the initial segment of 'str1' consisting of
 * characters from 'str2'
 *
 * @param str1 Pointer to the string to be searched
 * @param str2 Pointer to the string containing characters to match
 * @return s21_size_t Length of the initial segment of 'str1' containing
 * characters from 'str2'
 */
s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t len = 0;
  int match = 1;
  while (match && *(str1 + len)) {
    match = 0;
    for (s21_size_t i = 0; *(str2 + i); ++i) {
      if (*(str1 + len) == *(str2 + i)) {
        match = 1;
        ++len;
        break;
      }
    }
  }
  return len;
}
/**
 * @brief Calculates the length of the initial segment of 'str1' that contains
 * no characters from 'str2'
 *
 * @param str1 Pointer to the string to be searched
 * @param str2 Pointer to the string containing characters to avoid
 * @return s21_size_t Length of the initial segment of 'str1' that contains no
 * characters from 'str2'
 */
s21_size_t s21_strcspn(const char *str1, const char *str2) {
  size_t chLen1 = s21_strlen(str1);
  size_t chLen2 = s21_strlen(str2);
  size_t toReturn = 0;
  int wasFound = 0;
  for (size_t i = 0; i < chLen1 && !wasFound; i++) {
    for (size_t j = 0; j < chLen2 && !wasFound; j++) {
      if (str1[i] == str2[j]) {
        toReturn = i;
        wasFound = 1;
      }
    }
  }
  if (!wasFound) toReturn = chLen1;
  return toReturn;
}