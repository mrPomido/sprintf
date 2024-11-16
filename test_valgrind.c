/**
 * @file test_valgrind.c
 * @brief This file contains a set of tests for the custom string library
 * "s21_string.h".
 *
 * The file includes test functions for various string manipulation functions
 * implemented in "s21_string.h". These tests cover functions similar to those
 * in the standard C library <string.h> but implemented as part of the
 * s21_string library.
 *
 * The primary purpose of this file is to perform memory checks using Valgrind,
 * ensuring that there are no memory leaks, invalid memory accesses, or other
 * memory-related errors in the custom string functions.
 *
 * */
#include <string.h>

#include "s21_string.h"

#define TEST_SIZE 100

void test_memcpy() {
  char src[TEST_SIZE];
  char dest[TEST_SIZE];
  size_t n = sizeof(src);
  memset(src, 'A', TEST_SIZE - 1);
  src[TEST_SIZE - 1] = '\0';
  s21_memcpy(dest, src, n);
  printf("Result of memcpy: %s\n", dest);
}

void test_memset() {
  char str[TEST_SIZE];
  s21_memset(str, 'b', TEST_SIZE - 1);
  str[TEST_SIZE - 1] = '\0';
  printf("Result of memset: %s\n", str);
}

void test_strcpy() {
  char src[] = "Hello, World!";
  char dest[TEST_SIZE];
  s21_strcpy(dest, src);
  printf("Result of strcpy: %s\n", dest);
}

void test_strncpy() {
  char src[] = "Hello, World!";
  char dest[TEST_SIZE];
  s21_strncpy(dest, src, 5);
  printf("Result of strncpy: %s\n", dest);
}

void test_memchr() {
  const char str[] = "Hello, World!";
  const char ch = '.';
  s21_size_t n = strlen(str);
  void *res = s21_memchr(str, ch, n);
  if (res != NULL) {
    printf("Result of memchr: %s\n", (char *)res);
  } else {
    printf("Character not found.\n");
  }
}

void test_strchr() {
  const char str[] = "This is a test string";
  const char ch = 's';
  char *res = s21_strchr(str, ch);
  if (res != NULL) {
    printf("Result of strchr: %s\n", res);
  } else {
    printf("Character not found.\n");
  }
}

void test_strpbrk() {
  const char str1[] = "abcdef";
  const char str2[] = "xyz";
  char *res = s21_strpbrk(str1, str2);
  if (res != NULL) {
    printf("Result of strpbrk: %s\n", res);
  } else {
    printf("No characters in str1 match any characters in str2.\n");
  }
}

void test_strrchr() {
  const char str[] = "This is a test string";
  const char ch = 's';
  char *res = s21_strrchr(str, ch);
  if (res != NULL) {
    printf("Result of strrchr: %s\n", res);
  } else {
    printf("Character not found.\n");
  }
}

void test_strstr() {
  const char haystack[] = "TutorialsPoint is for learning";
  const char needle[] = "Point";
  char *res = s21_strstr(haystack, needle);
  if (res != NULL) {
    printf("Result of strstr: %s\n", res);
  } else {
    printf("Substring not found.\n");
  }
}

void test_to_upper() {
  const char str[] = "Hello, World!";
  char *res = s21_to_upper(str);
  printf("Result of to_upper: %s\n", res);
  free(res);
}

void test_to_lower() {
  const char str[] = "Hello, World!";
  char *res = s21_to_lower(str);
  printf("Result of to_lower: %s\n", res);
  free(res);
}

void test_insert() {
  const char src[] = "Hello!";
  const char str[] = "World";
  s21_size_t start_index = 3;
  char *res = s21_insert(src, str, start_index);
  printf("Result of insert: %s\n", res);
  free(res);
}

void test_trim() {
  const char src[] = "   Hello, World!   ";
  const char trim_chars[] = " \t";
  char *res = s21_trim(src, trim_chars);
  printf("Result of trim: %s\n", res);
  free(res);
}

void test_strcat() {
  char dest[TEST_SIZE] = "Hello, ";
  const char src[] = "World!";
  s21_strcat(dest, src);
  printf("Result of strcat: %s\n", dest);
}

void test_strncat() {
  char dest[TEST_SIZE] = "Hello, ";
  const char src[] = "World!";
  s21_strncat(dest, src, 3);
  printf("Result of strncat: %s\n", dest);
}

void test_strerror() {
  int errnum = 5;
  char *res = s21_strerror(errnum);
  printf("Result of strerror: %s\n", res);
}

void test_strtok() {
  char str[] = "This is a test string";
  const char delim[] = " ";
  char *token = s21_strtok(str, delim);
  while (token != NULL) {
    printf("Token: %s\n", token);
    token = s21_strtok(NULL, delim);
  }
}

void test_memcmp() {
  const char str1[] = "abc";
  const char str2[] = "abc";
  s21_size_t n = 3;
  int res = s21_memcmp(str1, str2, n);
  printf("Result of memcmp: %d\n", res);
}

void test_strcmp() {
  const char str1[] = "abc";
  const char str2[] = "abc";
  int res = s21_strcmp(str1, str2);
  printf("Result of strcmp: %d\n", res);
}

void test_strncmp() {
  const char str1[] = "abc";
  const char str2[] = "abd";
  s21_size_t n = 2;
  int res = s21_strncmp(str1, str2, n);
  printf("Result of strncmp: %d\n", res);
}

void test_strlen() {
  const char str[] = "Hello, World!";
  s21_size_t len = s21_strlen(str);
  printf("Length of string: %llu\n", len);
}

void test_strspn() {
  const char str1[] = "abcde312$#@";
  const char str2[] = "abc";
  s21_size_t len = s21_strspn(str1, str2);
  printf("Length of initial segment: %llu\n", len);
}

void test_strcspn() {
  const char str1[] = "abcde312$#@";
  const char str2[] = "123";
  s21_size_t len = s21_strcspn(str1, str2);
  printf("Length of initial segment without characters from str2: %llu\n", len);
}

void test_sprintf() {
  char buf[100];
  s21_sprintf(buf, "%d + %d = %d", 2, 3, 5);
  printf("Result of sprintf: %s\n", buf);
}

void test_sscanf() {
  const char str[] = "Hello 123 4.56";
  int a, b;
  float c;
  s21_sscanf(str, "%d %d %f", &a, &b, &c);
  printf("Result of sscanf: a=%d, b=%d, c=%.2f\n", a, b, c);
}

int main() {
  test_memcpy();
  test_memset();
  test_strcpy();
  test_strncpy();
  test_memchr();
  test_strchr();
  test_strpbrk();
  test_strrchr();
  test_strstr();
  test_to_upper();
  test_to_lower();
  test_insert();
  test_trim();
  test_strcat();
  test_strncat();
  test_strerror();
  test_strtok();
  test_memcmp();
  test_strcmp();
  test_strncmp();
  test_strlen();
  test_strspn();
  test_strcspn();
  test_sprintf();
  test_sscanf();

  return 0;
}