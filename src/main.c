#include <header.h>

int create_configs(char *username, char *email) 
{
    FILE *file = fopen(".neogit/config", "w");
    if (file == NULL) return 1;

    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);
    fprintf(file, "branch: %s\n", "main");

    fclose(file);

    file = fopen(".neogit/tracks", "w");
    fclose(file);

    file = fopen(".neogit/staging", "w");
    fclose(file);

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

        printf("%s\n", tmp_dir);

    } while (strcmp(tmp_dir, "/"));

    if (chdir(cwd) != 0) return 1;

    if (exists) {
        perror("neogit repo already exists!");
    } else {
        if (mkdir(".neogit", 0755) != 0) return 1;
        create_configs("soha", "soha.niroomand@gmail.com");
    }
    return 0;
}

int add_to_staging(char *filepath) 
{
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL) return 1;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int length = sizeof(line);
        if (line > 0 && line[length - 1] == '\n') line[length - 1] = '\0';

        if (!strcmp(line, filepath)) return 0;
    }
    fclose(file);

    file = fopen(".neogit/staging", "a");
    if (file == NULL) return 1;

    fprintf(file, "%s\n", filepath);
    fclose(file);

    return 0;
}

int run_add(int argc, char * const argv[])
{
    if (argc < 3) {
        perror("invalid command");
        return 1;
    }

    char *filepath = argv[2];
    if (add_to_staging(filepath) != 0) return 1;

    return 0;
}

void print_command(int argc, char * const argv[]) 
{
    for (int i = 0; i < argc; i++) {
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}

//#define _DEB

#ifdef _DEB
int main() 
{
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
    else if (!strcmp(argv[1], "add")) {
        run_add(argc, argv);
    }

    return 0;
}