#include "headers.h"

void background(ptr token[])
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        red();
        printf("error: fork falied in background\n");
        reset();
        return;
    }

    setpgid(0, 0);

    if (!pid)
    {
        pid_t pid2 = getpid();
        int test = execvp(token[0], token);
        if (!test)
        {
            red();
            printf("error in execvp\n");
            reset();
        }
        return;
    }
    else
    {
        job_count++;
        cyan();
        printf("[%lld] %d %s\n", job_count, pid, token[0]); // DETAILS OF BG PROCESS
        reset();
        job_arr[job_count - 1].pid = pid;
        job_arr[job_count-1].num = job_count;
        strcpy(job_arr[job_count - 1].name, token[0]); // TO KILL PROCESSES
    }
}