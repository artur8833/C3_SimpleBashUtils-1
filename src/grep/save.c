
#include "s21_grep.h"
#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <ctype.h>

int suchPattern(int argc, char *argv[], char str[]);
void flag_l(char *line, char *str);
void reader(int argc, char *argv[], opt *options);
void parser(int argc, char *argv[], opt *options) ;
void generalGerister(int reti);
void check_file(FILE *fp);
 
int main(int argc, char *argv[])
{

    opt options = {0};
    regex_t regex;
    parser(argc, argv, &options);
    printf("argv: %s\n",argv[optind]);

    if (argv[optind]==NULL)
    {
      printf("No file\n");
      return 0;
    }
  
    if (argc>1)
    {
      reader(argc, argv, &options);
    }
    
    else {
      printf("cat: flags empty and file...\n");
    }
    
    printf("\n");

    return 0;
}

void parser(int argc, char *argv[], opt *options) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivcln")) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        printf("optarg=%s\n",optarg);
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
        printf("Try cat help \n");
        exit(1);
    }
  }
}

void reader(int argc, char *argv[], opt *options)
{

    for (int i=optind+1;i<argc;i++)
    {
      FILE *fp = fopen(argv[i], "r");

      check_file(fp);
      char str[126];
      char buff[126];
      int count=0;
      int line =0;
      while((fgets(str, 126, fp)) != NULL)
          {
            if (options->e==1)
            {
              if((suchPattern(argc,argv,str)) && argc>4){
                printf("%s:%s",argv[i],str);
                count++;
              }
              else if (suchPattern(argc,argv,str))
              {
                printf("%s",str);
                count++;
              }
            }

            if (options->i==1)
            {
              strcpy(buff, str); 
              flag_l(argv[i], buff);
              if (suchPattern(argc, argv, buff)){
                printf("%s",str);
              }
            }
            
            if (options->n == 1) 
            {
              line++;
              if (suchPattern(argc, argv, str))
              {
                printf("%d:%s", line, str);
              }
            }
            
            if(options->l==1)
            {
              if (suchPattern(argc, argv, str) && !exit)
              {
                printf("%s\n",argv[i]);
              }
            }
            
            if (options->v ==1){
              if (!suchPattern(argc,argv,str)){
                printf("%s",str);
              }
            }
            
            if(options->c)
            {
              if (suchPattern(argc,argv,str)){
                count++;
              }
            }            
          }

          if (options->c==1)
          {
            printf("%d\n",count);
          }
          fclose(fp);

    }
}

int suchPattern(int argc, char *argv[], char str[])
{
    regex_t regex;
    int status=0;
    int reti;
    reti = regcomp(&regex, argv[2], 0);
    generalGerister(reti);
    reti = regexec(&regex, str, 0, NULL, 0);
    return !reti;
    regfree(&regex);
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
  if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }
}

void check_file(FILE *fp){
  if (fp==NULL){
    printf("No such file");
    exit(1);
  }
}

