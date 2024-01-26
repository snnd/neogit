#include <stdio.h>
#include "file_functions.h"

//#define _DEB

#ifdef _DEB
int main() {
    int argc = 2;
    char *argv[] = {"neogit", "init"};

#else
int main(int argc, char **argv)
{
#endif
    printf("Hello world!\n");
    printf("%d\n", argc);
    return 0;
    
}