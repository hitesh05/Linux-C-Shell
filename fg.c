#include "headers.h"

int job_num = 0;
int fd = 0;

void fg_exec()
{
    int flag = 0;
    for (ll i = 0; i < job_count; i++)
    {
        if (i == job_num)
        {
            flag = 1;
            printf("Process %s with pid %d continuing in foreground .... \n", job_arr[job_num].name, job_arr[job_num].pid);
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(fd, job_arr[job_num].pid);

            strcpy(curr_foreground_job.name, job_arr[job_num].name);
            curr_foreground_job.pid = job_arr[job_num].pid;

            int ret = kill(job_arr[job_num].pid, SIGCONT);
            if (ret == -1)
            {
                printf("error killing the process\n");
                return;
            }
            del_process(job_arr[job_num].pid);

            int wpid;
            int x = waitpid(-1, &wpid, WUNTRACED);

            tcsetpgrp(fd, getpid());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            if (WIFSTOPPED(wpid))
            {
                printf("Process %s with pid %d stopped .... \n", job_arr[job_num].name, job_arr[job_num].pid);
            }
            return;
        }
    }

    if (flag == 0)
    {
        printf("No such process found\n");
        return;
    }
}

void fg(ptr token[], ll ind)
{
    if (ind != 2)
    {
        printf("Incorrect number of commands\n");
        return;
    }

    job_num = atoi(token[1]);
    if (job_num < 0)
    {
        printf("Incorrect job number\n");
        return;
    }

    fg_exec();
    return;
}