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
bool tag_exists(char *tag);
void number_of_files(char *path, int *result);
bool is_alias(int argc, char * const argv[]);
bool shortcut_exists(char *shortcut, char message[]);
void show_commit_info(char *commit_id);
bool is_merged(int commit);

void create_configs();
void run_config_username(int argc, char * const argv[]);
void run_config_email(int argc, char * const argv[]);
void run_init(int argc, char * const argv[]);
void run_config_alias(int argc, char * const argv[]);
bool check_command(char command[]);

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
void log_branch(char *branch);
void log_author(char *author);

void run_branch(int argc, char * const argv[]);
void add_branch_to_first_commit(char *branch);

void run_status(int argc, char * const argv[]);
void add_to_last_state(char command[]);
bool is_in_last_state(char *filename);
bool is_in_working_directory(char *path, char *filename);
void calculate_hash(char *filename, char hash[]);
void calculate_hash_in_last_state(char *filename, char hash[]);
bool is_deleted();

void checkout_branch(char *branch);
void checkout_commit(char *number);
void run_checkout(int argc, char * const argv[]);

void run_revert(int argc, char * const argv[]);

void run_tag(int argc, char * const argv[]);

void run_diff(int argc, char * const argv[]);
void trail(char line[]);
bool diff_files(struct dirent *entry1, struct dirent *entry2, char * const argv[]);

void run_merge(int argc, char * const argv[]);
void merge_directory(char *path1, char *path2, char *number);
void branch_head(char *branch, char commit[]);
bool diff_files_merge(struct dirent *entry1, struct dirent *entry2, char path1[], char path2[]);
void add_commit_to_merged(int commit);

void run_set(int argc, char * const argv[]);
void run_replace(int argc, char * const argv[]);