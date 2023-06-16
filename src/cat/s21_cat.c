#include "s21_cat.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  opt flags = {0};

  reader(argc, argv, &flags);
  remind col = {'\n', 0, 0};

  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      print_file(argv[i], flags, &col);
    }
  }
  return 0;
}

void reader(int argc, char *argv[], opt *flags) {
  int c;
  const struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                        {"number", 0, 0, 'n'},
                                        {"squeeze-blank", 0, 0, 's'},
                                        {0, 0, 0, 0}};

  while ((c = getopt_long(argc, argv, "bevnstTE", long_options, 0)) != -1) {
    switch (c) {
      case 'b':
        flags->b = 1;
        break;

      case 'e':
        flags->e = 1;
        flags->v = 1;
        break;

      case 'E':
        flags->e = 1;
        break;

      case 'n':
        flags->n = 1;
        break;

      case 's':
        flags->s = 1;
        break;

      case 't':
        flags->t = 1;
        flags->v = 1;
        break;

      case 'T':
        flags->t = 1;
        break;

      case 'v':
        flags->v = 1;
        break;

      case '?':
        fprintf(stderr, "Error");
        exit(1);
        break;
    }
  }
}

void print_symb(int c, opt flags, remind *col) {
  if (flags.s && col->aut == '\n' && c == '\n' && col->print) {
    return;
  }

  if (col->aut == '\n' && c == '\n') {
    col->print = 1;
  } else {
    col->print = 0;
  }

  if (((flags.n && flags.b == 0) || (flags.b && c != '\n')) &&
      col->aut == '\n') {
    col->count++;
    printf("%6d\t", col->count);
  }
  if (flags.e && c == '\n') {
    printf("$");
  }
  if (flags.t && c == '\t') {
    printf("^");
    c = '\t' + 64;
  }
  if (flags.v) {
    v_print(c);
  } else {
    putchar(c);
  }
  col->aut = c;
}

void v_print(unsigned char c) {
  if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c >= 32 && c < 127) {
    printf("%c", c);
  } else if (c == 127) {
    printf("^?");
  } else if (c >= 128 + 32) {
    printf("M-");
    (c < 128 + 127) ? printf("%c", c - 128) : printf("^?");
  } else {
    (c > 32) ? printf("M-^%c", c - 128 + 64) : printf("^%c", c + 64);
  }
}

void print_file(char *name, opt flags, remind *col) {
  FILE *f = fopen(name, "r");
  if (f != NULL) {
    int c = fgetc(f);

    while (c != EOF) {
      print_symb(c, flags, col);
      c = fgetc(f);
    }
    fclose(f);
  }
}
