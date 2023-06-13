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
  //printf("template=%s\n",options.template);
  
  if (options.f){
    reader(argv, &options);
  }

  if (!options.e){
    strcpy(options.template, argv[optind++]); 
  }
  
  options.num = argc - optind;


  
  while(optind < argc){
    reader(argv, &options);
    optind++;
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
  //printf("oprions=%d\n",options->f);
  if (options->l) options->c = options->o = 0;
  if (options->c) options->o = 0;
  if (options->v && options->o) options->error = 1;
  
}

void reader(char **argv, opt *options)
{  
  //printf("optind=%d\n",optind);
  //printf("f4=%s\n",argv[optind]);    
  FILE *fp;
  if ((fp = fopen(argv[optind], "r")) == NULL) {
    if (!options->s)
      fprintf(stderr, "s21_grep: %s No such file or directory\n", argv[optind]);

  } else {
    char last_char;
    char *str=NULL;
    int len;
    int count=0;
    int linee =0;
    size_t size = 0;
    

    //printf("f2=%d",options->f);
    while((len = getline(&str, &size, fp)) != -1)
    {
      last_char = str[strlen(str) - 1];
      linee++;  
      int match = !suchPattern(str, options); //1-yes
      //printf("match=%d", match);
      //printf("f=%d\n", options->f);
      
      if (match){
        count++;
        //printf("f=%d\n", options->f);
        //printf("f=%d\n", options->f);
        //printf("f=%s\n", str);
      }

      if ((options->v == 1))
      {
        match=!match;
        //printf("f=%d\n", options->f);
      }

      else if (options->f){
        //printf("f1=%d",options->f);
        if (match){
          //printf("match=%d\n",match);
          //printf("%s",str); 
        }
      }

      else if ((options->e) || (options->i) || (options->h))
      {
        if (suchPattern(str, options)) {
          severalFiles(argv[optind], options);
          printf("%s", str);

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
        }
      }

      else if ((options->v == 1))
      {
        match=!match;
      }
      


      
      else if (options->c){
        continue;
      }

      else
      {
        if (suchPattern(str, options)) {
          severalFiles(argv[optind], options);
          printf("%s", str);
        }
      }
      
    }

    //printf("count=%d\n",count);
    
    if ((options->c == 1) && !options->l)
    {
      //severalFiles(argv[optind], options);
      printf("%d\n", count);
    }
    
    fclose(fp);
  }
    // if (options->i){
  //   flag |= REG_ICASE;
  // }
  // if (options->e || options->f || options->o){
  //   flag |= REG_EXTENDED;
  // }
}

int suchPattern(char *str, opt *options)
{
  regex_t regex;
  int reti;
  //int flag;
  // if (options->i){
  //   flag |= REG_ICASE;
  // }
  // if (options->e || options->f || options->o){
  //   flag |= REG_EXTENDED;
  // }
  //printf("done=%s\n",str);
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

void print_o(char *string, int flag, opt *options, char *filename,
             int line_number) {
  int status;
  regex_t reg;
  regcomp(&reg, options->template, flag);
  char *str = string;
  regmatch_t pmatch[100];
  status = regexec(&reg, str, 1, pmatch, 0);
  while (status == 0) {
    printf("%s:", filename);
    printf("%d:", line_number);

    printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so), str + pmatch[0].rm_so);
    str += pmatch[0].rm_eo;
    status = regexec(&reg, str, 1, pmatch, 0);
  }

  regfree(&reg);
}