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

void del_process(int id)
{
    // printf("Entered killproc with job count = %lld and proc number=%d\n", job_count, id);
    int flag = 0;
    for (ll i = 1; i <= job_count; i++)
    {
        if (job_arr[i].pid == id)
        {
            flag = 1;
            for (ll j = i; j < job_count; j++)
                job_arr[j] = job_arr[j + 1];
            job_count--;
        }
    }
    if (flag == 0)
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

    init_history();

    main_loop();

    return 0;
}