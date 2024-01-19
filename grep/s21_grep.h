#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options{
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int s;
    int h;
    int o;
    int f;
    char template[1024];
    int num;
    int error;
} opt;

int mem_template(opt *options, char *pattern);
void parser(int argc, char **argv, opt *options);
void reader(char **argv, opt *options);
int suchPattern(char *str, opt *options);
void generalGerister(int reti);
void from_flag_f(opt *options, char *name);
void print_o(char *string, opt *options, char *filename,int line_number);


#endif  