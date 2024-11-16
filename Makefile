CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c11 -g 
LDFLAGS=$(shell pkg-config --cflags --libs check)
GCOVFLAGS=-fprofile-arcs -ftest-coverage
TESTS=s21_sprintf_tests.c s21_sscanf_tests.c s21_string_tests.c
SOURSES=s21_sprintf.c s21_sscanf.c s21_string.c

VALGRIND_EXEC=test_valgrind
VALGRIND_SOURCES=test_valgrind.c

all: s21_string.a

s21_string.a:
#	$(CC) $(CFLAGS) -c *.c 
	$(CC) $(CFLAGS) -c $(SOURSES)
	ar rc s21_string.a *.o
	ranlib s21_string.a

test:
	$(CC) $(CFLAGS) $(LDFLAGS) $(TESTS) $(SOURSES) -lm 
	./a.out
	rm a.out

gcov_report:
	$(CC) $(CFLAGS) $(LDFLAGS) $(GCOVFLAGS) $(TESTS) $(SOURSES)  -o gcov_main -lm 
	./gcov_main
	lcov --capture --directory . --output-file coverage.info
	lcov --extract coverage.info '*/s21_sprintf.c' '*/s21_sscanf.c' '*/s21_string.c' -o filtered_coverage.info
	genhtml filtered_coverage.info --output-directory gcov_report
#	xdg-open gcov_report/index.html
	open gcov_report/index.html

clean:
	-rm -rf *.o *.html *.gcda *.gcno *.css *.a *.gcov *.info *.out *.cfg *.txt gcov*  $(VALGRIND_EXEC)

fmt_check:
	clang-format -n *.c *.h

fmt:
	clang-format -i *.c *.h

cpk:
	cppcheck *.[ch]

valgrinder: s21_string.a
	$(CC) $(CFLAGS) -o $(VALGRIND_EXEC) $(VALGRIND_SOURCES) s21_string.a -lm
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --show-leak-kinds=all --num-callers=20 --track-fds=yes ./$(VALGRIND_EXEC) 1 > /dev/null