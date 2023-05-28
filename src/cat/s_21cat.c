#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>


int same_line_check(const char* str);
void CatNumber (char *name);
int CatNumberEmpty(char* filename);
int tab_check(const char* str);


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


int CatNumberEmpty(char* filename) {
    FILE* fp = fopen(filename, "r");
    int counter = 1;
    char str[4096];
    while (!feof (fp)) {
        
        if (fgets(str, 4096, fp)) {
            if (counter == 1) {
                printf("%6d\t%s", counter, str); 
                counter++;
            }
            else if (same_line_check(str)) {
                printf("%s", str);
            }
            else {
                printf("%6d\t%s", counter, str);
                counter++;
            }
        }
    }

    fclose(fp);

    return 0;
}


void CatEndStr(char* filename) {
    FILE* fp = fopen(filename, "r");
    char *str;
    size_t len = 0;
    size_t read;
    while ((read = getline(&str, &len, fp)) != -1) {
        for (int i = 0; i < read; i++)
            (i == read - 1) ? printf("$%c", str[read - 1]) : printf("%c", str[i]);  
    }

}

int DelTab(char* filename) {
    FILE* fp = fopen(filename, "r");
    int counter = 0;
    char str[4096];
    while (!feof (fp)) 
    {
        if (fgets(str, 4096, fp)) 
        {
            if ((same_line_check(str))&&(counter==0))
            {
                printf("%s", str);
                counter++;
            }

            else if ((same_line_check(str))&&(counter>0)) 
            {
                counter++;
            }
            
            else 
            {
                printf("%s", str);
                counter=0;
            }
        }
    }

    fclose(fp);
    return 0;
}

int PrintTab(char* name)
{
    char c;
    FILE* f = fopen(name, "r");

        while ((c=getc(f))!=EOF) 
        {

            if(c=='\t')
            {
                printf("^I");
            }
            else{
                printf("%c",c);
            }
        }
    printf("\n");
    fclose(f);
}

int same_line_check(const char* str) {
    return !strcmp(str, "\n");
}

int main(int arg, char *avrg[]){
    

    int c;
    int option_index = 0;
    
    print_file(avrg[1]);


    const struct option long_options[]=
    {
        {"number-nonblank", no_argument, 0, 'b'},    
        {"number", no_argument, 0, 'n'},              
        {"squeeze-blank", no_argument, 0, 's'},       
        {0, 0, 0, 0}
    };


    while ((c = getopt_long(arg, avrg, "benstvRTV", long_options, &option_index)) != -1)
    {
        
        switch (c)
        {
            case 'b':
                CatNumberEmpty(avrg[2]);
                break;
            case 'e':
                CatEndStr(avrg[2]);
                break;
            case 'v':

                break;
            case 'E':

                break;
            case 'n':
                CatNumber(avrg[2]);
                break;
             case 's':
                DelTab(avrg[2]);
                break;
            case 't':
                PrintTab(avrg[2]);
                break;
            case 'T':

                break;
            case'?':
                break;
            
            default:
                printf("Try cat help \n");

        }
    }

    return 0;
}
