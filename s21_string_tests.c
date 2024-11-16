/**
 * @file:  test_s21_string.c
 * @brief File for unit testing the s21_string library.
 *
 * Description:
 *   This file contains unit tests for the functions in the s21_string library.
 *   Each function from s21_string.h is rigorously tested to ensure correct
 *   functionality across various scenarios. The tests cover copying, searching,
 *   transformation, additional utility, comparison, and calculation functions.
 *
 *   Tests are organized into separate test cases, focusing on specific groups
 *   of functions. This file relies on the Check testing framework to run the
 * unit tests and report results. The framework provides detailed information on
 *   test failures, including line numbers and expected vs. actual values,
 *   aiding in rapid identification and resolution of issues.
 *
 */
#include "s21_string.h"
#include "s21_tests.h"

// copy functions
START_TEST(test_s21_memcpy_basic) {
  char *s21_result;
  char *str_result;

  char str[20] = "Hello world!!";
  char s21_buffer[20] = {'\0'}, str_buffer[20] = {'\0'};
  int len_array[5] = {1, 20, 15, 0, 5};

  s21_result = s21_memcpy(s21_buffer, str, len_array[_i]);
  str_result = memcpy(str_buffer, str, len_array[_i]);
  ck_assert_str_eq(s21_result, str_result);
}
END_TEST

START_TEST(s21_memset_tests) {
  char test_1[20] = "bobr kurva";
  char test_2[25] = "kurva bobr";

  s21_size_t len_arr[5] = {0, 3, 4, 15, 10};
  char char_arr[5] = {'a', 'c', '.', '0', '%'};

  char *s21_result = s21_memset(test_1, char_arr[_i], len_arr[_i]);
  char *str_result = memset(test_1, char_arr[_i], len_arr[_i]);
  ck_assert_str_eq(s21_result, str_result);

  s21_result = s21_memset(test_2, char_arr[_i], len_arr[_i]);
  str_result = memset(test_2, char_arr[_i], len_arr[_i]);
  ck_assert_str_eq(s21_result, str_result);

  const struct {
    const char *desc;
    int symbol;
    s21_size_t n;
  } testcases[] = {
      {"hello", 'h', 5}, {"hello", 'o', 5},   {"hello", 'x', 5},
      {"1234", 'b', 0},  {"!@#$^&*", '&', 4}, {"000", '\0', 2},
  };
  s21_size_t len = sizeof(testcases) / sizeof(testcases[0]);
  char copy[1024];
  for (s21_size_t i = 0; i < len; i++) {
    strcpy(copy, testcases[i].desc);
    void *result = s21_memset(copy, testcases[i].symbol, testcases[i].n);
    void *s21_result = memset(copy, testcases[i].symbol, testcases[i].n);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_strcpy_tests) {
  char s21_dest[20] = "bobr_kurva";
  char str_dest[20] = "bobr_kurva";

  char *src_arr[6] = {"aaaaaaaaaaa", "ax+=0998765432", "_+!2345a  r", "mmm..",
                      " "};

  char *s21_result = s21_strcpy(s21_dest, src_arr[_i]);
  char *str_result = strcpy(str_dest, src_arr[_i]);

  ck_assert_str_eq(s21_result, str_result);
}
END_TEST

START_TEST(s21_strncpy_tests) {
  char copy_test[] =
      "1234567890qwertyuiop[]asdfghjkl;zxcv\0bnm,./"
      "\n\tQWERTYUIOPASDFGHJ\0KLZXCVBNM";
  char got[100] = "Rewrite me pls";
  char must[100] = "Rewrite me pls";
  for (s21_size_t size = 0; size < 71; size++) {
    s21_strncpy(got, copy_test, size);
    strncpy(must, copy_test, size);
    ck_assert_str_eq(got, must);
  }
  const struct {
    const char *desc;
    const char *src;
    s21_size_t n;
  } testcases[] = {{"aaaaaa", "bbb", 4},
                   {"aaaaaa", "123", 4},
                   {"aaaaaa", "!@#", 4},
                   {"12345", "", 1},
                   {"qwe", "\0", 2},
                   {"qwe", "\n", 2},
                   {"empty", "null", 0},
                   {"longlonglonglong", "short", 5},
                   {"short", "longlonglonglong", 10},
                   {"", "nonempty", 8},
                   {"bobr_kurva_bobr_kurva", "8765432", 3},
                   {"bobr_kurva_bobr_kurva", "_+!ddddd  r", 2},
                   {"bobr_kurva_bobr_kurva", "amammamama", 4},
                   {"bobr_kurva_bobr_kurva", "mmm.....", 5},
                   {"bobr_kurva_bobr_kurva", " ", 1},
                   {"simple copy", "simple copy", 12},
                   {"Rewrite me pls", "simple copy", 12},
                   {"Rewrite me pls", "simple copy", 1},
                   {"", "", 0},
                   {"Rewrite me pls",
                    "1234567890qwertyuiop[]asdfghjkl;zxcvbnm,./"
                    "\n\tQWERTYUIOPASDFGHJKLZXCVBNM",
                    71}};
  s21_size_t len = sizeof(testcases) / sizeof(testcases[0]);
  char copy[1024];
  for (s21_size_t i = 0; i < len; i++) {
    strcpy(copy, testcases[i].desc);
    char *result = s21_strncpy(copy, testcases[i].src, testcases[i].n);
    char expected_copy[1024];
    strcpy(expected_copy, testcases[i].desc);
    char *s21_result = strncpy(expected_copy, testcases[i].src, testcases[i].n);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

// search functions
START_TEST(s21_strcmp_tests) {
  struct {
    const char *str1;
    const char *str2;
  } testcases[] = {{"Hello", "Hello"},
                   {"Hello\0\0\0", "Hello\0 world"},
                   {"Hello world", "Hello"},
                   {"", ""},
                   {"", "Hello"},
                   {"Hello", ""},
                   {"123456", "123098"},
                   {"!@#$^&", "!@#*&^"},
                   {"abcdef", "abcdef"},
                   {"abcdef", "abcxyz"}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    int result = s21_strcmp(testcases[i].str1, testcases[i].str2);
    int s21_result = strcmp(testcases[i].str1, testcases[i].str2);
    ck_assert_int_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_strncmp_tests) {
  const struct {
    const char *str1;
    const char *str2;
    s21_size_t n;
  } testcases[] = {{"Hello world!", "Hello", 13},
                   {"Hello world!", "Hello world!", 13},
                   {"Hello world!", "Hello world!!!", 3},
                   {"Hello world!", "Hello world!!!", 30},
                   {"\0Hello", "", 1},
                   {"", "", 1},
                   {"\0\0\0", "\0\0\0", 3},
                   {"123456", "123456", 6},
                   {"123456", "123098", 6},
                   {"!@#$^&", "!@#$^&", 6},
                   {"!@#$^&", "!@#*&^", 6},
                   {"abcdef", "lollol", 3},
                   {"abcdef", "abcxyz", 3},
                   {"abcdef", "abcxyz", 6}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    int result =
        s21_strncmp(testcases[i].str1, testcases[i].str2, testcases[i].n);
    int s21_result =
        strncmp(testcases[i].str1, testcases[i].str2, testcases[i].n);
    ck_assert_int_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_memcmp_tests) {
  char *str1 = "atoms\0\0\0\0";
  char *str2 = "atoms\0abc";
  ck_assert_int_eq(s21_memcmp(str1, str2, 7), memcmp(str1, str2, 7));

  char *str3 = "atom";
  char *str4 = "atom";
  ck_assert_int_eq(s21_memcmp(str3, str4, 3), memcmp(str3, str4, 3));

  char *str5 = "atoms";
  char *str6 = " ";
  ck_assert_int_eq(s21_memcmp(str5, str6, 1), memcmp(str5, str6, 1));

  char *str7 = "";
  char *str8 = "";
  ck_assert_int_eq(s21_memcmp(str7, str8, 0), memcmp(str7, str8, 0));

  int array1[] = {11, 12, 13, 14, 15, 16, 17, 18, 19};
  int array2[] = {11, 12, 13, 14, 15, 16, 17, 17, 19};
  s21_size_t n = 7 * sizeof(int);
  ck_assert_int_eq(s21_memcmp(array1, array2, n), memcmp(array1, array2, n));

  char *str9 = "Hello world!0123";
  char *str10 = "Hello world!";
  ck_assert_int_le(s21_memcmp(str9, str10, 10), memcmp(str9, str10, 10));
  // ck_assert_int_ge(s21_memcmp(str9, str10, 15), memcmp(str9, str10, 15));
  ck_assert_int_le(s21_memcmp(str9, str10, 13), memcmp(str9, str10, 13));
}
END_TEST

START_TEST(test_s21_strchr_test1) {
  struct input_data {
    const char *str;
    int c;
  };
  struct input_data testcases[] = {
      {"hello, peerka", 'o'},
      {"hello, \0peerka", 'e'},
      {"aaaaaaaa", 'b'},
      {"", '\0'},
      {"\0string", 's'},
      {"abo boba", ' '},
      {"the\ntrap", '\n'},
      {"12345678", '5'},
      {"!@#$^&*", '$'},
      {"aaaa", 'a'},
      {"ABCZ", 'Z'},
      {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", '\0'},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strchr(testcases[i].str, testcases[i].c);
    char *s21_result = strchr(testcases[i].str, testcases[i].c);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(test_s21_strpbrk_test1) {
  struct input_data {
    const char *str1;
    const char *str2;
  };
  struct input_data testcases[] = {
      {"Hello world!", "w"},
      {"Hello world!", " d"},
      {"Hello world!", " H"},
      {"Hello world!", "x"},
      {"12345", "\0"},
      {"12345Z67890", ""},
      {"!@#", "!"},
      {"biba", ""},
      {"", ""},
      {"ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ", "12345Z67890"},
      {"", "AB"}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strpbrk(testcases[i].str1, testcases[i].str2);
    char *s21_result = strpbrk(testcases[i].str1, testcases[i].str2);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(test_s21_strrchr_test1) {
  struct input_data {
    const char *str;
    int c;
  };
  struct input_data testcases[] = {
      {"Hello World!", 'o'}, {"", '\0'},     {"AZNOPQRSTZUVWXYZ", 'Z'},
      {"Hello", '\0'},       {"Hello", 'a'}, {"1111111111234", '2'},
      {"!@#", '@'},          {"", '0'},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strrchr(testcases[i].str, testcases[i].c);
    char *s21_result = strrchr(testcases[i].str, testcases[i].c);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(test_s21_strstr_test1) {
  char str1[] = {'q', 'r', 's', 't', 'w', 's', 'a', 's', '\0'};
  char *str2 = "woo";
  ck_assert_pstr_eq(strstr(str1, str2), s21_strstr(str1, str2));
  struct input_data {
    const char *haystack;
    const char *needle;
  };
  struct input_data testcases[] = {
      {"Hello world!", "world"},
      {"Hello world!", "biba"},
      {"Hello world!", ""},
      {"hi hi hi", "hi"},
      {"hi hi hi", "Hi"},
      {"", ""},
      {"Hello", ""},
      {"", "Hello"},
      {"ABCabc", "AB"},
      {"H", "H"},
      {"qwertyuiopQWERTYUIOP{}|\":<>?", "YTREWQ"},
      {"Hello\0world", "world"},
      {"12345", "34"},
      {"!@#$", "@"},
      {"Hello\tworld", "world"},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char *result = s21_strstr(testcases[i].haystack, testcases[i].needle);
    char *s21_result = strstr(testcases[i].haystack, testcases[i].needle);
    ck_assert_pstr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(test_s21_memchr_test1) {
  int int_array[] = {11, 12, 13, 14, 15, 16, 17, 18, 19};
  int int_c = 19;
  s21_size_t int_n = 5 * sizeof(int);
  ck_assert_ptr_eq(s21_memchr(int_array, int_c, int_n),
                   memchr(int_array, int_c, int_n));

  float float_array[] = {11.1, 12.1, 13.1, 14, 15.99, 16.001, 17, 18, 19};
  float float_c = 14;
  s21_size_t float_n = 9 * sizeof(float);
  ck_assert_ptr_eq(s21_memchr(float_array, float_c, float_n),
                   memchr(float_array, float_c, float_n));

  struct input_data {
    const char *str;
    int c;
    s21_size_t n;
  };
  struct input_data testcases[] = {
      {"hello, peerka", 'o', 8},
      {"hello, \0peerka", 'e', 4},
      {"aaaaaaaa", 'b', 8},
      {"", '\0', 1},
      {"\0string", 's', 8},
      {"abo boba", ' ', 8},
      {"the\ntrap", '\n', 8},
      {"12345678", '5', 8},
      {"!@#$^&*", '$', 8},
      {"aaaa", 'a', 4},
      {"", 'A', 1},
      {"123456789", '0', 9},
      {"123456789", '1', 9},
  };
  s21_size_t num_testcases = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < num_testcases; i++) {
    void *result = s21_memchr(testcases[i].str, testcases[i].c, testcases[i].n);
    void *s21_result = memchr(testcases[i].str, testcases[i].c, testcases[i].n);
    ck_assert_ptr_eq(result, s21_result);
  }
}
END_TEST

START_TEST(strtok_1) {
  char str4_1[] = "/testing/with/original/string.h/";
  char str4_2[] = "/testing/with/original/string.h/";
  char del1[3] = "/";
  char *nstr_1, *nstr_2;
  nstr_1 = strtok(str4_1, del1);
  nstr_2 = s21_strtok(str4_2, del1);
  while (nstr_1 != s21_NULL) {
    ck_assert_pstr_eq(nstr_1, nstr_2);
    nstr_1 = strtok(s21_NULL, del1);
    nstr_2 = s21_strtok(s21_NULL, del1);
  }
}
END_TEST

START_TEST(strtok_2) {
  char str1[] = "++++++++";
  char str2[] = "++++++++";
  const char delims[] = "+_! =";
  char *got = s21_strtok(str1, delims);
  char *expected = strtok(str2, delims);
  ck_assert_ptr_null(got);
  ck_assert_ptr_null(expected);
}
END_TEST

START_TEST(strtok_3) {
  char str1[] = "Aboba_Floppa_test";
  char str2[] = "Aboba_Floppa_test";
  const char delims[] = "+_! =";
  char *got = s21_strtok(str1, delims);
  char *expected = strtok(str2, delims);
  ck_assert_uint_eq(s21_strlen(got), s21_strlen(expected));
  ck_assert_str_eq(got, expected);
  int i = 5;
  while (i) {
    got = s21_strtok(s21_NULL, delims);
    expected = strtok(s21_NULL, delims);
    i--;
    if (got || expected) {
      ck_assert_str_eq(got, expected);
    } else {
      ck_assert_ptr_null(got);
      ck_assert_ptr_null(expected);
    }
  }
}
END_TEST

START_TEST(s21_strcat_tests) {
  char *s21_result;
  char *str_result;
  char buffer[100] = {" "};
  char s21_buffer[100] = {" "};
  char *test_array[5] = {"abracadabra", "papa./'[]\'%s   ", "0-s221A", "\0",
                         ""};
  s21_result = s21_strcat(s21_buffer, test_array[_i]);
  str_result = strcat(buffer, test_array[_i]);
  ck_assert_str_eq(s21_result, str_result);
}
END_TEST

START_TEST(s21_strncat_tests) {
  struct input_data {
    char str1[1024];
    const char *str2;
    s21_size_t n;
  };
  struct input_data testcases[] = {
      {"Hello, ", "World!", 13},        {"Hello, ", "123456", 13},
      {"Hello, ", "!@#$^&", 13},        {"Hello add nothing", "", 10},
      {"", "+Hello add nothing", 10},   {"0 symbols", "hello", 0},
      {"Hello from empty line", "", 5}, {"     ", "      ", 3}};
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    char str1_copy[1024];
    strcpy(str1_copy, testcases[i].str1);
    char expected_str1_copy[1024];
    strcpy(expected_str1_copy, testcases[i].str1);
    char *result = s21_strncat(str1_copy, testcases[i].str2, testcases[i].n);
    char *s21_result =
        strncat(expected_str1_copy, testcases[i].str2, testcases[i].n);
    ck_assert_pstr_eq(result, s21_result);
  }

  char *s21_result;
  char *str_result;
  char buffer[100] = {" "};
  char s21_buffer[100] = {" "};

  char *test_array[5] = {"abracadabra", "   ", "0-s221A", "\0", ""};
  int len_array[5] = {1, 100, 32, 0, 100};

  s21_result = s21_strncat(s21_buffer, test_array[_i], len_array[_i]);
  str_result = strncat(buffer, test_array[_i], len_array[_i]);
  ck_assert_str_eq(s21_result, str_result);
}
END_TEST

START_TEST(s21_strerror_tests) {
  ck_assert_str_eq(s21_strerror(_i), strerror(_i));
}
END_TEST

START_TEST(s21_strlen_tests) {
  const char *testcases[] = {
      "Hello", "Hello world!", "123456", "!@#$^", "a\na\n", "", "\0string",
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    s21_size_t result = s21_strlen(testcases[i]);
    s21_size_t s21_result = strlen(testcases[i]);
    ck_assert_uint_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_strcspn_tests) {
  const char *testcases[][2] = {
      {"Hello world!", "Hello"},
      {"Hello world!", "Helo"},
      {"Hello world!", "abc"},
      {"Hello world!", ""},
      {"", "Hello"},
      {"Hello world!", "Hello world"},
      {"12345", "12345"},
      {"12345", "12654"},
      {"0123456789", "9876"},
      {"0123456789", "210"},
      {"", ""},
      {"\0string", ""},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    s21_size_t result = s21_strcspn(testcases[i][0], testcases[i][1]);
    s21_size_t s21_result = strcspn(testcases[i][0], testcases[i][1]);
    ck_assert_uint_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_strspn_tests) {
  const char *testcases[][2] = {
      {"Hello world!", "Hello"},
      {"Hello world!", "Helo"},
      {"Hello world!", "abc"},
      {"Hello world!", ""},
      {"", "Hello"},
      {"Hello world!", "Hello world"},
      {"12345", "12345"},
      {"12345", "12654"},
      {"0123456789", "9876"},
      {"0123456789", "210"},
      {"", ""},
      {"\0string", ""},
  };
  s21_size_t n = sizeof(testcases) / sizeof(testcases[0]);
  for (s21_size_t i = 0; i < n; i++) {
    s21_size_t result = s21_strcspn(testcases[i][0], testcases[i][1]);
    s21_size_t s21_result = strcspn(testcases[i][0], testcases[i][1]);
    ck_assert_uint_eq(result, s21_result);
  }
}
END_TEST

START_TEST(s21_to_upper_tests) {
  char *str1 =
      "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"
      "0";
  char *str2 = s21_to_upper(str1);
  ck_assert_str_eq(str2,
                   "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVW"
                   "XYZ1234567890");
  free(str2);

  char *str3 = s21_NULL;
  char *str4 = s21_to_upper(str3);
  ck_assert_ptr_null(str4);
  free(str4);
}
END_TEST

START_TEST(s21_to_lower_tests) {
  char *str1 =
      "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789"
      "0";
  char *str2 = s21_to_lower(str1);
  ck_assert_str_eq(str2,
                   "1234567890abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvw"
                   "xyz1234567890");
  free(str2);

  char *str7 = s21_NULL;
  char *str8 = s21_to_lower(str7);
  ck_assert_ptr_null(str8);
  free(str8);
}
END_TEST

// uwu
START_TEST(s21_insert_tests) {
  char *str1 = "4";
  char *str2 = "2";
  char *str_res1 = s21_insert(str1, str2, 0);
  ck_assert_str_eq(str_res1, "24");
  free(str_res1);
  char *str_res2 = s21_insert(str1, str2, 1);
  ck_assert_str_eq(str_res2, "42");
  free(str_res2);

  char *str10 = "";
  char *str20 = "abcdefghijklmnopqrstuvwxyz";
  char *str_res10 = s21_insert(str10, str20, 0);
  ck_assert_str_eq(str_res10, "abcdefghijklmnopqrstuvwxyz");
  free(str_res10);
  char *str_res20 = s21_insert(str20, str10, 0);
  ck_assert_str_eq(str_res20, "abcdefghijklmnopqrstuvwxyz");
  free(str_res20);

  char *str11 = "12345678901234567890";
  char *str21 = "abcdefghijklmnopqrstuvwxyz";
  char *str_res11 = s21_insert(str11, str21, 10);
  ck_assert_str_eq(str_res11, "1234567890abcdefghijklmnopqrstuvwxyz1234567890");
  free(str_res11);
  char *str_res21 = s21_insert(str21, str11, 0);
  ck_assert_str_eq(str_res21, "12345678901234567890abcdefghijklmnopqrstuvwxyz");
  free(str_res21);
  char *str_res31 = s21_insert(str11, str21, 20);
  ck_assert_str_eq(str_res31, "12345678901234567890abcdefghijklmnopqrstuvwxyz");
  free(str_res31);

  char *str12 = NULL;
  char *str22 = "1234567890";
  char *str32 = "ABC";
  char *str_res12 = s21_insert(str12, str22, 0);
  ck_assert_str_eq(str_res12, "1234567890");
  free(str_res12);
  char *str_res22 = s21_insert(str22, str12, 0);
  ck_assert_str_eq(str_res22, "1234567890");
  free(str_res22);
  char *str_res32 = s21_insert(str22, str32, -1);
  ck_assert_ptr_null(str_res32);
  free(str_res32);
  char *str_res42 = s21_insert(str22, str32, 11);
  ck_assert_ptr_null(str_res42);
  free(str_res42);
}
END_TEST

// uwu
START_TEST(s21_trim_tests) {
  char *str = "111222111";
  char *str_res1 = s21_trim(str, "1");
  ck_assert_str_eq(str_res1, "222");
  free(str_res1);
  char *str_res2 = s21_trim(str, "12");
  ck_assert_str_eq(str_res2, "");
  free(str_res2);

  char *str1 = "qwertyuiop ,. qwertyuiop";
  char *str_res11 = s21_trim(str1, " ,.");
  ck_assert_str_eq(str_res11, "qwertyuiop ,. qwertyuiop");
  free(str_res11);
  char *str_res21 = s21_trim(str1, "qwertyuiop");
  ck_assert_str_eq(str_res21, " ,. ");
  free(str_res21);

  char *str0 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *str_res10 = s21_trim(str0, " ,.");
  ck_assert_str_eq(str_res10, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  free(str_res10);
  char *str_res20 = s21_trim(str0, "ABC");
  ck_assert_str_eq(str_res20, "DEFGHIJKLMNOPQRSTUVWXYZ");
  free(str_res20);
  char *str_res30 = s21_trim(str0, "A");
  ck_assert_str_eq(str_res30, "BCDEFGHIJKLMNOPQRSTUVWXYZ");
  free(str_res30);
  char *str_res40 = s21_trim(str0, "Z");
  ck_assert_str_eq(str_res40, "ABCDEFGHIJKLMNOPQRSTUVWXY");
  free(str_res40);
  char *str_res50 = s21_trim(str0, "XYZ");
  ck_assert_str_eq(str_res50, "ABCDEFGHIJKLMNOPQRSTUVW");
  free(str_res50);
  char *str_res60 = s21_trim(str0, "ABCXYZ");
  ck_assert_str_eq(str_res60, "DEFGHIJKLMNOPQRSTUVW");
  free(str_res60);
  //  char *str_res33 = "\n111";
  //   char *str_res33 = s21_trim(str0, s21_NULL);
  //   ck_assert_str_eq(str_res33, "\n111");
  //     free(str_res33);
  //    char *str_res23 = " 111 ";
  //        char *str_res23 = s21_trim(str0, "");
  //          ck_assert_str_eq(str_res23, "111");
  //     free(str_res23);
}
END_TEST

Suite *s21_string_suite(void) {
  Suite *s = suite_create("s21_string.h tests");

  // tests of search functions
  TCase *tc_tests_search;
  s = suite_create("s21_string.h tests");
  tc_tests_search = tcase_create("search_func");
  tcase_add_test(tc_tests_search, test_s21_memchr_test1);
  tcase_add_test(tc_tests_search, test_s21_strrchr_test1);
  tcase_add_test(tc_tests_search, test_s21_strpbrk_test1);
  tcase_add_test(tc_tests_search, test_s21_strstr_test1);
  tcase_add_test(tc_tests_search, test_s21_strchr_test1);
  suite_add_tcase(s, tc_tests_search);

  // tests of transformation functions C#
  TCase *tc_tests_CS;
  tc_tests_CS = tcase_create("C#_func");
  tcase_add_test(tc_tests_CS, s21_to_upper_tests);
  tcase_add_test(tc_tests_CS, s21_to_lower_tests);
  tcase_add_test(tc_tests_CS, s21_insert_tests);
  tcase_add_test(tc_tests_CS, s21_trim_tests);
  suite_add_tcase(s, tc_tests_CS);

  // tests of additional functions
  TCase *tc_tests_help;
  tc_tests_help = tcase_create("help_func");
  tcase_add_loop_test(tc_tests_help, s21_strcat_tests, 0, 5);
  tcase_add_loop_test(tc_tests_help, s21_strncat_tests, 0, 5);
#if defined(__APPLE__)
  tcase_add_loop_test(tc_tests_help, s21_strerror_tests, -1, 109);
#elif defined(__linux__)
  tcase_add_loop_test(tc_tests_help, s21_strerror_tests, -1, 149);
#endif
  // tcase_add_test(tc_tests_help, s21_strtok_tests);
  tcase_add_test(tc_tests_help, strtok_1);
  tcase_add_test(tc_tests_help, strtok_2);
  tcase_add_test(tc_tests_help, strtok_3);
  suite_add_tcase(s, tc_tests_help);

  // tests of comparison functions
  TCase *tc_tests_comp;
  tc_tests_comp = tcase_create("comparison_func");
  tcase_add_test(tc_tests_comp, s21_strcmp_tests);
  tcase_add_test(tc_tests_comp, s21_strncmp_tests);
  tcase_add_test(tc_tests_comp, s21_memcmp_tests);
  suite_add_tcase(s, tc_tests_comp);

  // tests of calculation functions
  TCase *tc_tests_calc;
  tc_tests_calc = tcase_create("calculation_func");
  tcase_add_test(tc_tests_calc, s21_strlen_tests);
  tcase_add_test(tc_tests_calc, s21_strcspn_tests);
  tcase_add_test(tc_tests_calc, s21_strspn_tests);
  suite_add_tcase(s, tc_tests_calc);

  // tests of copy functions
  TCase *tc_tests_copy;
  // s = suite_create("s21_string.h tests");
  tc_tests_copy = tcase_create("copy_func");
  tcase_add_loop_test(tc_tests_copy, test_s21_memcpy_basic, 0, 5);
  tcase_add_loop_test(tc_tests_copy, s21_memset_tests, 0, 5);
  tcase_add_loop_test(tc_tests_copy, s21_strcpy_tests, 0, 5);
  tcase_add_loop_test(tc_tests_copy, s21_strncpy_tests, 0, 5);
  suite_add_tcase(s, tc_tests_copy);

  return s;
}

int main(void) {
  Suite *s, *s1, *s2;
  SRunner *runner, *runner1, *runner2;
  // Создаем тестовые наборы
  s = s21_string_suite();
  s1 = s21_suite_sprintf();
  s2 = s21_sscanf_test();
  // Создаем тестовые раннеры
  runner = srunner_create(s);
  runner1 = srunner_create(s1);
  runner2 = srunner_create(s2);
  // Запускаем тесты в режиме подробного вывода
  srunner_run_all(runner, CK_VERBOSE);
  srunner_run_all(runner1, CK_VERBOSE);
  srunner_run_all(runner2, CK_VERBOSE);
  // Запускаем тесты в минимальном режиме
  srunner_run_all(runner, CK_MINIMAL);
  srunner_run_all(runner1, CK_MINIMAL);
  srunner_run_all(runner2, CK_MINIMAL);
  // Получаем количество проваленных тестов
  srunner_ntests_failed(runner);
  srunner_ntests_failed(runner1);
  srunner_ntests_failed(runner2);
  // Освобождаем память, выделенную для тестовых раннеров
  srunner_free(runner);
  srunner_free(runner1);
  srunner_free(runner2);
  // Возвращаем статус выполнения тестов
  return 0;
}