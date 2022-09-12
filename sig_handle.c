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

void sig_exec(int job_num, int sig_num)
{
    int flag = 0;
    for (ll i = 0; i < job_count; i++)
    {
        if (i == job_num)
        {
            flag = 1;
            printf("sending signal to process %s : %d\n", job_arr[job_num].name, job_arr[job_num].pid);

            int x = kill(job_arr[job_num].pid, sig_num);
            if (x < 0)
            {
                printf("error in sending signal\n");
                return;
            }
            return;
        }
    }

    if (flag == 0)
    {
        printf("No such process found\n");
    }
    return;
}

void sig(ptr token[], ll ind)
{
    if (ind != 3)
    {
        printf("Incorrect number of commands\n");
        return;
    }

    int job_num, sig_num;

    job_num = atoi(token[1]);
    if (job_num < 0)
    {
        printf("Incorrect job number\n");
        return;
    }

    sig_num = atoi(token[1]);
    if (sig_num < 0)
    {
        printf("Incorrect signal number\n");
        return;
    }

    sig_exec(job_num, sig_num);
    return;
}