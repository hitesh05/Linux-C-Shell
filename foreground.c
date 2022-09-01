#include "headers.h"

void foreground(ptr token[])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        red();
        printf("fork failed in foreground\n");
        reset();
        return;
    }

    curr_foreground_job.pid = pid;
    strcpy(curr_foreground_job.name, token[0]);

    if (!pid)
    {
        // child process runs
        int flag = execvp(token[0], token);
        if (!flag)
        {
            red();
            printf("Error: execvp failed\n");
            reset();
            return;
        }
        return;
    }
    else
    {
        // wait for child process to run
        int s;
        waitpid(pid, &s, WUNTRACED);
    }
}