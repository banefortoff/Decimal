CC=gcc
CFLAG=-c
STDFLAGS=-Werror -Wall -Wextra -std=c11
STDFLAGS_COVERAGE=-fprofile-arcs -ftest-coverage
LFLAGS_COVERAGE=-lgcov --coverage
CHECK_LIBS=-lcheck
SRCS=s21_*.c
OBJS=s21_*.o
OS := $(shell uname -s)
PREF_TEST_SRC=./unit_tests/
TEST_SRC=$(wildcard $(PREF_TEST_SRC)/s21_*.c)

ifeq ($(OS), Linux)
    CHECK_LIBS += -lsubunit -lrt -lpthread -lm
endif

all: s21_decimal.a

build: clean test gcov_report

s21_decimal:
	$(CC) $(CFLAG) $(STDFLAGS) $(SRCS)

test: s21_decimal.a
	$(CC) $(STDFLAGS) ${TEST_SRC} s21_decimal.a -o test $(CHECK_LIBS)
	./test

s21_decimal.a: s21_decimal
	ar rcs s21_decimal.a *.o
	ranlib s21_decimal.a

gcov_report: clean
	$(CC) $(STDFLAGS) ${TEST_SRC} *.c -o test $(CHECK_LIBS) $(LFLAGS_COVERAGE) -lm
	./test 
	lcov -t "gcov" -o gcov.info -c -d .  
	genhtml -o report gcov.info
	open report/index-sort-f.html

leak: 
	leaks -atExit -- ./test

clang-format:
	clang-format -i *.c *.h unit_tests/*.c unit_tests/*.h
	clang-format -n *.c *.h unit_tests/*.c unit_tests/*.h

clean:
	rm -rf *.o *.a *.gcno *.gcda *.gcov *.out *.info out
	rm -rf gcov.info
	rm -rf report
	rm -rf test
