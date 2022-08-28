#include "headers.h"

pid_t curr_id;

void print_prompt()
{
    char curr_dir[MAX_SIZE], mydir[MAX_SIZE];
    char toprint[MAX_SIZE];
    getcwd(curr_dir, sizeof(curr_dir));

    int check1 = getlogin_r(username, sizeof(username));
    // printf("%d\n%s\n", check1, username);
    if (check1 != 0)
    {
        perror("some error with finding username");
        return;
    }

    int check2 = gethostname(system_name, sizeof(system_name));
    // printf("%d\n%s\n", check2, system_name);
    if (check2 != 0)
    {
        perror("some error with finding username");
        return;
    }

    // home/hitesh/Documents/IIIT-H/OSN/assignments/A2/Linux-C-Shell/abc/def
    char delim[2] = "/";
    if (!strcmp(curr_dir, pseudo_home))
    {
        strcat(mydir, "~");
    }
    else{
        // char test[] = "home/hitesh/Documents/IIIT-H/OSN/assignments/A2/Linux-C-Shell/abc/def";
        char *token;
        char token1[MAX_SIZE];
        token = strtok(pseudo_home, delim);
        while (token != NULL)
        {
            strcpy(token1, token);
            token = strtok(NULL, delim);
        }
        token = strtok(curr_dir, delim);
        while (strcmp(token, token1))
        {
            token = strtok(NULL, delim);
        }
        token = strtok(NULL, "");
        strcat(mydir, "~/");
        strcat(mydir, token);
    }
    

    strcat(toprint, "<");
    strcat(toprint, username);
    strcat(toprint, "@");
    strcat(toprint, system_name);
    strcat(toprint, ":");

    printf("\033[1;32m");
    printf("\n%s", toprint);
    printf("\033[0m");
    strcpy(toprint, "");
    strcat(toprint, mydir);
    strcat(toprint, ">");

    printf("\033[1;34m");

	printf("%s ", toprint);
	printf("\033[0m");
}

void main_loop(void)
{
    int status = 1;
    char *command;
    ssize_t command_size = 0;

    while (status)
    {
        print_prompt();

        getline(&command, &command_size, stdin);
        
        execcommand(command);

        break;
    }
}

int main(int argc, ptr argv[])
{
    curr_id = getpid();
    // printf("%d\n", curr_id);

    // char curr_dir[MAX_SIZE];
    getcwd(pseudo_home, sizeof(pseudo_home));
    // printf("%s\n", curr_dir);

    main_loop();

    return 0;
}