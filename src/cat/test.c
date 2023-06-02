#include "s_21cattest.h"
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  opt options = {0};

  parser(argc, argv, &options);
  
  if (argv[optind]==NULL)
  {
    printf("No file\n");
    return 0;
  }
  
  if (argc > 1) 
  {
    reader(argc, argv, &options);
  } 
  else {
    printf("cat: flags empty and file...\n");
  }
  
  return 0;
}

void parser(int argc, char *argv[], opt *options) {
  int error = 0;
  int opt;
  int option_index = 0;
  const struct option long_options[] = {
      {"number-nonblank", no_argument, 0, 'b'},
      {"number", no_argument, 0, 'n'},
      {"squeeze-blank", no_argument, 0, 's'},
      {0, 0, 0, 0}};

  while ((opt = getopt_long(argc, argv, "beEnstvTV", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->v = 1;
        options->e = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->v = 1;
        options->t = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      default:
        printf("Try cat help \n");
    }
  }
}


void reader(int argc, char *argv[], opt *options) {
  for (int i = optind; i < argc; i++) 
  { 
    
    FILE *file_name = fopen(argv[i], "r");
    if (file_name!=NULL) 
    {
      int start_line = 1;
      int count_empty_line = 1;
      int count_all_line = 1;
      char previous_char = '\n';
      char next_char = ' ';
      char c;

      while ((c = fgetc(file_name)) != EOF) {
        if (options->e == 1) 
        {
          flag_e(c);
        }
      }

    fclose(file_name);
    } else {
      printf("No open file\n");
    }

  }
}

void flag_e(char c) {
    if (c == '\n') {
      printf("$\n");
    } else {
      v_print(c);
    }
}

int flag_n(int start_line, int *count_all_line, char ch) {
  if (start_line) {
    printf("%6d\t", (*count_all_line)++);
  }
  return ch == '\n';
}

int flag_s(char previous_char, char next_char, char ch) {
  return ch == '\n' && previous_char == '\n' && next_char == '\n';
}

int flag_b(char previous_char, int *count_empty_line, char ch) {
  if (previous_char == '\n' && ch != '\n') {
    printf("%6d\t", (*count_empty_line)++);
  }
  return ch == '\n';
}

int flag_t(char ch) {
  int t = 0;
  if (ch == '\t') {
    t = printf("^I");
  }
  return t;
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



cat -e -benstvET tests/test_1_cat.txt tests/test_2_cat.txt