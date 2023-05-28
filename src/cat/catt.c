#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>



void print_file(char *name)
{
 
    FILE *f = fopen(name, "r");
        
    if (f != NULL) 
    {
        char c = fgetc(f);
        while (c != EOF) 
        {
            printf("%c",c);
            c = fgetc(f);
        }
        fclose(f);
    }
}


void CatNumberEmpty (char *name)
{
    int count=1;
    char c;
    int cc=0;
    FILE* f = fopen(name, "r");

        while ((c=getc(f))!=EOF) 
        {
            if(count==1) 
            {
                printf("%6d\t%c",count,c);
                count++;
            }
            if(c=='\n')
            {
                printf("%c", c );
                count++;
            }
            else{
                printf("%c",c);
            }
        }
    printf("\n");
    fclose(f);
}

int main(int arg, char *avrg[]){

    int c;

    print_file(avrg[1]);

    while ((c = getopt_long(arg, avrg, "benstvRTV", NULL, NULL)) != -1)
    {
        
        switch (c)
        {
            case 'b':
                CatNumberEmpty(avrg[2]);
                break;
            case 'e':

                break;
                
            case 'v':

                break;
            case 'E':

                break;
            case 'n':

                break;
             case 's':

                break;
            case 't':

                break;
            case 'T':

                break;
            default:
                printf("Try cat help \n");

        }
    }


    return 0;
}