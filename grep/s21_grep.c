#define _GNU_SOURCE
#include "s21_grep.h"

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  opt options = {0};

  if (argc < 2) {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...");
    return 1;
  }

  parser(argc, argv, &options);
  options.num = argc - optind;

  if (!options.e && !options.f) {
    strcpy(options.template, argv[optind++]);
  }

  options.num = argc - optind;

  while (optind < argc) {
    reader(argv, &options);
    optind++;
  }

  return 0;
}

void parser(int argc, char **argv, opt *options) {
  int opt;

  while ((opt = getopt(argc, argv, "e:ivclnshf:o")) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        mem_template(options, optarg);
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 'o':
        options->o = 1;
        break;
      case 'f':
        from_flag_f(options, optarg);
        break;
      case '?':
        fprintf(stderr, "Invalid options in %s\n", argv[0]);
        exit(1);
    }
  }

  if (options->l) options->c = options->o = 0;
  if (options->c) options->o = 0;
  if (options->v && options->o) options->error = 1;
}

void reader(char **argv, opt *options) {
  FILE *fp;
  if ((fp = fopen(argv[optind], "r")) == NULL) {
    if (!options->s)
      fprintf(stderr, "s21_grep: %s No such file or directory\n", argv[optind]);

  } else {
    char last_char;
    char *str = NULL;
    int len;
    int count = 0;
    int linee = 0;
    size_t size = 0;

    while ((len = getline(&str, &size, fp)) != -1) {
      last_char = str[strlen(str) - 1];
      linee++;
      int match = suchPattern(str, options);  // 1-yes

      if (options->i && options->f) {
        options->i = 0;
      }

      if ((options->v)) {
        match = !match;
      }

      if (match) {
        count++;
      }

      if (match && options->l && !options->error) {
        printf("%s\n", argv[optind]);
        break;
      }

      if (options->o && !options->c && !options->l && !options->error) {
        print_o(str, options, argv[optind], linee);
      }

      if (match && !options->c && !options->error && !options->o) {
        if ((options->num > 1) && (!options->h)) {
          printf("%s:", argv[optind]);
        }

        if (options->n) {
          printf("%d:", linee);
        }

        printf("%s", str);
        if (last_char != '\n') printf("\n");
      }
    }

    if ((options->c == 1) && !options->l) {
      if (options->num > 1 && !options->h) printf("%s:", argv[optind]);

      printf("%d", count);

      if (last_char != '\n' || len == -1) printf("\n");
    }

    if (str) free(str);
  }

  if (fp) fclose(fp);
}

int mem_template(opt *options, char *pattern) {
  if ((strlen(options->template) == 0)) {
    strcat(options->template, pattern);
  } else {
    strcat(options->template, "\\|");
    strcat(options->template, pattern);
  }

  return 0;
}

int suchPattern(char *str, opt *options) {
  regex_t regex;
  int result = 0;
  result = regcomp(&regex, options->template,
                   (options->i ? REG_ICASE : 0 | REG_NEWLINE));
  if (result) {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
  }
  result = regexec(&regex, str, 0, NULL, 0);
  regfree(&regex);
  return !result;
}

void from_flag_f(opt *options, char *name) {
  FILE *fp = NULL;
  char *str = NULL;
  // printf("f=%s\n",name);
  if ((fp = fopen(name, "r")) == NULL) {
    fprintf(stderr, "s21_grep: %s No such file or directory\n", name);
  } else {
    int len;
    size_t size = 0;
    while ((len = getline(&str, &size, fp)) != -1) {
      if (str[len - 1] == '\n') {
        str[len - 1] = '\0';
      }
      options->f = 1;
      mem_template(options, str);
    }
    if (str) {
      free(str);
    }
  }
  if (fp) {
    fclose(fp);
  }
}

void print_o(char *string, opt *options, char *filename, int line_number) {
  int status;
  regex_t reg;
  regcomp(&reg, options->template, (options->i ? REG_ICASE : 0 | REG_NEWLINE));
  char *str = string;
  regmatch_t pmatch[100];
  status = regexec(&reg, str, 1, pmatch, 0);
  while (status == 0) {
    if (options->num > 1 && !options->h) printf("%s:", filename);
    if (options->n) printf("%d:", line_number);
    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
           str + pmatch[0].rm_so);
    str += pmatch[0].rm_eo;
    status = regexec(&reg, str, 1, pmatch, 0);
  }

  regfree(&reg);
}