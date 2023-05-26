#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


void CatNoArg(){
    char buf[4096];
    int fd = STDIN_FILENO;
    int bytes_read;
    bytes_read = read(fd, buf, 4096);
    while (bytes_read!=-1)
    {
        printf("%.*s",bytes_read ,buf);
        bytes_read = read(fd, buf, 4096);
    }
}

void CatArgs(int arg, char *avrg[])
{
    (void)arg;
    char buf [4096];
    int fd= open(avrg[1], O_RDONLY);
    int bytes_read;
    bytes_read = read(fd, buf, 4096);
    while (bytes_read>0)
    {
        printf("%.*s",bytes_read ,buf);
        bytes_read = read(fd, buf, 4096);
    }

}

int main(int arg, char *avrg[]){

    (void)avrg;
    if (arg==1)
    {
        CatNoArg();
    }
    else
    {
        CatArgs(arg,avrg);
    }

    return 0;
}
