#include "headers.h"

int job_num;

void bg_exec()
{
    int flag = 0;
    for (ll i = 0; i < job_count; i++)
    {
        if (job_arr[i].num == job_num)
        {
            flag = 1;
            int x = kill(job_arr[job_num].pid, SIGCONT);
            if (x < 0)
            {
                printf("error in bg\n");
                return;
            }
        }
    }

    if (flag == 0)
    {
        printf("No such process found\n");
        return;
    }
}

void bg(ptr token[], ll ind)
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

    bg_exec();
    return;
}