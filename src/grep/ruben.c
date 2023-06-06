void reader(int argc, char **argv, opt *options)
{
    for (int i = optind; i < argc; i++)
    {
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
              
              if((suchPattern(argv, str, options)) && argc > 4) {
                printf("%s:%s", argv[i], str);
                count++;
              }
              else if (suchPattern(argv, str, options)) {
                printf("%s", str);
                count++;
              }
            }

            // if (options->i == 1)
            // {
            //   strcpy(buff, str); 
            //   flag_l(argv[i], buff);
            //   if (suchPattern(argv, buff, options)){
            //     printf("%s",str);
            //   }
            // }
            
            // if (options->n == 1) 
            // {
            //   line++;
            //   if (suchPattern(argv, str, options))
            //   {
            //     printf("%d:%s", line, str);
            //   }
            // }
            
            // if(options->l==1)
            // {
            //   if (suchPattern(argv, str, options))
            //   {
            //     printf("%s\n", argv[i]);
            //   }
            // }
            
            // if (options->v ==1){
            //   if (!suchPattern(argv, str, options)){
            //     printf("%s", str);
            //   }
            // }
            
            // if(options->c)
            // {
            //   if (suchPattern(argv, str, options)){
            //     count++;
            //   }
            // }            
          }

          if (options->c==1)
          {
            printf("%d\n", count);
          }
          fclose(fp);

    }
}

int suchPattern(char **argv, char *str, opt *options)
{
    regex_t regex;
    int reti;
    reti = regcomp(&regex, options.template,  (REG_NEWLINE | REG_EXTENDED));
    generalGerister(reti);
    reti = regexec(&regex, str, 0, NULL, 0);
    regfree(&regex);
    return !reti;
}