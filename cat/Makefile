GCC=gcc
CFLAGC=-c -Wall -Werror -Wextra --std=c11 --pedantic

all: clean s21_cat

s21_cat: s21_cat.o 
	$(GCC) s21_cat.o -o s21_cat

s21_cat.o: s21_cat.c s21_cat.h
	$(GCC) $(CFLAGC) s21_cat.c -o s21_cat.o

clean:
	rm -f *o
	rm -f s21_cat
	clear