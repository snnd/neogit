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
