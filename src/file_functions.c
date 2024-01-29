#include "header.h"

int create_configs() 
{
    FILE *file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/staging", "w");
    fclose(file);

    if (mkdir(".neogit/configs", 0755) != 0) return 1;

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

    return 0;
}

int run_config_username(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return 1;
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
        return 1;
    }

    if (chdir(cwd) != 0) return 1;

    return 0;
}

int run_config_email(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return 1;
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
        return 1;
    }

    if (chdir(cwd) != 0) return 1;

    return 0;
}

int run_init(int argc, char * const argv[]) 
{
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
        closedir(dir);
        
        if (getcwd(tmp_dir, sizeof(tmp_dir)) == NULL) return 1;
        if (strcmp(tmp_dir, "/")) {
            if (chdir("..") != 0) return 1;
        }

        // printf("%s\n", tmp_dir);

    } while (strcmp(tmp_dir, "/"));

    if (chdir(cwd) != 0) return 1;

    if (exists) {
        printf("neogit repo already exists!\n");
    } else {
        if (mkdir(".neogit", 0755) != 0) return 1;
        create_configs();
    }
    return 0;
}

int add_to_staging(char *path) 
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();
    
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL) return 1;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n') line[length - 1] = '\0';
        if (!strcmp(path, line)) return 0;
    }
    fclose(file);
    
    file = fopen(".neogit/staging", "a");
    if (file == NULL) return 1;

    if (is_dir(path)) {
        fprintf(file, "%s\n", path);
        DIR *dir = opendir(path);
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            puts(entry->d_name);
            if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;
            
            char *help = realpath(entry->d_name, NULL);
            add_to_staging(help);
        }
        closedir(dir);
    } else {
        fprintf(file, "%s\n", path);
    }

    fclose(file);

    if (chdir(cwd) != 0) return 1;

    return 0;
}

int is_staged(char *path)
{
    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    FILE *file = fopen(".neogit/staging", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (line > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (strstr(path, line)) {chdir(cwd); return 1;}
    }

    chdir(cwd);

    return 0;
}

void add_space(int depth, int first_depth)
{
    for (int i = 0; i < first_depth - depth; i++)
        printf("   ");
}

int run_add_n_recursive(int depth, int first_depth)
{
    if (depth == 0) return 0;

    struct dirent *entry;

    DIR *dir = opendir(".");

    if (dir == NULL) return 1;
    char cwd[MAX_PATH_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
        if (entry->d_type == DT_DIR) {
            add_space(depth, first_depth);
            printf(MAG);
            printf("%s ", entry->d_name);
            char help[MAX_PATH_LENGTH]; strcpy(help, cwd);
            strcat(help, "/");
            strcat(help, entry->d_name);
            if (is_staged(help)) printf("(staged)\n");
            else printf("(not staged)\n");
            printf(RESET);
            if (chdir(entry->d_name) != 0) return 1;
            run_add_n_recursive(depth - 1, first_depth);
            if (chdir(cwd) != 0) return 1;
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

    return 0;
}

int run_add_n(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return 1;
    }
    if (run_add_n_recursive(atoi(argv[3]), atoi(argv[3])) != 0) return 1;

    return 0;
}

int run_add_f(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return 1;
    }

    for (int i = 0; i < argc - 3; i++) {
        char* path = realpath(argv[3 + i], NULL);

        if(path == NULL){
            printf("path \"%s\" does not exist!\n", path);
        } else {
            if (add_to_staging(path) != 0) return 1;
        }
    }

    return 0;
}

int run_add(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return 1;
    }

    char* path = realpath(argv[2], NULL);
    if(path == NULL){
        printf("this path doesn't exist!\n");
        return 1;
    } else {
        if (add_to_staging(path) != 0) return 1;
    }

    return 0;
}

int run_reset(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return 1;
    }

    char cwd[MAX_PATH_LENGTH];
    getcwd(cwd, sizeof(cwd));
    go_to_main_address();

    char *path = realpath(argv[2], NULL);
    FILE *file = fopen(".neogit/staging", "r");
    FILE *result = fopen(".neogit/result", "w");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
            line[length - 1] = '\0';

        if (strcmp(line, path))
            fprintf(result, "%s\n", line);
    }
    fclose(file); fclose(result);
    if (remove(".neogit/staging") != 0) return 1;
    if (rename(".neogit/result", ".neogit/staging") != 0) return 1;

    if (chdir(cwd) != 0) return 1;
    
    return 0;
}

int is_dir(char *path) 
{
    DIR *dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);
        return 1;
    } else return 0;
}

int go_to_main_address()
{
    while (true) {
        DIR *dir = opendir(".");
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, ".neogit")) return 0;
        }
        if (chdir("..") != 0) return 1;
    }

    return 0;
}

void print_command(int argc, char * const argv[]) 
{
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}