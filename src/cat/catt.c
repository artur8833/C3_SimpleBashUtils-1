
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(char *name) {
  FILE *f = fopen(name, "rt");
    
  if (f != NULL) 
  {
  int c = fgetc(f);
  while (c != EOF) 
  {
    printf("%c",c);
    c = fgetc(f);
  }
  fclose(f);
  }
}

int main() {

    char *name;
    scanf("%s", name);

    print_file(name);

    return 0;
}

