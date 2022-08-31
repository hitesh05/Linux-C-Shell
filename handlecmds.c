#include "headers.h"

void init_allcmds()
{
    strcpy(all_commands[0], "pwd");
    strcpy(all_commands[1], "cd");
    strcpy(all_commands[2], "echo");
    strcpy(all_commands[3], "ls");
    strcpy(all_commands[4], "pinfo");
}

ptr *splitcmd(ptr command)
{
    ll x = sizeof(ptr);
    char **cmdarray = malloc(100 * x);
    if (cmdarray == NULL)
    {
        printf("malloc failed in handlecmds\n");
    }

    char *cmd = strtok(command, ";\n");
    ll count = 0;
    for (int pt = 0; cmd != NULL; pt++, count++)
    {
        ll y = sizeof(cmdarray) / x;
        if (pt > y)
        {
            cmdarray = realloc(cmdarray, 100 * x);
            if (cmdarray == NULL)
            {
                printf("realloc failed in handlecmds\n");
            }
        }

        cmdarray[pt] = cmd;
        cmd = strtok(NULL, ";\n");
    }
    cmdarray[count] = 0;

    return cmdarray;
}

void execute(ptr cmd)
{
    char cpy_cmd[1024];
    strcpy(cpy_cmd, cmd);

    getcwd(cwd, sizeof(cwd));

    token[0] = strtok(cmd, " \t\r\n");
    ll ind = 0;

    foreground_text[0] = '\0';

    while (token[ind] != NULL)
    {
        ind++;
        token[ind] = strtok(NULL, " \t\r\n");
    }

    if (token[0][0] == '.') // for executables
    {
        time_t begin = time(NULL);

        foreground(token);

        time_t end = time(NULL);

        if ((end - begin) >= 1)
        {
            sprintf(foreground_text, "took %lds", (end - begin));
        }
    }
    if (!strcmp(all_commands[0], token[0])) // pwd
    {
        pwd();
    }
    else if (!strcmp(all_commands[1], token[0])) // cd
    {
        cd(token, ind);
    }
    else if (!strcmp(all_commands[2], token[0])) // echo
    {
        echo(token, ind);
    }
    else if (!strcmp(all_commands[3], token[0])) // ls
    {
        ls(cwd, ind, token);
    }
    else if (!strcmp(all_commands[4], token[0])) // pinfo
    {
        pinfo(token, ind);
    }
    else if (!strcmp("vi", token[0]) || !strcmp("emacs", token[0]) || !strcmp("gedit", token[0])) // foreground processes
    {
        time_t begin = time(NULL);

        foreground(token);

        time_t end = time(NULL);
        if ((end - begin) >= 1)
        {
            sprintf(foreground_text, "took %lds", (end - begin));
        }
    }
    else if (!strcmp("exit", token[0]) || !strcmp("quit", token[0]))
    {
        // write to history
        printf("GOODBYE\n");
        exit(0);
    }
    else // foreground processes
    {
        time_t begin = time(NULL);

        foreground(token);

        time_t end = time(NULL);
        if ((end - begin) >= 1)
        {
            sprintf(foreground_text, "took %lds", (end - begin));
        }
    }
}

ptr execcommand(ptr command)
{
    init_allcmds();

    ptr *cmdarray = splitcmd(command);

    for (int i = 0; cmdarray[i] != NULL; i++)
    {
        execute(cmdarray[i]);
    }
}