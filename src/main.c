#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include "file_functions.h"


#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000

void print_command(int argc, char * const argv[]);
int run_init(int argc, char * const argv[]);

int run_init(int argc, char * const argv[]) {
    char cwd[MAX_FILENAME_LENGTH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
    
    bool exists = false;
    char tmp_dir[MAX_FILENAME_LENGTH];
    struct dirent *entry;
    do {
        DIR *dir = opendir(".");
        if (dir == NULL) return 1;

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && !strcmp(entry->d_name, ".neogit")) {
                exists = true;
            }
        }
        
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL) return 1;
        if (strcmp(tmp_dir, "/")) {
            if (chdir("..") != 0) return 1;
        }

        printf("%s\n", tmp_dir);

    } while (strcmp(tmp_dir, "/"));

    
    if (exists) {
        perror("neogit repo already exists!");
    } else {
        // TODO
    }
    return 0;
}

void print_command(int argc, char * const argv[]) {
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s ", argv[i]);
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
        run_init(argc, argv);
    }

    return 0;
}