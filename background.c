#include "headers.h"

void background(ptr token[])
{
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("error: fork falied in background\n");
        return;
    }

    setpgid(0, 0);

    if (!pid)
    {
        pid_t pid2 = getpid();
        int test = execvp(token[0], token);
        if (!test)
        {
            perror("error in execvp\n");
        }
        return;
    }
    else
    {
        job_count++;
        printf("[%lld] %d %s\n", job_count, pid, token[0]); // DETAILS OF BG PROCESS
        job_arr[job_count].pid = pid;
        strcpy(job_arr[job_count].name, token[0]); // TO KILL PROCESSES
    }
}