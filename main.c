#include "headers.h"

pid_t curr_id;

void yellow()
{
    printf("\x1b[1;33m");
}
void blue()
{
    printf("\x1b[1;34m");
}
void reset()
{
    printf("\x1b[0m");
}

void print_prompt()
{
    // printf("inside print_prompt\n");
    char curr_dir[MAX_SIZE], mydir[MAX_SIZE];
    char toprint[MAX_SIZE];
    getcwd(curr_dir, sizeof(curr_dir));
    // printf("cwd: %s\n", curr_dir);

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

    char delim[2] = "/";

    if (!strcmp(curr_dir, pseudo_home))
    {
        strcpy(mydir, "~");
    }
    else if (strlen(curr_dir) < strlen(pseudo_home))
    {
        strcpy(mydir, curr_dir);
    }
    else
    {
        // char test[] = "home/hitesh/Documents/IIIT-H/OSN/assignments/A2/Linux-C-Shell/abc/def";
        char *token;
        char token1[MAX_SIZE];
        // printf("pseudo home: %s\n", pseudo_home);
        char copy[MAX_SIZE];
        strcpy(copy, pseudo_home);
        token = strtok(copy, delim);
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
        strcpy(mydir, "~/");
        strcat(mydir, token);
    }

    strcpy(toprint, "<");
    strcat(toprint, username);
    strcat(toprint, "@");
    strcat(toprint, system_name);
    strcat(toprint, ":");

    printf("\033[1;32m");
    printf("\n%s", toprint);
    printf("\033[0m");
    strcpy(toprint, "");
    strcat(toprint, mydir);
    strcat(toprint, foreground_text);
    strcat(toprint, ">");

    printf("\033[1;34m");

    printf("%s ", toprint);
    printf("\033[0m");
}

void del_process(int id)
{
    // printf("Entered killproc with job count = %lld and proc number=%d\n", job_count, id);
    int flag = 0;
    if (id == -1)
        job_count = 0;
    else
        for (ll i = 1; i <= job_count; i++)
        {
            if (job_arr[i].pid == id)
            {
                // printf("job killed\n");
                flag = 1;
                for (ll j = i; j < job_count; j++)
                    job_arr[j] = job_arr[j + 1];
                job_count--;
            }
        }
    if (flag == 0 && id != -1)
        printf("Error: no such process found\n");

    return;
}

void done()
{
    pid_t p;
    int status;
    p = waitpid(-1, &status, WNOHANG);
    for (ll z = 1; z <= job_count; z++)
    {
        if (p < 0)
        {
            perror("\nwaitpid failed\n");
        }
        const int exit = WEXITSTATUS(status);

        // printf("WIFEXITED (STATUS) OF KJOB = %d\nWEXITSTATUS %d  p= %d  job_arr[z] %d\n", WIFEXITED(status), WEXITSTATUS(status),p,job_arr[z].pid);

        if (((WIFEXITED(status) && p == job_arr[z].pid)))
        {
            if (exit == 0)
                fprintf(stderr, "\nExitted normally with exit status: %d\n", exit);
            else
                fprintf(stderr, "\nExitted abnormally\n");

            fprintf(stderr, "%s with pid %d: exited\n", job_arr[z].name, p);
            print_prompt();
            fflush(stdout);
            del_process(p);
        }
    }
}


void main_loop(void)
{
    int status = 1;
    char *command;
    ssize_t command_size = 0;

    foreground_text[0] = '\0';

    while (status)
    {
        curr_foreground_job.pid = -1;

        signal(SIGCHLD, done);

        print_prompt();

        getline(&command, &command_size, stdin);

        execcommand(command);

        // break;
    }
}

int main(int argc, ptr argv[])
{
    curr_id = getpid();
    // printf("%d\n", curr_id);

    getcwd(cwd, sizeof(cwd));
    // char curr_dir[MAX_SIZE];
    getcwd(pseudo_home, sizeof(pseudo_home));
    getcwd(prev_dir, sizeof(prev_dir));
    // printf("%s\n", curr_dir);
    chdir(cwd);

    job_count = 0;

    main_loop();

    return 0;
}