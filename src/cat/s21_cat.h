#ifndef PRINT_H
#define PRINT_H

#define _GNU_SOURCE

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} opt;

typedef struct {
  int aut;
  int count;
  int print;
} remind;

void print_file(char *name, opt flags, remind *col);
void print_symb(int c, opt flags, remind *col);
void v_print(unsigned char c);
void reader(int argc, char *argv[], opt* flags);

#endif