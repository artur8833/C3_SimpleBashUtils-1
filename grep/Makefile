GCC=gcc
CFLAGC=-c -Wall -Werror -Wextra --std=c11 --pedantic

all: clean s21_grep

s21_grep: s21_grep.o 
	$(GCC) s21_grep.o -o s21_grep

s21_grep.o: s21_grep.c s21_grep.h
	$(GCC) $(CFLAGC) s21_grep.c -o s21_grep.o

clean:
	rm -f *o
	rm -f s21_grep
	clear