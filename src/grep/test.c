#define _GNU_SOURCE
#include "test.h"

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
  Options options = {0};

  if (argc < 2) {
    fprintf(stderr, "Usage: s21_grep [OPTION]... PATTERNS [FILE]...");
    return 1;
  }

  set_options(&options, argc, argv);
  set_pattern(argv, &options);
  printf("\n%s\n",options.pattern);
  options.files_count = argc - optind;

  while (optind < argc) {
    s21_grep(&options, argv[optind++]);
  }

  return 0;
}

void set_options(Options *options, int argc, char **argv) {
  char short_options[] = "e:ivclnhsf:o";

  int flag;
  while ((flag = getopt(argc, argv, short_options)) != -1) {
    switch (flag) {
      case 'e':  // шаблон
        regex_cat(options, optarg);
        options->e = 1;
        break;
      case 'i':  // игнор различий регистра
        options->i = 1;
        break;
      case 'v':  // инвертирует смысл поиска соответствий
        options->v = 1;
        break;
      case 'c':  // выводим только количество совпадающих строк
        options->c = 1;
        break;
      case 'l':  // выводит только совпадающие файлы
        options->l = 1;
        break;
      case 'n':  // добавляет номер строки при выводе
        options->n = 1;
        break;
      case 'h':  // выводит совпадающие строки без имен файлов
        options->h = 1;
        break;
      case 's':  // подавляет сообщения об ошибках
        options->s = 1;
        break;
      case 'f':  // получает регулярки из файла
        regex_from_file(options, optarg);
        break;
      case 'o':  // печатает только совпадающие непустые части строки
        options->o = 1;
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

void set_pattern(char **argv, Options *options) {
  if (!options->e && !options->f) {
    strcpy(options->pattern, argv[optind++]);
  }
}

int set_flags_for_regex(Options options, int flag_) {
  int flag = flag_;
  
  if (options.i) {  // I - FLAG
    flag |= REG_ICASE;
  }

  if (options.e || options.f || options.o) {
    flag |= REG_EXTENDED;
  }
  return flag;
}

void regex_cat(Options *options, char *regex) {
  if (!options->e && !options->f) 
  {
    strcpy(options->pattern, regex);
  } 
  else
   {
    strcat(options->pattern, "|");
    strcat(options->pattern, regex);
  }

}

void regex_from_file(Options *options, char *filename) {
  FILE *fp = NULL;
  char *line = NULL;

  if ((fp = fopen(filename, "r")) == NULL) {
    if (!options->s)
      fprintf(stderr, "s21_grep: %s No such file or directory\n", filename);
  } else {
    // getline variables
    int len = 0;
    size_t size = 0;

    while ((len = getline(&line, &size, fp)) != -1) 
    {
      if (line[len - 1] == '\n') line[len - 1] = '\0';
      regex_cat(options, line);
      options->f = 1;
    }
    if (line) free(line);
  }

  if (fp) fclose(fp);
}

void s21_grep(Options *options, char *filename) {
  int flag = set_flags_for_regex(*options, REG_NEWLINE);
  FILE *fp;

  // printf("%s\n", options->pattern);

  if ((fp = fopen(filename, "r")) == NULL) {
    if (!options->s)
      fprintf(stderr, "s21_grep: %s No such file or directory\n", filename);

  } else {
    int line_number = 0;
    int count_match_lines = 0;

    // getline variables
    int len;
    char *line = NULL;
    size_t size = 0;
    char last_char;

    while ((len = getline(&line, &size, fp)) != -1) {
      last_char = line[strlen(line) - 1];
      line_number++;

      int is_match =!search_for_matches(line, options->pattern, flag);  // 1 - yes; 0 - no

      if (options->v) is_match = !is_match;  // V FLAG

      if (is_match){
        count_match_lines++;
        //printf("count=%d\n", count_match_lines);
      }   // counter

      if (is_match && options->l && !options->error) {  // L FLAG
        printf("%s\n", filename);
        break;
      }
      if (options->o && !options->c && !options->l &&
          !options->error) {  // O FLAG
        print_o(line, flag, *options, filename, line_number);
      }

      if (is_match && !options->c && !options->error && !options->o) {  // match

        print_filename(*options, filename);
        print_line_number(*options, line_number);

        printf("%s", line);  // last new line
        if (last_char != '\n') printf("\n");
      }
    }

    if (options->c && !options->l) {  // C FLAG H FLAG
      // last_char = line[len - 1];
  
      if (options->files_count > 1 && !options->h) printf("%s:", filename);

      printf("%d", count_match_lines);
      //  printf("%d\n", line[0]);
      if (last_char != '\n' || len == -1) printf("\n");
    }

    if (line) free(line);
  }  // else = file access

  if (fp) fclose(fp);
}

int search_for_matches(char *string, char *pattern, int flag) {
  regex_t reg;
  int status = regcomp(&reg, pattern, flag);
  if (!status) status = regexec(&reg, string, 0, NULL, 0);
  regfree(&reg);
  return status;
}

void print_o(char *string, int flag, Options options, char *filename,
             int line_number) {
  int status;
  regex_t reg;
  regcomp(&reg, options.pattern, flag);
  char *str = string;
  regmatch_t pmatch[100];
  status = regexec(&reg, str, 1, pmatch, 0);
  while (status == 0) {
    print_filename(options, filename);
    print_line_number(options, line_number);

    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so), str + pmatch[0].rm_so);
    str += pmatch[0].rm_eo;
    status = regexec(&reg, str, 1, pmatch, 0);
  }

  regfree(&reg);
  // return status;
}

void print_filename(Options options, char *filename) {
  if (options.files_count > 1 && !options.h) {  // files_count H FLAG
    printf("%s:", filename);
  }
}

void print_line_number(Options options, int line_number) {
  if (options.n) {  // N FLAG
    printf("%d:", line_number);
  }
}