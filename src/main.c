#include "header.h"

void iterate(void *func(char *path))
{
    char cwd[MAX_PATH_LENGTH];
    char help[MAX_PATH_LENGTH];
    
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        if (entry->d_type == DT_DIR) {
            realpath(entry->d_name, help);
            func(help);
            getcwd(cwd, sizeof(cwd));
            chdir(help);
            iterate(func);
            chdir(cwd);
        } else {
            realpath(entry->d_name, help);
            func(help);
        }
    }

    closedir(dir);
}

void write_path_to_file(char *path)
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/staging", "a");
    fprintf(file, "%s\n", path);
    fclose(file);
    chdir(cwd);
}

void remove_path_from_file(char *path)
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/staging", "r");
    FILE *out = fopen(".neogit/out", "w");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        if (strcmp(path, line)) fprintf(out, "%s\n", line);
    }
    
    fclose(file);
    fclose(out);

    remove(".neogit/staging");
    rename(".neogit/out", ".neogit/staging");

    chdir(cwd);
}

void write_path_to_tracks(char *path)
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/tracks", "a");
    fprintf(file, "%s\n", path);
    fclose(file);
    chdir(cwd);
}

void create_configs() 
{
    FILE *file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/staging", "w");
    fclose(file);

    mkdir(".neogit/commits", 0777);
    
    mkdir(".neogit/last_state", 0777);

    mkdir(".neogit/branches", 0777);
    file = fopen(".neogit/branches/main", "w");
    fclose(file);

    mkdir(".neogit/tags", 0777);

    mkdir(".neogit/configs", 0777);

    file = fopen(".neogit/configs/address", "w");
    char address[MAX_PATH_LENGTH];
    getcwd(address, sizeof(address));
    fprintf(file, "%s\n", address);
    fclose(file);

    file = fopen(".neogit/configs/username", "w");
    fclose(file); 

    file = fopen(".neogit/configs/email", "w");
    fclose(file); 

    file = fopen(".neogit/configs/alias", "w");
    fclose(file); 

    file = fopen(".neogit/configs/shortcuts", "w");
    fclose(file); 

    file = fopen(".neogit/configs/branch", "w");
    fprintf(file, "main\n");
    fclose(file);    
}

void run_config_username(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    if (!strcmp(argv[2], "--global") && argc == 5) {
        FILE *file = fopen("/home/sana_niroomand/project/include/configs/username", "w");
        fprintf(file, "%s\n", argv[4]);
        fclose(file);
    }
    else if (strcmp(argv[2], "--global") && argc == 4) {
        FILE *file = fopen(".neogit/configs/username", "w");
        fprintf(file, "%s\n", argv[3]);
        fclose(file);
    }
    else {
        printf("invalid command\n");
    }

    chdir(cwd);

}

void run_config_email(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    if (!strcmp(argv[2], "--global") && argc == 5) {
        FILE *file = fopen("/home/sana_niroomand/project/include/configs/email", "w");
        fprintf(file, "%s\n", argv[4]);
        fclose(file);
    }
    else if (strcmp(argv[2], "--global") && argc == 4) {
        FILE *file = fopen(".neogit/configs/email", "w");
        fprintf(file, "%s\n", argv[3]);
        fclose(file);
    }
    else {
        printf("invalid command\n");
    }

    chdir(cwd);
}

bool check_command(char copy_command[])
{
    int argc = 0;
    char *argv[MAX_COMMAND_LENGTH];
    char command[MAX_COMMAND_LENGTH];
    strcpy(command, copy_command);
    argv[argc++] = strtok(command, " ");
    while ((argv[argc] = strtok(NULL, " ")) != NULL) argc++;

    if (argc < 2) {
        printf("please enter a valid command\n");
        return false;
    }
    if (!strcmp(argv[1], "config")) {
        if (argc < 4) {
            printf("please enter a valid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "add")) {
        if (argc < 3) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "reset")) {
        if (argc < 3) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "commit")) {
        if (argc < 4 || strcmp(argv[2], "-m")) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "checkout")) {
        if (argc < 3) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "revert")) {
        if (argc < 3) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "diff")) {
        if (argc < 5) {
            printf("invalid command\n");
            return false;
        }
    }
    else if (!strcmp(argv[1], "merge")) {
        if (argc < 5) {
            printf("invalid command\n");
            return false;
        }
    }

    return true;
}

void run_config_alias(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    if (!strcmp(argv[2], "--global") && argc == 5) {
        if (!check_command(argv[2])) return;
        FILE *file = fopen("/home/sana_niroomand/project/include/configs/alias", "a");
        strtok(argv[3], ".");
        strcpy(argv[3], strtok(NULL, "."));
        fprintf(file, "neogit %s\n", argv[3]);
        fprintf(file, "%s\n", argv[4]);
        fclose(file);
    }
    else if (strcmp(argv[2], "--global") && argc == 4) {
        if (!check_command(argv[3])) return;
        FILE *file = fopen(".neogit/configs/alias", "a");
        strtok(argv[2], ".");
        strcpy(argv[2], strtok(NULL, "."));
        fprintf(file, "neogit %s\n", argv[2]);
        fprintf(file, "%s\n", argv[3]);
        fclose(file);
    }
    else {
        printf("invalid command\n");
    }

    chdir(cwd);
}

bool shortcut_exists(char *shortcut, char message[])
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    char line[MAX_LINE_LENGTH];
    int n;

    FILE *file = fopen(".neogit/configs/shortcuts", "r");
    int c = fgetc(file);
    fclose(file);

    if (c == EOF) {
        chdir(cwd);
        return false;
    } else {
        n = 0;
        file = fopen(".neogit/configs/shortcuts", "r");
        while (fgets(line, sizeof(line), file) != NULL) {
            int length = strlen(line);
            if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
            n++;
            if (n % 2 == 0) continue;
            if (!strcmp(shortcut, line)) {
                chdir(cwd); 
                fgets(line, sizeof(line), file);
                length = strlen(line);
                if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
                strcpy(message, line);
                return true;
            }
        }
        fclose(file);
        chdir(cwd);
    }

    return false;
}

void run_set(int argc, char * const argv[])
{
    if (argc < 6) {
        printf("invalid command\n");
        return;
    }

    if (strlen(argv[3]) > 72) {
        printf("shortcut message too long\n");
        return;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/configs/shortcuts", "a");
    fprintf(file, "%s\n", argv[5]);
    fprintf(file, "%s\n", argv[3]);
    fclose(file);

    chdir(cwd);
}

void run_replace(int argc, char * const argv[])
{
    if (argc < 6) {
        printf("invalid command\n");
        return;
    }

    if (strlen(argv[3]) > 72) {
        printf("new shortcut message is too long\n");
        return;
    }

    char message[MAX_MESSAGE_LENGTH];
    if (!shortcut_exists(argv[5], message)) {
        printf("shortcut doesn't exist\n");
        return;
    }

    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/configs");

    char line[MAX_LINE_LENGTH];
    int n;

    FILE *file = fopen("shortcuts", "r");
    FILE *out = fopen("out", "w");

    n = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        n++;
        if (n % 2 == 1) {
            if (!strcmp(argv[5], line)) {
                fprintf(out, "%s\n", line);
                fgets(line, sizeof(line), file);
                length = strlen(line);
                if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
                fprintf(out, "%s\n", argv[3]);
                n++;
            } else fprintf(out, "%s\n", line);
        } else fprintf(out, "%s\n", line);
    }
    fclose(file); fclose(out);
    remove("shortcuts");
    rename("out", "shortcuts");
    chdir(cwd);
}

void run_remove(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return;
    }

    char message[MAX_MESSAGE_LENGTH];
    if (!shortcut_exists(argv[3], message)) {
        printf("shortcut doesn't exist\n");
        return;
    }

    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/configs");

    char line[MAX_LINE_LENGTH];
    int n;

    FILE *file = fopen("shortcuts", "r");
    FILE *out = fopen("out", "w");

    n = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        n++;
        if (n % 2 == 1) {
            if (!strcmp(argv[3], line)) {
                fgets(line, sizeof(line), file);
                n += 2;
            }
            else fprintf(out, "%s\n", line);
        } else fprintf(out, "%s\n", line);
    }
    fclose(file); fclose(out);
    remove("shortcuts");
    rename("out", "shortcuts");
    chdir(cwd);
}

void run_init(int argc, char * const argv[]) 
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    
    bool exists = false;
    char tmp_dir[MAX_FILENAME_LENGTH];
    struct dirent *entry;
    do {
        DIR *dir = opendir(".");

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && !strcmp(entry->d_name, ".neogit")) {
                exists = true;
            }
        }
        closedir(dir);
        
        getcwd(tmp_dir, sizeof(tmp_dir));
        if (strcmp(tmp_dir, "/")) {
            chdir("..");
        }

        // printf("%s\n", tmp_dir);

    } while (strcmp(tmp_dir, "/"));

    chdir(cwd);

    if (exists) {
        printf("neogit repo already exists!\n");
    } else {
        mkdir(".neogit", 0777);
        create_configs();
    }
}

void add_to_staging(char *path)
{
    if (is_staged(path)) return;
    write_path_to_file(path);
    int flag = 0;
    if (!is_tracked(path)) {write_path_to_tracks(path); flag = 1;}
    if (is_dir(path)) {
        char cwd[MAX_PATH_LENGTH];
        getcwd(cwd, sizeof(cwd));
        chdir(path);
        iterate(write_path_to_file);
        if (flag) iterate(write_path_to_tracks);
        chdir(cwd);
    }
}

void remove_from_staging(char *path)
{
    remove_path_from_file(path);
    if (is_dir(path)) {
        char cwd[MAX_PATH_LENGTH];
        getcwd(cwd, sizeof(cwd));
        chdir(path);
        iterate(remove_path_from_file);
        chdir(cwd);
    }
}

void run_add(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return;
    }

    if (!strcmp(argv[2], "-n")) {
        run_add_n_recursive(atoi(argv[3]), atoi(argv[3]));
        return;
    }

    int constant;
    if (!strcmp(argv[2], "-f")) constant = 3;
    else constant = 2;

    for (int i = 0; i < argc - constant; i++) {
        char path[MAX_PATH_LENGTH];
        realpath(argv[i + constant], path);
        if (is_dir(path) || is_file(path)) {
            add_to_staging(path);
        } else {
            printf("\"%s\" doesn't exist!\n", argv[i + constant]);
        }
    }
}

void add_space(int depth, int first_depth)
{
    for (int i = 0; i < first_depth - depth; i++)
        printf("   ");
}

void run_add_n_recursive(int depth, int first_depth)
{
    if (depth == 0) return;

    struct dirent *entry;

    DIR *dir = opendir(".");

    char cwd[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        getcwd(cwd, sizeof(cwd));
        if (entry->d_type == DT_DIR) {
            add_space(depth, first_depth);
            printf(CYN);
            printf("%s\n", entry->d_name);
            char help[MAX_PATH_LENGTH]; strcpy(help, cwd);
            strcat(help, "/");
            strcat(help, entry->d_name);
            printf(RESET);
            chdir(entry->d_name);
            run_add_n_recursive(depth - 1, first_depth);
            chdir(cwd);
        } else {
            add_space(depth, first_depth);
            printf("%s ", entry->d_name);
            char help[MAX_PATH_LENGTH]; strcpy(help, cwd);
            strcat(help, "/");
            strcat(help, entry->d_name);
            if (is_staged(help)) printf("(staged)\n");
            else printf("(not staged)\n");
        }  
    }

    closedir(dir);

}

void run_reset(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return;
    }
    
    if (!strcmp(argv[2], "-undo")) {reset_undo(); return;}

    int constant;
    if (!strcmp(argv[2], "-f")) constant = 3;
    else constant = 2;

    for (int i = 0; i < argc - constant; i++) {
        char path[MAX_PATH_LENGTH];
        realpath(argv[i + constant], path);
        if (is_dir(path) || is_file(path)) {
            remove_from_staging(path);
        } else {
            printf("\"%s\" doesn't exist!\n", argv[i + constant]);
        }
    }
}

void reset_undo()
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));

    go_to_main_address();
    FILE *file = fopen(".neogit/staging", "r");
    char line[MAX_LINE_LENGTH];
    char prev[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file) != NULL) strcpy(prev, line);
    int length = strlen(prev);
    if (length > 0 && prev[length - 1] == '\n') prev[length - 1] = '\0';
    remove_from_staging(prev);
    if (is_dir(prev)) reset_undo();

    fclose(file);
    chdir(cwd);
}

user_info *who()
{
    user_info *user = malloc(sizeof(user_info));

    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/configs/username", "r");
    int c = fgetc(file);
    fclose(file);

    if (c == EOF) {
        chdir(cwd);
        file = fopen("/home/sana_niroomand/project/include/configs/username", "r");
        c = fgetc(file);
        if (c == EOF) {
            fclose(file);
            return NULL;
        } else {
            ungetc(c, file);
            fscanf(file, "%[^\n]s", user->username);
            fclose(file);
            file = fopen("/home/sana_niroomand/project/include/configs/email", "r");
            fscanf(file, "%[^\n]s", user->email);
            fclose(file);

            return user;
        }
    } else {
        file = fopen(".neogit/configs/username", "r");
        fscanf(file, "%[^\n]s", user->username);
        fclose(file);
        file = fopen(".neogit/configs/email", "r");
        fscanf(file, "%[^\n]s", user->email);
        fclose(file);
        chdir (cwd);

        return user;
    }
}

int commit_number()
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/commits");

    int result = 0;

    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
        result++;
    } 

    chdir(cwd);
    return result;
}

bool is_staged(char *path)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/staging", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (!strcmp(path, line)) {chdir(cwd); return true;}
    }

    chdir(cwd);

    return false;
}

bool any_staged()
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    FILE *file = fopen(".neogit/staging", "r");
    int c = fgetc(file);
    fclose(file);
    chdir(cwd);
    if (c == EOF) {
        return false;
    } else return true;
}

bool is_tracked(char *path)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/tracks", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (!strcmp(path, line)) {chdir(cwd); return true;}
    }

    chdir(cwd);
    return false;
}

bool branch_exists(char *branch)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/branches");
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, branch)) {chdir(cwd); return true;}
    }
    chdir(cwd);
    return false;
}

bool tag_exists(char *tag)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/tags");
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, tag)) {chdir(cwd); return true;}
    }
    chdir(cwd);
    return false; 
}

bool is_dir(char *path) 
{
    DIR *dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);
        return true;
    } else return false;
}

bool is_file(char *path)
{
    FILE *file = fopen(path, "r");
    if (file != NULL) {
        fclose(file);
        return true;
    } else return false;
}

void go_to_main_address()
{
    int flag = 0;
    while (true) {
        DIR *dir = opendir(".");
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".neogit")) {
                flag = 1;
                break;
            }
        }
        closedir(dir);
        if (flag) break;
        chdir("..");
    }
}

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
        if (!strcmp(entry->d_name, filename)) {chdir(cwd); closedir(dir); return true;}
    }
    chdir(cwd);
    closedir(dir);
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
        char current[MAX_NAME_LENGTH];
        current_branch(current);

        while ((entry = readdir(dir)) != NULL) {
            if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
            if (!strcmp(current, entry->d_name)) {
                printf(RED);
                printf("*");
                printf(RESET);
                printf("%s\n", entry->d_name);
            }
            else printf("%s\n", entry->d_name);
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
    if (argc < 4) {
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

    if (!strcmp(argv[2], "-m") && strlen(argv[3]) > 72) {
        printf("commit message too long!\n");
        return;
    }

    char message[MAX_MESSAGE_LENGTH];
    if (!strcmp(argv[2], "-s")) {
        if (!shortcut_exists(argv[3], message)) {
            printf("no shortcut exists with this name\n");
            return;
        }
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

    if (!strcmp(argv[2], "-m")) {
        fprintf(file, "message: %s\n", argv[3]);
    } else if (!strcmp(argv[2], "-s")) {
        fprintf(file, "message: %s\n", message);
    }
    
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
    int files = 0;

    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
        realpath(entry->d_name, path);
        if (is_staged(path) || (is_tracked(path) && !strcmp(number, "1"))) {
            strcpy(command, "");
            strcat(command, "cp ");
            if (is_dir(path)) {
                strcat(command, "-r ");
                number_of_files(path, &files);
            } else files++;
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
            if (is_dir(path)) {
                strcat(command, "-r ");
                number_of_files(path, &files);
            } else files++;
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

    fprintf(file, "files committed: %d\n", files);
    fclose(file);

    chdir(".neogit");
    file = fopen("staging", "w");
    fclose(file);
    chdir(cwd);
}

void calculate_hash(char *filename, char hash[])
{
    char cmd[MAX_COMMAND_LENGTH] = "md5sum ";
    
    strcat(hash, filename);
    
    char buf[MAX_HASH_CODE] = {0};
    FILE *fp;
    
    fp = popen(cmd, "r");

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        sscanf(buf, "%s", hash);
    }

    pclose(fp);
}

void calculate_hash_in_last_state(char *filename, char hash[]) 
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

bool is_deleted(char *path, char *filename)
{
    char help[MAX_PATH_LENGTH];
    go_to_main_address();
    DIR *dir = opendir(".neogit/last_state");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            realpath(entry->d_name, help);
            
            if (!is_in_working_directory(path, entry->d_name) && !strcmp(entry->d_name, filename)) {
                chdir(path);
                closedir(dir);
                return true;
            }
        }
    }
    chdir(path);
    closedir(dir);

    return false;
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

            if (!is_in_last_state(entry->d_name)) {
                printf("%s ", entry->d_name);
                if (is_staged(help)) printf("+");
                else printf("-");
                printf("A\n");
                continue;
            }
            
            if (is_deleted(path, entry->d_name)) {
                printf("%s ", entry->d_name);
                if (is_staged(help)) printf("+");
                else printf("-");
                printf("D\n");
                continue;
            }
            
            calculate_hash(entry->d_name, new_hash);
            puts(new_hash);
            calculate_hash_in_last_state(entry->d_name, old_hash);
            puts(old_hash);
            if (strcmp(new_hash, old_hash)) {
                printf("%s ", entry->d_name);
                if (is_staged(help)) printf("+");
                else printf("-");
                printf("M\n");
                continue;
            }
        }
    }
    closedir(dir);
}

void checkout_branch(char *branch) 
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/branches");
    char number[1000];
    FILE *file = fopen(branch, "r");
    fscanf(file, "%s", number);
    fclose(file);
    chdir("../configs");
    file = fopen("branch", "w");
    fprintf(file, "%s\n", branch);
    fclose(file);   
    chdir(cwd);

    checkout_commit(number);
}

void checkout_commit(char *number)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    char main[MAX_PATH_LENGTH];
    getcwd(main, sizeof(main));
    char command[MAX_COMMAND_LENGTH] = "rm -r ";
    char help[MAX_PATH_LENGTH];
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
        realpath(entry->d_name, help);
        if (!is_tracked(help)) continue;
        if (entry->d_type == DT_DIR) {
            strcpy(command, "rm -r ");
            strcat(command, entry->d_name);
            system(command);
        } else remove(entry->d_name);
    }
    closedir(dir);
    
    char directory[MAX_PATH_LENGTH] = ".neogit/commits/";
    strcat(directory, number);
    chdir(directory);
    dir = opendir(".");
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
        realpath(entry->d_name, help);
        strcpy(command, "cp ");
        if (entry->d_type == DT_DIR) strcat(command, "-r ");
        strcat(command, help);
        strcat(command, " ");
        strcat(command, main);
        system(command);
    }
    closedir(dir);
    chdir(cwd);
}

void run_checkout(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return;
    }

    if (any_staged()) {
        printf("please commit the changes first\n");
        return;
    }

    if (!strcmp(argv[2], "HEAD")) {
        char number[1000];
        sprintf(number, "%d", commit_number());
        checkout_commit(number);
    }
    else if (strstr(argv[2], "HEAD-")) {
        int jump = commit_number() - (argv[2][5] - '0');
        char number[1000];
        sprintf(number, "%d", jump);
        checkout_commit(number);
    }
    else if (branch_exists(argv[2])) {
        checkout_branch(argv[2]);
    } 
    else {
        checkout_commit(argv[2]);
    }
}

void run_revert(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return;
    }

    bool flag_e = false;

    char command[MAX_COMMAND_LENGTH] = "neogit checkout ";
    strcat(command, argv[argc - 1]);
    system(command);
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/commits");
    strcpy(command, "cp -r ");
    strcat(command, argv[argc - 1]);
    strcat(command, " ");
    char dest[1000];
    sprintf(dest, "%d", commit_number() + 1);
    strcat(command, dest);
    system(command);
    chdir(dest);
    FILE *file = fopen(".info", "r");
    FILE *out = fopen(".out", "w");
    char line[MAX_LINE_LENGTH];
    while ((fgets(line, sizeof(line), file)) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        if (strstr(line, "id: ")) fprintf(out, "id: %d\n", commit_number());
        else if (strstr(line, "time: ")) {
            time_t t = time(NULL);
            struct tm *tm = localtime(&t);
            fprintf(out, "time: %s", asctime(tm));
        }
        else if (strstr(line, "message: ") && flag_e) {
            char new_message[MAX_MESSAGE_LENGTH];
            puts("enter the new message please:");
            fgets(new_message, sizeof(new_message), stdin);
            length = strlen(new_message);
            if (length > 0 && new_message[length - 1] == '\n') new_message[length - 1] = '\0';
            fprintf(out, "message: %s\n", new_message);
        }
        else fprintf(out, "%s\n", line);
    }
    fclose(file);
    fclose(out);
    remove(".info");
    rename(".out", ".info");
    chdir(cwd);
}

void run_tag(int argc, char * const argv[])
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/tags");
    
    if (argc == 2) {
        struct dirent **namelist;
        int n = scandir(".", &namelist, 0, alphasort);
        for (int i = 0; i < n; i++) {
            if ((namelist[i]->d_name)[0] == '.' || !strcmp(namelist[i]->d_name, "sana_niroomand")) {
                free(namelist[i]);
                continue;
            }
            printf("%s\n", namelist[i]->d_name);
            free(namelist[i]);
        }
        free(namelist);
    }
    else if (!strcmp(argv[2], "show")) {
        FILE *file = fopen(argv[3], "r");
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
        fclose(file);
    }
    else if (!strcmp(argv[2], "-a")) {

        user_info *user = who();
        if (user == NULL) {
            printf("please add your username and email before committing!\n");
            return;
        }

        if (strcmp(argv[argc - 1], "-f")) {
            if (tag_exists(argv[3])) {printf("this tag already exists!\n"); return;}
        } else {
            if (tag_exists(argv[3])) remove(argv[3]);
        }

        FILE *file = fopen(argv[3], "w");
        fprintf(file, "name: %s\n", argv[3]);
        if (argc > 5 && !strcmp(argv[4], "-c")) fprintf(file, "commit id: %s\n", argv[5]);
        else if (argc > 7 && !strcmp(argv[6], "-c")) fprintf(file, "commit id: %s\n", argv[7]);
        else fprintf(file, "commit id: %d\n", commit_number());
        fprintf(file, "author: %s %s\n", user->username, user->email);
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        fprintf(file, "time: %s", asctime(tm));
        if (argc > 5 && !strcmp(argv[4], "-m")) fprintf(file, "message: %s\n", argv[5]);
        else fprintf(file, "message: \n");
        fclose(file);
    }

    chdir(cwd);
}

void trail(char line[])
{
    int index = strlen(line) - 1;
    if (isspace(line[index])) line[index--] = '\0';
}

bool diff_files(struct dirent *entry1, struct dirent *entry2, char * const argv[])
{
    char filepath[MAX_PATH_LENGTH];
    realpath(argv[3], filepath);
    strcat(filepath, "/");
    strcat(filepath, entry1->d_name);
    FILE *file1 = fopen(filepath, "r");
    realpath(argv[4], filepath);
    strcat(filepath, "/");
    strcat(filepath, entry2->d_name);
    FILE *file2 = fopen(filepath, "r");

    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    int n1 = 0, n2 = 0;
    int flag = 0;

    while ((fgets(line1, sizeof(line1), file1) != NULL) && (fgets(line2, sizeof(line2), file2) != NULL)) {
        n1++; n2++;
        trail(line1); trail(line2);
        while (strlen(line1) == 0) {
            if (fgets(line1, sizeof(line1), file1) == NULL) break;
            trail(line1);
            n1++;
        }
        while (strlen(line2) == 0) {
            if (fgets(line2, sizeof(line2), file2) == NULL) break;
            trail(line2);
            n2++;
        }
        trail(line1); trail(line2);

        if (strcmp(line1, line2)) {
            if (!flag) printf("«««««\n");
            printf("%s-%d\n", argv[3], n1);
            printf(MAG);
            printf("%s\n", line1);
            printf(RESET);
            printf("%s-%d\n", argv[4], n2);
            printf(CYN);
            printf("%s\n", line2);
            printf(RESET);
            flag = 1;
        }
    }
    fclose(file1); fclose(file2);
    if (flag) {
        printf("»»»»»\n");
        return true;
    }
    else return false;
}

bool diff_files_merge(struct dirent *entry1, struct dirent *entry2, char commit1[], char commit2[])
{
    char filepath[MAX_PATH_LENGTH];
    realpath(commit1, filepath);
    strcat(filepath, "/");
    strcat(filepath, entry1->d_name);
    FILE *file1 = fopen(filepath, "r");
    realpath(commit2, filepath);
    strcat(filepath, "/");
    strcat(filepath, entry2->d_name);
    FILE *file2 = fopen(filepath, "r");

    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    int n1 = 0, n2 = 0;
    int flag = 0;

    while ((fgets(line1, sizeof(line1), file1) != NULL) && (fgets(line2, sizeof(line2), file2) != NULL)) {
        n1++; n2++;
        trail(line1); trail(line2);
        while (strlen(line1) == 0) {
            if (fgets(line1, sizeof(line1), file1) == NULL) break;
            trail(line1);
            n1++;
        }
        while (strlen(line2) == 0) {
            if (fgets(line2, sizeof(line2), file2) == NULL) break;
            trail(line2);
            n2++;
        }
        trail(line1); trail(line2);

        if (strcmp(line1, line2)) {
            if (!flag) printf("«««««\n");
            realpath(commit1, filepath);
            strcat(filepath, "/");
            strcat(filepath, entry1->d_name);
            puts(filepath);
            printf("%s-%d\n", entry1->d_name, n1);
            printf(MAG);
            printf("%s\n", line1);
            printf(RESET);
            realpath(commit2, filepath);
            strcat(filepath, "/");
            strcat(filepath, entry2->d_name);
            puts(filepath);
            printf("%s-%d\n", entry2->d_name, n2);
            printf(CYN);
            printf("%s\n", line2);
            printf(RESET);
            flag = 1;
        }
    }
    fclose(file1); fclose(file2);
    if (flag) {
        printf("»»»»»\n");
        return true;
    }
    else return false;
}

void run_diff(int argc, char * const argv[])
{
    if (argc < 5) {
        printf("invalid command\n");
        return;
    }

    if (!strcmp(argv[2], "-f")) {
        char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
        FILE *file1 = fopen(argv[3], "r");
        FILE *file2 = fopen(argv[4], "r");
        int n1 = 0, n2 = 0;
        int start1 = 0, start2 = 0;
        int end1 = 0, end2 = 0;

        if (argc == 7) {
            if (!strcmp(argv[5], "-line1")) {
                sscanf(argv[6], "%d-%d", &start1, &end1);
                while (n1 < start1 - 1) {
                    fgets(line1, sizeof(line1), file1);
                    n1++;
                }
            } else if (!strcmp(argv[5], "-line2")) {
                sscanf(argv[6], "%d-%d", &start2, &end2);
                while (n2 < start2 - 1) {
                    fgets(line2, sizeof(line2), file2);
                    n2++;
                }
            }
        } else if (argc == 9) {
            sscanf(argv[6], "%d-%d", &start1, &end1);
            sscanf(argv[8], "%d-%d", &start2, &end2);
            while (n1 < start1 - 1) {
                fgets(line1, sizeof(line1), file1);
                n1++;
            }
            while (n2 < start2 - 1) {
                fgets(line2, sizeof(line2), file2);
                n2++;
            }
        }

        while ((fgets(line1, sizeof(line1), file1) != NULL) && (fgets(line2, sizeof(line2), file2) != NULL)) {
            n1++; n2++;
            trail(line1); trail(line2);
            while (strlen(line1) == 0) {
                if (fgets(line1, sizeof(line1), file1) == NULL) break;
                trail(line1);
                n1++;
            }
            while (strlen(line2) == 0) {
                if (fgets(line2, sizeof(line2), file2) == NULL) break;
                trail(line2);
                n2++;
            }
            trail(line1); trail(line2);

            if (strcmp(line1, line2)) {
                printf("«««««\n");
                printf("%s-%d\n", argv[3], n1);
                printf(MAG);
                printf("%s\n", line1);
                printf(RESET);
                printf("%s-%d\n", argv[4], n2);
                printf(CYN);
                printf("%s\n", line2);
                printf(RESET);
                printf("»»»»»\n");
            }

            if (n1 == end1 || n2 == end2) break;
        }
        fclose(file1); fclose(file2);
    }

    else if (!strcmp(argv[2], "-c")) {
        char cwd[MAX_PATH_LENGTH];
        getcwd(cwd, sizeof(cwd));
        go_to_main_address();
        chdir(".neogit/commits");
        struct dirent *entry1;
        struct dirent *entry2;
        int flag;
        DIR *dir1 = opendir(argv[3]);
        while ((entry1 = readdir(dir1)) != NULL) {
            if ((entry1->d_name)[0] == '.' || !strcmp(entry1->d_name, "sana_niroomand") || entry1->d_type != DT_REG) continue;
            flag = 0;
            DIR *dir2 = opendir(argv[4]);
            while ((entry2 = readdir(dir2)) != NULL) {
                if (!strcmp(entry1->d_name, entry2->d_name) && entry2->d_type == DT_REG) {flag = 1; break;}
            }
            closedir(dir2);
            if (!flag) {
                printf(MAG);
                puts(entry1->d_name);
                printf(RESET);
            } else diff_files(entry1, entry2, argv);
        }
        closedir(dir1);

        DIR *dir2 = opendir(argv[4]);
        while ((entry2 = readdir(dir2)) != NULL) {
            if ((entry2->d_name)[0] == '.' || !strcmp(entry2->d_name, "sana_niroomand") || entry2->d_type != DT_REG) continue;
            flag = 0;
            dir1 = opendir(argv[3]);
            while ((entry1 = readdir(dir1)) != NULL) {
                if (!strcmp(entry1->d_name, entry2->d_name) && entry1->d_type == DT_REG) {flag = 1; break;}
            }
            closedir(dir1);
            if (!flag) {
                printf(CYN);
                puts(entry2->d_name);
                printf(RESET);
            }
        }
        closedir(dir2);
        chdir(cwd);
    }
}

void branch_head(char *branch, char commit[])
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/branches");
    FILE *file = fopen(branch, "r");
    fscanf(file, "%s", commit);
    fclose(file);
    chdir(cwd);
}

void run_merge(int argc, char * const argv[])
{
    if (argc < 5) {
        printf("invalid command\n");
        return;
    }

    char commit1[1000], commit2[1000];
    branch_head(argv[3], commit1);
    branch_head(argv[4], commit2);
    
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    chdir(".neogit/commits");
    char number[1000];
    sprintf(number, "%d", commit_number() + 1);
    mkdir(number, 0777);
    struct dirent *entry1;
    struct dirent *entry2;
    int flag;
    char command[MAX_COMMAND_LENGTH];
    char path[MAX_PATH_LENGTH];
    DIR *dir1 = opendir(commit1);
    while ((entry1 = readdir(dir1)) != NULL) {
        if ((entry1->d_name)[0] == '.' || !strcmp(entry1->d_name, "sana_niroomand") || entry1->d_type != DT_REG) continue;
        flag = 0;
        DIR *dir2 = opendir(commit2);
        while ((entry2 = readdir(dir2)) != NULL) {
            if (!strcmp(entry1->d_name, entry2->d_name) && entry2->d_type == DT_REG) {flag = 1; break;}
        }
        closedir(dir2);
        if (!flag) {
            chdir(commit1);
            realpath(entry1->d_name, path);
            chdir("..");
            strcpy(command, "cp ");
            if (entry1->d_type == DT_DIR) strcat(command, "-r ");
            strcat(command, path);
            strcat(command, " ");
            strcat(command, number);
            system(command);
        } else {
            if (diff_files_merge(entry1, entry2, commit1, commit2)) {
                printf("sorry merge cannot happen. we reached a conflict!");
                strcpy(command, "rm -r ");
                strcat(command, number);
                system(command);
                closedir(dir1);
                chdir(cwd);
                return;
            } else {
                chdir(commit1);
                realpath(entry1->d_name, path);
                chdir("..");
                strcpy(command, "cp ");
                if (entry1->d_type == DT_DIR) strcat(command, "-r ");
                strcat(command, path);
                strcat(command, " ");
                strcat(command, number);
                system(command);
            }
        }
    }
    closedir(dir1);

    DIR *dir2 = opendir(commit2);
    while ((entry2 = readdir(dir2)) != NULL) {
        if ((entry2->d_name)[0] == '.' || !strcmp(entry2->d_name, "sana_niroomand") || entry2->d_type != DT_REG) continue;
        flag = 0;
        dir1 = opendir(commit1);
        while ((entry1 = readdir(dir1)) != NULL) {
            if (!strcmp(entry1->d_name, entry2->d_name) && entry1->d_type == DT_REG) {flag = 1; break;}
        }
        closedir(dir1);
        if (!flag) {
            chdir(commit2);
            realpath(entry2->d_name, path);
            chdir("..");
            strcpy(command, "cp ");
            if (entry2->d_type == DT_DIR) strcat(command, "-r ");
            strcat(command, path);
            strcat(command, " ");
            strcat(command, number);
            system(command);
        }
    }

    add_commit_to_branch(commit_number(), argv[3]);
    chdir(number);
    FILE *file = fopen(".info", "w");

    fprintf(file, "id: %s\n", number);

    fprintf(file, "message: merged branch %s and %s\n", argv[3], argv[4]);

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    fprintf(file, "time: %s", asctime(tm));
    
    fprintf(file, "branch: %s\n", argv[3]);
    
    user_info *user = who();
    fprintf(file, "username: %s\n", user->username);
    fprintf(file, "email: %s\n", user->email);
    int files = 0;
    number_of_files(number, &files);
    fprintf(file, "files committed: %d\n", files);
    fclose(file);
    closedir(dir2);
    chdir(cwd);
}

void number_of_files(char *path, int *files)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    char help[MAX_PATH_LENGTH];
    chdir(path);
    DIR *dir = opendir(".");
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        if (entry->d_type == DT_DIR) {
            memset(help, 0, sizeof(help));
            realpath(entry->d_name, help);
            number_of_files(help, files);
        } else {
            memset(help, 0, sizeof(help));
            realpath(entry->d_name, help);
            (*files)++;
        }
    }

    closedir(dir);
    chdir(cwd);
}

void current_branch(char current[])
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    FILE *file = fopen(".neogit/configs/branch", "r");
    fgets(current, 4000, file);
    int length = strlen(current);
    if (length > 0 && current[length - 1] == '\n') current[length - 1] = '\0';
    fclose(file);
    chdir(cwd);
}

bool neogit_exists()
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    
    bool exists = false;
    char tmp_dir[MAX_FILENAME_LENGTH];
    struct dirent *entry;
    do {
        DIR *dir = opendir(".");

        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_DIR && !strcmp(entry->d_name, ".neogit")) {
                closedir(dir);
                chdir(cwd);
                return true;
            }
        }
        closedir(dir);
        
        getcwd(tmp_dir, sizeof(tmp_dir));
        if (strcmp(tmp_dir, "/")) {
            chdir("..");
        }

    } while (strcmp(tmp_dir, "/"));

    chdir(cwd);

    return false;
}

bool is_alias(int argc, char * const argv[])
{
    char cwd[MAX_FILENAME_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    char line[MAX_LINE_LENGTH];
    char command[MAX_COMMAND_LENGTH] = "";
    for (int i = 0; i < argc; i++) {
        strcat(command, argv[i]);
        if (i < argc - 1) strcat(command, " ");
    }
    int n;

    FILE *file = fopen(".neogit/configs/alias", "r");
    int c = fgetc(file);
    fclose(file);

    if (c == EOF) {
        chdir(cwd);
        file = fopen("/home/sana_niroomand/project/include/configs/alias", "r");
        c = fgetc(file);
        if (c == EOF) {
            fclose(file);
            return false;
        } else {
            ungetc(c, file);
            n = 0;
            while (fgets(line, sizeof(line), file) != NULL) {
                int length = strlen(line);
                if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
                n++;
                if (n % 2 == 0) continue;
                if (!strcmp(command, line)) {
                    fgets(line, sizeof(line), file);
                    length = strlen(line);
                    if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
                    system(line); 
                    return true;
                }
            }
            fclose(file);
        }
    } else {
        file = fopen(".neogit/configs/alias", "r");
        n = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            int length = strlen(line);
            if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
            n++;
            if (n % 2 == 0) continue;
            if (!strcmp(command, line)) {
                chdir(cwd); 
                fgets(line, sizeof(line), file);
                length = strlen(line);
                if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
                system(line); 
                return true;
            }
        }
        fclose(file);
        chdir(cwd);
    }

    return false;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please enter a valid command\n");
        return 1;
    }

    if (!strcmp(argv[1], "init")) run_init(argc, argv);
    else if (neogit_exists()) {
        if (!strcmp(argv[1], "config")) {
            if (argc < 4) {
                printf("please enter a valid command\n");
                return 1;
            }
            if (!strcmp(argv[2], "user.name") || !strcmp(argv[3], "user.name")) run_config_username(argc, argv);
            else if (!strcmp(argv[2], "user.email") || !strcmp(argv[3], "user.email")) run_config_email(argc, argv);
            else if (strstr(argv[2], "alias.") || strstr(argv[3], "alias.")) run_config_alias(argc, argv);
        }
        else if (!strcmp(argv[1], "add")) run_add(argc, argv);
        else if (!strcmp(argv[1], "reset")) run_reset(argc, argv);
        else if (!strcmp(argv[1], "commit")) run_commit(argc, argv);
        else if (!strcmp(argv[1], "log")) run_log(argc, argv);
        else if (!strcmp(argv[1], "branch")) run_branch(argc, argv);
        else if (!strcmp(argv[1], "status")) run_status(argc, argv);
        else if (!strcmp(argv[1], "checkout")) run_checkout(argc, argv);
        else if (!strcmp(argv[1], "revert")) run_revert(argc, argv);
        else if (!strcmp(argv[1], "tag")) run_tag(argc, argv);
        else if (!strcmp(argv[1], "diff")) run_diff(argc, argv);
        else if (!strcmp(argv[1], "merge")) run_merge(argc, argv);
        else if (!strcmp(argv[1], "set")) run_set(argc, argv);
        else if (!strcmp(argv[1], "replace")) run_replace(argc, argv);
        else if (!strcmp(argv[1], "remove")) run_remove(argc, argv);
        else if (!is_alias(argc, argv)) printf("please enter a valid command\n");
    }
    else {
        printf("neogit hasn't been initialized\n");
    }

    return 0;
}