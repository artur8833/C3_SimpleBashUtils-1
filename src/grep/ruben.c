#include "s21_grep.h"
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int flag_e(int argc, char *argv[], char str[]);
void flag_l(char *line, char *str);
void reader(int argc, char *argv[], opt *options);
void parser(int argc, char *argv[], opt *options);
void generalGerister(int reti);
 
int main(int argc, char *argv[])
{
    opt options = {0};
    regex_t regex;
    parser(argc, argv, &options);
    reader(argc, argv, &options);
 
    return 0;
}

void parser(int argc, char *argv[], opt *options) {
  int opt;
  int option_index = 0;

  while ((opt = getopt(argc, argv, "eivcln")) != -1) {
    
    switch (opt) {
      case 'e':
        options->e = 1;
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
      default:
        printf("Try grep --help \n");
        exit(1);
    }
  }
}


void reader(int argc, char *argv[], opt *options)
{

    // char *str;
    // char *buff;
    char str[128];
    char buff[128];
    FILE *fp = fopen(argv[3], "r");
    size_t len = 0;
    size_t read;
    size_t count = 0;
    size_t line = 0;
    int exit = 0;

    while((fgets(str, 126, fp)) != NULL)
    // while((read = getline(&str, &len, fp)) != -1)
    {
      if (options->e == 1)
      {
        if (flag_e(argc,argv,str))
          printf("%s",str);
      }
      if (options->i == 1)
      {
        strcpy(buff, str);
        flag_l(argv[2], buff);
        if (flag_e(argc, argv, buff))
          printf("%s",str);
      }
      if (options->c == 1) {
        if (flag_e(argc, argv, str))
          count++;
      }
      if (options->n == 1) {
        line++;
        if (flag_e(argc, argv, str))
          printf("%ld:%s", line, str);
      }
      if (options->l == 1) {
        if (flag_e(argc, argv, str) && !exit) {
          exit++;
          printf("%s\n", argv[3]); 
        }
      }
      if (options->v == 1) {
        if (!flag_e(argc, argv, str)) {
          printf("%s",str);
        }
      }
    }

    if (options->c==1) printf("%ld\n", count);
    fclose(fp);
}

int flag_e(int argc, char *argv[], char str[])
{
    regex_t regex;
    int status=0;
    int reti;
    reti = regcomp(&regex, argv[2], 0);
    if (reti) { 
      fprintf(stderr, "Could not compile regex\n"); 
      exit(1); 
    }
    reti = regexec(&regex, str, 0, NULL, 0);

    return !reti;
}

void flag_l(char *line, char *str)
{

  for (int i=0; str[i];i++){
    str[i]=tolower(str[i]);
  }

  for (int i=0; line[i];i++){
    line[i]=tolower(line[i]);
  }
}

void generalGerister(int reti)
{
  if (reti) { fprintf(stderr, "Could not compile regex\n"); exit(1); }
}
