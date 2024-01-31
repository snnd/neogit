#include <header.h>

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
        }
        else if (!strcmp(argv[1], "add")) run_add(argc, argv);
        else if (!strcmp(argv[1], "reset")) run_reset(argc, argv);
        else if (!strcmp(argv[1], "commit")) run_commit(argc, argv);
    }
    else {
        printf("neogit hasn't been initialized\n");
    }

    return 0;
}