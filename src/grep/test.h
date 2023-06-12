#ifndef S21_GREP_H
#define S21_GREP_H

#define BUFSIZE 1024

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;

  int files_count;
  char pattern[BUFSIZE];
  int error;
} Options;

void set_options(Options *options, int argc, char **argv);
int search_for_matches(char *string, char *pattern, int flag);
void print_o(char *string, int flag, Options options, char *filename,
             int line_number);
void s21_grep(Options *options, char *filename);
void set_pattern(char **argv, Options *options);
int set_flags_for_regex(Options options, int flag_);
void regex_cat(Options *options, char *regex);
void regex_from_file(Options *options, char *filename);
void print_filename(Options options, char *filename);
void print_line_number(Options options, int line_number);

#endif