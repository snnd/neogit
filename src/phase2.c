#include "header.h"
int num_of_files = 0;
// maybe opendir to fix the problem with getcwd

void add_to_last_state(char command[])
{
    char help[MAX_COMMAND_LENGTH];
    strcpy(help, command);
    strcat(help, "/.neogit/last_state");
    system(help);
}

bool is_in_last_state(char *filename) 
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/last_state");
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, filename)) {chdir(cwd); return true;}
    }
    chdir(cwd);
    return false;
}

bool is_in_working_directory(char *path, char *filename)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    chdir(path);
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, filename)) {chdir(cwd); return true;}
    }
    chdir(cwd);
    return false;
}

void run_log(int argc, char * const argv[])
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/commits");

    char number[1000];
    FILE *file;
    char line[MAX_LINE_LENGTH];

    int constant;
    if (argc > 2 && !strcmp(argv[2], "-n")) {
        constant = commit_number() - atoi(argv[3]) + 1;
    } else constant = 1;

    for (int i = commit_number(); i >= constant; i--) {
        strcpy(number, "");
        sprintf(number, "%d", i);
        chdir(number);
        file = fopen(".info", "r");
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
        if (i != constant )printf("\n");
        fclose(file);
        chdir("..");
    }
    chdir(cwd);
}

void run_branch(int argc, char * const argv[])
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    chdir(".neogit/branches");
    if (argc == 2) {
        DIR *dir = opendir(".");
        struct dirent *entry;
        
        while ((entry = readdir(dir)) != NULL) {
            if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
            printf("%s\n", entry->d_name);
        }
        closedir(dir);
    } else {
        if (branch_exists(argv[2])) {
            printf("this branch already exists\n");
            return;
        }
        FILE *file = fopen(argv[2], "w");
        fprintf(file, "%d\n", commit_number());
        fclose(file);
    }

    chdir(cwd);
}

void add_commit_to_branch(int commit, char *branch)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/branches");
    FILE *file = fopen(branch, "r");
    FILE *out = fopen("out", "w");
    char line[MAX_LINE_LENGTH];
    fprintf(out, "%d\n", commit);
    while ((fgets(line, sizeof(line), file)) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        fprintf(out, "%s\n", line);
    }
    fclose(file);
    fclose(out);
    remove(branch);
    rename("out", branch);
    chdir(cwd);
}

void run_commit(int argc, char * const argv[])
{
    if (argc < 4 || strcmp(argv[2], "-m")) {
        printf("invalid command\n");
        return;
    }
    
    user_info *user = who();
    if (user == NULL) {
        printf("please add your username and email before committing!\n");
        return;
    }

    if (!any_staged()) {
        printf("no staged file or directory!\n");
        return;
    }

    if (strlen(argv[3]) > 72) {
        printf("commit message too long!\n");
        return;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/commits");

    char number[1000];
    sprintf(number, "%d", commit_number() + 1);
    mkdir(number, 0777);
    char current[MAX_NAME_LENGTH]; current_branch(current);
    add_commit_to_branch(commit_number(), current);
    chdir(number);

    FILE *file = fopen(".info", "w");

    fprintf(file, "id: %s\n", number);

    fprintf(file, "message: %s\n", argv[3]);

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(file, "time: %s", asctime(tm));
    
    fprintf(file, "branch: %s\n", current);
    

    fprintf(file, "username: %s\n", user->username);
    fprintf(file, "email: %s\n", user->email);

    chdir("..");
    chdir("../..");
    DIR *dir = opendir(".");
    struct dirent *entry;
    char path[MAX_PATH_LENGTH];
    char command[MAX_COMMAND_LENGTH];
    char cwd2[MAX_PATH_LENGTH];
    getcwd(cwd2, sizeof(cwd2));
    int files = 0, directories = 0;

    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
        realpath(entry->d_name, path);
        if (is_staged(path) || (is_tracked(path) && !strcmp(number, "1"))) {
            strcpy(command, "");
            strcat(command, "cp ");
            if (is_dir(path)) strcat(command, "-r ");
            chdir(path);
            number_of_files();
            chdir(cwd2);
            strcat(command, path);
            strcat(command, " ");
            strcat(command, cwd2);
            add_to_last_state(command);
            strcat(command, "/.neogit/commits/");
            strcat(command, number);
            system(command);
        }
        else if (is_tracked(path)) {
            strcpy(command, "");
            strcat(command, "cp ");
            if (is_dir(path)) strcat(command, "-r ");
            num_of_files++;
            strcat(command, cwd2);
            strcat(command, "/.neogit/commits/");
            int new_number = atoi(number);
            new_number--;
            char prev[1000];
            sprintf(prev, "%d", new_number);
            strcat(command, prev);
            strcat(command, "/");
            strcat(command, entry->d_name);
            strcat(command, " ");
            strcat(command, cwd2);
            add_to_last_state(command);
            strcat(command, "/.neogit/commits/");
            strcat(command, number);
            system(command);
        }
        else {
            strcpy(command, "");
            strcat(command, "cp ");
            if (is_dir(path)) strcat(command, "-r ");
            strcat(command, path);
            strcat(command, " ");
            strcat(command, cwd2);
            add_to_last_state(command);
        }
    } 

    fprintf(file, "files committed: %d\n", num_of_files);
    num_of_files = 0;
    fclose(file);

    chdir(".neogit");
    file = fopen("staging", "w");
    fclose(file);
    chdir(cwd);
}

void calculate_hash(char filename[], char hash[])
{
    char cmd[MAX_COMMAND_LENGTH] = "md5sum ";
    
    strcat(cmd, filename);
    // puts("here");
    char buf[MAX_HASH_CODE] = {0};
    FILE *fp;
    
    fp = popen(cmd, "r");

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        sscanf(buf, "%s", hash);
    }

    pclose(fp);
}

void calculate_hash_in_last_state(char filename[], char hash[]) 
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/last_state");
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, filename)) {
            calculate_hash(filename, hash);
        }
    }
    closedir(dir);
    chdir(cwd);
}

void run_status(int argc, char * const argv[])
{
    char help[MAX_PATH_LENGTH];
    char path[MAX_PATH_LENGTH];
    char new_hash[MAX_HASH_CODE];
    char old_hash[MAX_HASH_CODE];
    getcwd(path, sizeof(path));
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            realpath(entry->d_name, help);
            printf("%s ", entry->d_name);
            if (is_staged(help)) printf("+");
            else printf("-");
            if (!is_in_last_state(entry->d_name)) {printf("A\n"); continue;}

            go_to_main_address();
            chdir(".neogit/last_state");
            DIR *dir2 = opendir(".");
            while ((entry = readdir(dir2)) != NULL) {
                if (entry->d_type == DT_REG) {
                    realpath(entry->d_name, help);
                    if (!is_in_working_directory(path, entry->d_name)) {printf("D\n"); continue;}
                }
            }
            chdir(path);
            closedir(dir2);

            calculate_hash(entry->d_name, new_hash);
            puts(new_hash);
            calculate_hash_in_last_state(entry->d_name, old_hash);
            puts(old_hash);
            if (strcmp(new_hash, old_hash)) {printf("M\n"); continue;}
            
            printf("N\n");
        }
    }

    closedir(dir);
}

void number_of_files()
{
    char cwd[MAX_PATH_LENGTH];
    char help[MAX_PATH_LENGTH];
    
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        if (entry->d_type == DT_DIR) {
            realpath(entry->d_name, help);
            getcwd(cwd, sizeof(cwd));
            chdir(help);
            number_of_files();
            chdir(cwd);
        } else {
            realpath(entry->d_name, help);
            num_of_files++;
        }
    }

    closedir(dir);
}

void current_branch(char current[])
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    FILE *file = fopen(".neogit/configs/branch", "r");
    fgets(current, sizeof(current), file);
    int length = strlen(current);
    if (length > 0 && current[length - 1] == '\n') current[length - 1] = '\0';
    fclose(file);
    chdir(cwd);
}
