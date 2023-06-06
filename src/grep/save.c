#include "s21_grep.h"
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>


int mem_template(opt *options);
void parser(int argc, char **argv, opt *options) ;
void reader(int argc, char **argv, opt *options, char *template);
int suchPattern(char **argv, char *str, char *template, opt *options)
void flag_l(char *line, char *str);
void generalGerister(int reti);
void check_file(FILE *fp);

int mem_template(opt *options) {
  if (strlen(options->template)!=0) {
    strcat(options->template, "|");
    strcat(options->template, optarg);
  } else {
    strcat(options->template, optarg);
  }
  return 0;
}
 
int main(int argc, char *argv[])
{
    opt options = {0};
    char *template = NULL;
    parser(argc, argv, &options);

    
    if (argv[optind]==NULL)
    {
      printf("No file\n");
      return 0;
    }
    
    
    if (argc > 1)
    {
      template = options.template;
      reader(argc, argv, &options, template);
    }
    
    else {
      printf("grep: flags empty and file...\n");
    }

    return 0;
}

void parser(int argc, char **argv, opt *options) {
  int opt;

  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        mem_template(options);
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
    printf("tem=%s\n",options->template);
  }
}

void reader(int argc, char **argv, opt *options, char *template)
{
    //printf("done");
    int num=0;
    
    if (argv[optind]==argv[1]){
      num = 2;
    }
    else{
      num=optind;
    }
    printf("done\n");
    printf("optind=%d\n",optind);
    for (int i = num; i < argc; i++)
    {
      
      printf("%s\n",argv[i]);

      FILE *fp = fopen(argv[i], "r");
      check_file(fp);
      char str[1024];   
      char buff[1024];
      int count=0;
      int line =0;

      while((fgets(str, 1024, fp)) != NULL)
          {
            if (options->e == 1)
            {
              if((suchPattern(argv, str, template)) && (argc-optind)>1) {
                printf("%s:%s", argv[i], str);
                count++;
              }
              else if (suchPattern(argv, str, template,options)) {
                printf("%s", str);
                count++;
              }
            }

            else if (options->i == 1)
            {
              strcpy(buff, str); 
              flag_l(argv[i], buff);
              if (suchPattern(argv, buff,template,options)){
                printf("%s",str);
              }
            }
            
            else if (options->n == 1) 
            {
              line++;
              if (suchPattern(argv, str,template, options))
              {
                printf("%d:%s", line, str);
              }
            }
            
            else if(options->l==1)
            {
              if (suchPattern(argv, str,template,options))
              {
                printf("%s\n", argv[i]);
              }
            }
            
            else if (options->v ==1){
              if (!suchPattern(argv, str,template,options)){
                printf("%s", str);
              }
            }
            
            else if(options->c)
            {
              if (suchPattern(argv, str,template,options)){
                count++;
              }
            }

            else
            {
              if(suchPattern(argv, str, argv[1],options)){
                printf("%s", str);
              }
            }            
          }


          if (options->c==1)
          {
            printf("%d\n", count);
          }
          fclose(fp);
    }
    
}

int suchPattern(char **argv, char *str, char *template, opt *options)
{

    regex_t regex;
    int reti;
    reti = regcomp(&regex, template, options->i? REG_ICASE : 0 | REG_NEWLINE options->e? REG_EXTENDED);
    generalGerister(reti);
    reti = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return !reti;
}

void flag_l(char *line, char *str)
{
  for (int i=0; str[i];i++){
    str[i] = tolower(str[i]);
  }

  for (int i = 0; line[i]; i++){
    line[i] = tolower(line[i]);
  }
}

void generalGerister(int reti)
{
  if(reti) { 
    fprintf(stderr, "Could not compile regex\n" 
  ); 
    exit(1); 
  }
}

void check_file(FILE *fp){
  if (fp==NULL){
    printf("No such file");
    exit(1);
  }
}