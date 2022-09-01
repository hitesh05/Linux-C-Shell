#include "headers.h"

void foreground(ptr token[])
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed in foreground\n");
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
            perror("Error: execvp failed\n");
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