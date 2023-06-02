#include "s21_cat.h"
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
  int opt;
  int option_index = 0;
  const struct option long_options[] = {
      {"number-nonblank", 0, 0, 'b'},
      {"number", 0, 0, 'n'},
      {"squeeze-blank", 0, 0, 's'},
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
        exit(1);
    }
  }
}

void reader(int argc, char *argv[], opt *options) 
{  
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
      char ch[1024] = {'\0'};

      while ((*ch = fgetc(file_name)) != EOF) {
        if (options->s == 1 && flag_s(previous_char, next_char, *ch)) {
          continue;
        }
        if (options->n == 1 && options->b==0) {
          start_line = flag_n(start_line, &count_all_line, *ch);
        }

        if (options->b ==1) {
          flag_b(previous_char, &count_empty_line, *ch);
        }

        if (options->e == 1) {
          flag_e(*ch);
        }
        if (options->v == 1) {
          flag_v(ch);
        }

        next_char = previous_char;
        previous_char = *ch;

        if (options->t == 1 && flag_t(*ch)) {
          continue;
        }

        if (*ch == '\0') {
          fputc(*ch, stdout);
        } else {
          fputs(ch, stdout);
        }
        memset(ch, '\0', 1024);
      }

    fclose(file_name);
    } else {
      printf("No open file\n");
    }

  }
}

int flag_e(char ch) {
  if (ch == '\n') {
    printf("$");
  }
  return ch == '\n';
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

int flag_v(char *ch) {
  unsigned char tmp = *ch;
  if ((tmp <= 8) || (tmp >= 11 && tmp <= 31) || (tmp >= 127 && tmp <= 159))
    strcpy(ch, special_symbol[tmp]);
  return tmp;
}
