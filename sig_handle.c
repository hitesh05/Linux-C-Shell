#include "headers.h"

void control_c(int sig)
{
    int pid = getpid();
    int check = -1;

    if (pid != SHELL_PID)
    {
        return;
    }

    if (pid == SHELL_PID && curr_foreground_job.pid == check)
    {
        print_prompt();
        fflush(stdout);
    }

    int x = SIGINT;
    if (curr_foreground_job.pid != check)
    {
        kill(curr_foreground_job.pid, x);
    }
    signal(x, control_c);
}

void control_z(int sig)
{
    int pid = getpid();

    if (pid != SHELL_PID)
    {
        return;
    }

    int x = SIGTTIN;
    int y = SIGTSTP;

    int check = -1;

    if (curr_foreground_job.pid != check)
    {
        kill(curr_foreground_job.pid, x);
        kill(curr_foreground_job.pid, y);
        job_count++;
        job_arr[job_count].pid = curr_foreground_job.pid;
        char name[10000];
        strcpy(name, curr_foreground_job.name);
        strcpy(job_arr[job_count].name, name);
        return;
    }
    signal(y, control_z);
    if (pid == SHELL_PID)
    {
        print_prompt();
        fflush(stdout);
    }

    return;
}

void control_d(int sig)
{
    write_history();
    printf("\n\n");
    cyan();
    printf("GOODBYE\n\n");
    reset();
    exit(0);
}