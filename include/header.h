#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000
#define MAX_NAME_LENGTH 1000
#define MAX_PATH_LENGTH 2000

void print_command(int argc, char * const argv[]);
int go_to_main_address();
int is_dir(char *path);

int run_init(int argc, char * const argv[]);
int create_configs();

int run_config_username(int argc, char * const argv[]);
int run_config_email(int argc, char * const argv[]);

int run_add(int argc, char * const argv[]);
int add_to_staging(char *filepath);
int is_staged(char *path);
void add_space(int depth, int first_depth);
int run_add_n_recursive(int depth, int first_depth);
int run_add_n(int argc, char * const argv[]);
int run_add_f(int argc, char * const argv[]);

int run_reset(int argc, char * const argv[]);