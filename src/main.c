#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "file_functions.h"

void print_command(int argc, char * const argv[]) {
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s", argv[i]);
    }
    fprintf(stdout, "\n");
}

//#define _DEB

#ifdef _DEB
int main() {
    int argc = 2;
    char *argv[] = {"neogit", "init"};

#else
int main(int argc, char *argv[])
{
#endif
    
    if (argc < 2) {
        perror("please enter a valid command");
        return 1;
    }

    if (!strcmp(argv[1], "init")) {
        print_command(argc, argv);
    }

    return 0;
}