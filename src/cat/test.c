#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>


int check_same_str(const char* c);
int same_line_check(const char* str);

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

int check_same_str(const char* c){
    return !strcmp(c, "/n");
}



void EndStr(char *name)
{
    FILE* f = fopen(name, "r");
    char c[4096];
    

        if (fgets(c, 4096, f)) {

            if (same_line_check(c)) {
                printf("%s", c);
            }
            else {
                printf("%s", c);
                
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

