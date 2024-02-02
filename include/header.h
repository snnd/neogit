#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

#define MAX_FILENAME_LENGTH 4000
#define MAX_COMMIT_MESSAGE_LENGTH 4000
#define MAX_LINE_LENGTH 4000
#define MAX_MESSAGE_LENGTH 4000
#define MAX_NAME_LENGTH 4000
#define MAX_PATH_LENGTH 4000
#define MAX_COMMAND_LENGTH 4000
#define MAX_HASH_CODE 4000

typedef struct {
    char username[MAX_NAME_LENGTH];
    char email[MAX_NAME_LENGTH];
} user_info;

bool neogit_exists();

void iterate(void *func(char *path));
void write_path_to_file(char *path);
void remove_path_from_file(char *path);
bool is_dir(char *path);
bool is_file(char *path);
void go_to_main_address();
void current_branch(char current[]);
bool branch_exists(char *branch);
void number_of_files(char *path, int *result);

void create_configs();
void run_config_username(int argc, char * const argv[]);
void run_config_email(int argc, char * const argv[]);
void run_init(int argc, char * const argv[]);

bool is_staged(char *path);
bool any_staged();
void add_to_staging(char *path);
void run_add(int argc, char * const argv[]);
void add_space(int depth, int first_depth);
void run_add_n_recursive(int depth, int first_depth);

void remove_from_staging(char *path);
void run_reset(int argc, char * const argv[]);
void reset_undo();

void write_path_to_tracks(char *path);
bool is_tracked(char *path);

int commit_number();
void run_commit(int argc, char * const argv[]);
void add_commit_to_branch(int commit, char *branch);

void run_log(int argc, char * const argv[]);

void run_branch(int argc, char * const argv[]);

void run_status(int argc, char * const argv[]);
void add_to_last_state(char command[]);
bool is_in_last_state(char *filename);
bool is_in_working_directory(char *path, char *filename);
void calculate_hash(char *filename, char hash[]);
void calculate_hash_in_last_state(char *filename, char hash[]);
bool is_deleted(char *path, char *filename);

void checkout_branch(char *branch);
void checkout_commit(char *number);
void run_checkout(int argc, char * const argv[]);