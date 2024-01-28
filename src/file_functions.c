#include <header.h>

int create_configs() 
{
    FILE *file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/staging", "w");
    fclose(file);

    if (mkdir(".neogit/configs", 0755) != 0) return 1;

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

    return 0;
}

int run_config_email(int argc, char * const argv[])
{
    if (argc < 4) {
        printf("invalid command\n");
        return 1;
    }
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
        } else closedir(dir);

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
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL) return 1;

    

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (line > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (!strcmp(line, path)) return 0;
    }
    fclose(file);

    file = fopen(".neogit/staging", "a");
    if (file == NULL) return 1;

    fprintf(file, "%s\n", path);
    fclose(file);

    return 0;
}

int is_staged(char *path)
{
    FILE *file = fopen(".neogit/staging", "r");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (line > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (!strcmp(line, path)) return 1;
    }

    return 0;
}

void add_space(int depth, int first_depth)
{
    for (int i = 0; i < first_depth - depth; i++)
        printf("  ");
}

int run_add_n_recursive(int depth, int first_depth)
{
    if (depth == 0) return 0;

    struct dirent *entry;

    DIR *dir = opendir(".");

    if (dir == NULL) return 1;
    char cwd[MAX_FILENAME_LENGTH];

    while ((entry = readdir(dir)) != NULL) {
        if ((entry->d_name)[0] == '.' || !strcmp(entry->d_name, "sana_niroomand")) continue;

        if (entry->d_type == DT_DIR) {
            if (getcwd(cwd, sizeof(cwd)) == NULL) return 1;
            add_space(depth, first_depth);
            printf(BLU);
            printf("%s ", entry->d_name);
            if (is_staged(cwd)) {
                if (strstr())
            }
            printf(RESET);
            if (chdir(entry->d_name) != 0) return 1;
            run_add_n_recursive(depth - 1, first_depth);
            if (chdir(cwd) != 0) return 1;
        } else {
            add_space(depth, first_depth);
            printf("%s\n", entry->d_name);
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
        char *path = argv[3 + i];
        
        if (path_exists(path)) {
            if (add_to_staging(path) != 0) return 1;
        } else
            printf("path \"%s\" does not exist!\n", path);
    }

    return 0;
}

int run_add(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return 1;
    }

    char *path = argv[2];
    
    if (path_exists(path)) {
        if (add_to_staging(path) != 0) return 1;
    } else {
        printf("such a path does not exist!\n");
        return 1;
    }

    return 0;
}

int run_reset(int argc, char * const argv[])
{
    if (argc < 3) {
        printf("invalid command\n");
        return 1;
    }

    FILE *file = fopen(".neogit/staging", "r");
    FILE *result = fopen(".neogit/result", "w");
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
            line[length - 1] = '\0';

        if (strcmp(line, argv[2])) {
            fputs(line, result);
        }
    }
    fclose(file); fclose(result);
    if (remove(".neogit/staging") != 0) return 1;
    if (rename(".neogit/result", ".neogit/staging") != 0) return 1;
    
    return 0;
}

int is_regular_file(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int path_exists(char *path)
{
    if (is_regular_file) {
        if (fopen(path, "r") == NULL) return 0;
        else return 1;
    }
    else {
        if (opendir(path) == NULL) return 0;
        else return 1;
    }
}

void print_command(int argc, char * const argv[]) 
{
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}