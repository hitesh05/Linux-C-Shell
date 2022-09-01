#include "headers.h"

void pinfo(ptr token[], ll ind)
{
    if (ind > 2)
    {
        red();
        printf("pinfo: Too many arguments\n");
        reset();
        return;
    }

    pid_t pid;

    if (ind == 1)
    {
        pid = getpid();
    }
    else
    {
        pid = strtol(token[1], NULL, 10);
    }

    if (pid <= 0)
    {
        red();
        printf("Invalid pid\n");
        reset();
        return;
    }

    // accessing /proc/[pid]/status file
    char proc_stat_file[1024];
    sprintf(proc_stat_file, "/proc/%d/status", pid);
    FILE *status_file = fopen(proc_stat_file, "r");
    if (!status_file)
    {
        red();
        printf("pinfo: error opening status file");
        reset();
        return;
    }
    int x = 0;
    char buffer[500];
    char delim[2] = ":";

    // print pid
    printf("pid : %d\n", pid);

    while (fgets(buffer, sizeof(buffer), status_file))
    {
        if (x == 2)
        {
            // printf("%s", buffer);
            char *str = strtok(buffer, delim);
            str = strtok(NULL, " \t");
            if (str)
            {
                printf("process status : %s", str);
                if (pid == getpgrp())
                {
                    printf("+\n");
                }
                else
                {
                    printf("\n");
                }
            }
            else
            {
                str[0] = '\0';
                printf("process status : %s\n", str);
            }
        }
        else if (x == 17)
        {
            // printf("%s", buffer);
            char *str = strtok(buffer, delim);
            str = strtok(NULL, " \t");
            if (str)
            {
                printf("memory : %s {Virtual Memory}\n", str);
            }
            else
            {
                str[0] = '\0';
                printf("memory : %s {Virtual Memory}\n", str);
            }
            break;
        }
        x++;
    }
    fclose(status_file);

    // accessing the executable in /proc/[pid]/exe
    char proc_exe_file[1024];
    char read[1024];
    sprintf(proc_exe_file, "/proc/%d/exe", pid);

    int flag = readlink(proc_exe_file, read, sizeof(read));
    if (!flag)
    {
        red();
        printf("pinfo: Error opening executable\n");
        reset();
        return;
    }

    char exe[1024];
    read[flag] = '\0';
    if (!strcmp(read, pseudo_home))
    {
        strcpy(exe, "~");
    }
    else if (strlen(read) < strlen(pseudo_home))
    {
        strcpy(exe, read);
    }
    else
    {
        char *t;
        char t1[MAX_SIZE];
        // printf("pseudo home: %s\n", pseudo_home);
        char copy[MAX_SIZE];
        strcpy(copy, pseudo_home);
        t = strtok(copy, "/");
        while (t != NULL)
        {
            strcpy(t1, t);
            t = strtok(NULL, "/");
        }
        t = strtok(read, "/");
        while (strcmp(t, t1))
        {
            t = strtok(NULL, "/");
        }
        t = strtok(NULL, "");
        strcpy(exe, "~/");
        strcat(exe, t);
    }

    printf("executable Path : %s\n", exe);

    return;
}