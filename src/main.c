#include <header.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please enter a valid command\n");
        return 1;
    }

    if (!strcmp(argv[1], "init")) run_init(argc, argv);
    else if (!strcmp(argv[1], "add") && !strcmp(argv[2], "-f")) run_add_f(argc, argv);
    else if (!strcmp(argv[1], "add") && !strcmp(argv[2], "-n")) run_add_n(argc, argv);
    else if (!strcmp(argv[1], "add")) run_add(argc, argv);
    // else if (!strcmp(argv[1], "commit")) run_commit(argc, argv);
    else if (!strcmp(argv[1], "config")) {
        if (argc < 4) {
            printf("please enter a valid command\n");
            return 1;
        }
        if (!strcmp(argv[2], "user.name") || !strcmp(argv[3], "user.name")) run_config_username(argc, argv);
        else if (!strcmp(argv[2], "user.email") || !strcmp(argv[3], "user.email")) run_config_email(argc, argv);
    }

    return 0;
}