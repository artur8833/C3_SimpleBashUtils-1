#include "s21_cat.h"
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void print_file(char* name);
void CatNumber(char* name);
void CatNumberEmpty(char* filename);
void v_print(char c);
void CatEndStr(char* filename);
int DelTab(char* filename);
void PrintTab(char* name);
int same_line_check(const char* str);




void (int arg, char* argv[], option *config){

  options = {0};

}


int main(int arg, char* argv[]) {
  int c;
  int option_index = 0;
  print_file(argv[1]);

  const struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  while ((c = getopt_long(arg, argv, "benstvRTV", long_options,
                          &option_index)) != -1) {
    switch (c) {
      case 'b':
        CatNumberEmpty(argv[2]);
        break;
      case 'e':
        CatEndStr(argv[2]);
        break;
      case 'v':
        break;
      case 'E':
        CatEndStr(argv[2]);
        break;
      case 'n':
        CatNumber(argv[2]);
        break;
      case 's':
        DelTab(argv[2]);
        break;
      case 't':
        PrintTab(argv[2]);
        break;
      case 'T':
        PrintTab(argv[2]);
        break;
      case '?':
        break;
      default:
        printf("Try cat help \n");
    }
  }

  return 0;
}

void print_file(char* name) {

  
  FILE* f = fopen(name, "r");

  if (f != NULL) {
    char c = fgetc(f);
    while (c != EOF) {
      printf("%c", c);
      c = fgetc(f);
    }
    fclose(f);
  }
}

void CatNumber(char* name) {
  int count = 1;
  char c;
  FILE* f = fopen(name, "r");

  while ((c = getc(f)) != EOF) {
    if (count == 1) {
      printf("%6d\t%c", count, c);
      count++;
    }
    if (c == '\n') {
      printf("%c%6d\t", c, count);
      count++;
    } else {
      printf("%c", c);
    }
  }

  printf("\n");
  fclose(f);
}

void CatNumberEmpty(char* filename) {
  FILE* fp = fopen(filename, "r");
  int counter = 1;
  char str[4096];
  while (!feof(fp)) {
    if (fgets(str, 4096, fp)) {
      if (counter == 1) {
        printf("%6d\t%s", counter, str);
        counter++;
      } else if (same_line_check(str)) {
        printf("%s", str);
      } else {
        printf("%6d\t%s", counter, str);
        counter++;
      }
    }
  }
  fclose(fp);
}

void v_print(char c) {
  if (c & 128) {
    printf("M-");
    c &= ~128;
  }
  if (c < -96) {
    printf("M-^%c", c + 192);
  } else if (c < 0) {
    printf("M-%c", c + 128);
  } else if (c == 9 || c == 10) {
    printf("%c", c);
  } else if (c < 32) {
    printf("^%c", c + 64);
  } else if (c < 127) {
    printf("%c", c);
  } else {
    printf("^?");
  }
}

void CatEndStr(char* filename) {
  FILE* f = fopen(filename, "r");
  char c;
  while ((c = getc(f)) != EOF) {
    if (c == '\n') {
      printf("$\n");
    } else {
      v_print(c);
    }
  }
}

int DelTab(char* filename) {
  FILE* fp = fopen(filename, "r");
  int counter = 0;
  char str[4096];
  while (!feof(fp)) {
    if (fgets(str, 4096, fp)) {
      if ((same_line_check(str)) && (counter == 0)) {
        printf("%s", str);
        counter++;
      }

      else if ((same_line_check(str)) && (counter > 0)) {
        counter++;
      }

      else {
        printf("%s", str);
        counter = 0;
      }
    }
  }

  fclose(fp);
  return 0;
}

void PrintTab(char* name) {
  char c;
  FILE* f = fopen(name, "r");

  while ((c = getc(f)) != EOF) {
    if (c == '\t') {
      printf("^I");
    } else {
      printf("%c", c);
    }
  }
  printf("\n");
  fclose(f);
}

int same_line_check(const char* str) { return !strcmp(str, "\n"); }
