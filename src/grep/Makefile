GCC=gcc
CFLAGC=-c -Wall -Werror -Wextra --std=c11 --pedantic

all: s21_grep

s21_cat: s21_grep.o 
	$(GCC) s21_grep.o -o s21_grep

s21_cat.o: s21_grep.c s21_grep.h
	$(GCC) $(CFLAGC) s21_grep.c -o s21_grep.o


clear:
	clear

clean:
	rm -f *o
	rm -f s21_grep
	clear



