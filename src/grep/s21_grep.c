#include <sys/types.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

 
int main(int argc, char *argv[])
{
        regex_t regex;
        int reti;
        char msgbuf[100];
        char strok [1024];
        char strok_file[4096];

        reti = regcomp(&regex, argv[1], 0);
        if( reti ){ fprintf(stderr, "Could not compile regex\n"); exit(1); }
 
        FILE *fp = fopen(argv[2], "r");
        char str[128];
        if ((fp)==NULL) 
        {
            printf("Cannot open file.\n");
            exit (1);
        }
        while((fgets(str, 126, fp)) != NULL)
        { 
                reti = regexec(&regex, str, 0, NULL, 0);
                if (!reti)
                {
                    printf("%s",str);

                 }
        }
        fclose(fp);
    return 0;
}

