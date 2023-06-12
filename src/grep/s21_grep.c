#define  _GNU_SOURCE

#include "s21_grep.h"

#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int mem_template(opt *options, char *pattern) {
  if (strlen(options->template)==0) {
    //printf("str=%ld", strlen(options->template));
    strcat(options->template, pattern);
  } else {
    strcat(options->template, "\\|");
    strcat(options->template, pattern);
  }
    
  return 0;
}
 
int main(int argc, char *argv[])
{    
  opt options = { 0 };
  parser(argc, argv, &options);
    
  if (options.f){
    reader(argv, &options);
  }

  if (!options.e){
    strcpy(options.template, argv[optind++]); 
  }
  options.num = argc - optind;

  while(optind < argc){
    reader(argv, &options);
  }

  return 0;
}

void parser(int argc, char **argv, opt *options) {
  int opt;

  while ((opt = getopt(argc, argv, "e:ivclnshf:o")) != -1) 
  {
    switch (opt) {
      case 'e':
        options->e = 1;
        mem_template(options,optarg);
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
        //printf("optarg=%s", optarg);
        from_flag_f(options,optarg);
        break;
      default:
        printf("Try grep --help \n");
        exit(1);
    }
  }
  
  if (options->l) options->c = options->o = 0;
  if (options->c) options->o = 0;
  if (options->v && options->o) options->error = 1;


}

void reader(char **argv, opt *options)
{  
  printf("optind=%d\n",optind);
  printf("f4=%s\n",argv[3]);    
  FILE *fp;
  if ((fp = fopen(argv[optind], "r")) == NULL) {
    if (options->s != 1)
      printf("s21_grep: %s: No such file or directory\n", argv[optind]);
    
    exit(1);
  } else {
    char last_char;
    char *str=NULL;
    int len;
    int count=0;
    int linee =0;
    size_t size = 0;
    printf("f2=%d",options->f);
    while((len = getline(&str, &size, fp)) != -1)
    {
      last_char = str[strlen(str) - 1];

      if ((options->e) || (options->i) || (options->h))
      {
        if (suchPattern(str, options)) {
          severalFiles(argv[optind], options);
          printf("%s", str);
          count++;

          if (last_char != '\n')
            printf("\n");
        }
      }
          
      else if (options->n == 1) 
      {
        linee++;
        if (suchPattern(str, options))
        {
          severalFiles(argv[optind], options);
          
          printf("%d:%s", linee, str);
        }
      }
          
      else if(options->l == 1)
      {
        if (suchPattern(str, options) && (count==0))
        {
          printf("%s\n", argv[optind]);
          count++;
        }
      }

      else if (options->v == 1) 
      {
        if (!suchPattern(str, options))
        {
          severalFiles(argv[optind], options);
          
          printf("%s", str);

          if (last_char != '\n')
            printf("\n");
        }
      }
          
      else if(options->c)
      {
        if (suchPattern(str, options)){
          count++;
        }
      }

      else if(options->f){
        //printf("f1=%d",options->f);
        if (suchPattern(str, options)){
          printf("%s",str);
        }
      }

      else
      {
        if (suchPattern(str, options)) {
          severalFiles(argv[optind], options);
          printf("%s", str);
        }
      }
    }
        
    if (options->c == 1)
    {
      severalFiles(argv[optind], options);
      printf("%d\n", count);
    }
    fclose(fp);
  }

  optind++;
}

int suchPattern(char *str, opt *options)
{
  regex_t regex;
  int reti;
  reti = regcomp(&regex, options->template, options->i ? REG_ICASE | REG_EXTENDED : 0 | REG_NEWLINE);
  generalGerister(reti);
  reti = regexec(&regex, str, 0, NULL, 0);
  regfree(&regex);
  return !reti;
}

void severalFiles(char *argv, opt *options) {
  if (options->num > 1 && !options->h)
    printf("%s:", argv);
}

void generalGerister(int reti)
{
  if (reti) { 
    fprintf(stderr, "Could not compile regex\n"); 
    exit(1); 
  }
}

void from_flag_f(opt *options, char *name){
  FILE *fp=NULL;
  char *str=NULL;
  //printf("f=%s\n",name);
  if((fp=fopen(name ,"r"))==NULL){
    fprintf(stderr, "s21_grep: %s No such file or directory\n", name);
  }
  else{
    int len;
    size_t size = 0;
    while((len=getline(&str,&size,fp))!=-1)
    {
      if(str[len-1]=='\n')
      {
        str[len-1]='\0';
      }
      options->f=1;
      mem_template(options,str);
    }
    if (str)
    {
      free(str);
    } 
  }
  if (fp){
    fclose(fp);
  }

}