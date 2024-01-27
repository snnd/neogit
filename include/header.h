#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>


#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000

void print_command(int argc, char * const argv[]);

int run_init(int argc, char * const argv[]);
int create_configs(char *username, char *email);

int run_add(int argc, char * const argv[]);
int add_to_staging(char *filepath);

// int run_reset(int argc, char * const argv[]);
// int remove_from_staging(char *filepath);

int run_commit(int argc, char * const argv[]);
int inc_last_commit_ID();
bool check_file_directory_exists(char *filepath);
int commit_staged_file(int commit_ID, char *filepath);
int track_file(char *filepath);
bool is_tracked(char *filepath);
int create_commit_file(int commit_ID, char *message);
int find_file_last_commit(char* filepath);

// int run_checkout(int argc, char *const argv[]);
// int find_file_last_change_before_commit(char *filepath, int commit_ID);
// int checkout_file(char *filepath, int commit_ID);