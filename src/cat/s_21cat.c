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

void CatNumber (char *name)
{
    int count=1;
    char c;
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
                printf("%c%6d\t", c, count );
                count++;
            }
            else{
                printf("%c",c);
            }
        }
    printf("\n");
    fclose(f);
}

void CatNumberEmpty (char *name)
{
    int count=1;
    char c;
    FILE* f = fopen(name, "r");

        while ((c=getc(f))!=EOF) 
        {
            if(count==1) 
            {
                printf("%6d\t%c",count,c);
                count++;
            }
            if((c=='\n')$$(stcmp(c,"\0")))
            {   
                printf("%c%6d\t", c, count );
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


    //const struct option long_options[]=
    //{
    //    {"--number-nonblank", no_argument, 0, 'b'},    //B
    //    {"--number", no_argument, 0, 'n'},              //N
    //    {"--squeeze-blank", no_argument, 0, 's'},       //S
    //    {NULL, 0, NULL, 0}
    //};



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
                CatNumber(avrg[2]);
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
