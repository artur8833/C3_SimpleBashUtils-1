//#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


void CatNoArg()
{
 
}


int main(int arg, char **avrg){

    (void)avrg;
    if (arg==1)
    {
        CatNoArg();
    }


    return 0;
}
