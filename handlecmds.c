#include "headers.h"

void init_allcmds()
{
    strcpy(all_commands[0], "pwd");
    strcpy(all_commands[1], "cd");
    strcpy(all_commands[2], "echo");
    strcpy(all_commands[3], "ls");
    strcpy(all_commands[4], "pinfo");
    strcpy(all_commands[5], "history");
    strcpy(all_commands[6], "discover");
    strcpy(all_commands[7], "fg");
    strcpy(all_commands[8], "bg");
    strcpy(all_commands[9], "jobs");
    strcpy(all_commands[10], "sig");
}

ptr *splitcmd(ptr command)
{
    ll x = sizeof(ptr);
    char **cmdarray = malloc(100 * x);
    if (cmdarray == NULL)
    {
        red();
        printf("malloc failed in handlecmds\n");
        reset();
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
                red();
                printf("realloc failed in handlecmds\n");
                reset();
            }
        }

        cmdarray[pt] = cmd;
        cmd = strtok(NULL, ";\n");
    }
    cmdarray[count] = 0;

    return cmdarray;
}

int check_for_background(ptr token[], ll ind)
{
    if (!ind)
    {
        red();
        printf("Invalid argument\n");
        reset();
        return -1;
    }

    int flag = 0;
    if (ind >= 2)
    {
        if (!strcmp(token[ind - 1], "&"))
        {
            token[ind - 1] = NULL;
            ind--;
            flag = 1;
        }
        else if (token[ind - 1][strlen(token[ind - 1]) - 1] == '&')
        {
            flag = 1;
            token[ind - 1][strlen(token[ind - 1]) - 1] = '\0';
            token[ind] = NULL;
            ind--;
        }
    }
    else if (ind >= 1)
    {
        if (token[ind - 1][strlen(token[ind - 1]) - 1] == '&')
        {
            flag = 1;
            token[ind - 1][strlen(token[ind - 1]) - 1] = '\0';
            token[ind] = NULL;
        }
    }

    return flag;
}

void execute(ptr cmd)
{
    add_history(cmd);

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

    int is_pipe = 0;
    int pipenum = 0;
    while (token[pipenum] != NULL)
    {
        if (strcmp(token[pipenum], ">>") == 0 || strcmp(token[pipenum], "<") == 0 || strcmp(token[pipenum], "|") == 0 || strcmp(token[pipenum], ">") == 0)
        {
            // call piper
            pipe_(cpy_cmd);
            is_pipe = 1;
            break;
        }
        pipenum++;
    }

    if (is_pipe == 0)
    {
        int check_back = check_for_background(token, ind);

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
        else if (!strcmp(all_commands[5], token[0])) // history
        {
            history(token, ind);
        }
        else if (!strcmp(all_commands[6], token[0])) // discover
        {
            discover(cwd, token, ind);
        }
        else if (!strcmp(all_commands[7], token[0])) // fg
        {
            fg(token, ind);
        }
        else if (!strcmp(all_commands[8], token[0])) // bg
        {
            bg(token, ind);
        }
        else if (!strcmp(all_commands[9], token[0])) // jobs
        {
            jobs(token, ind);
        }
        else if (!strcmp(all_commands[10], token[0])) // sig
        {
            sig(token, ind);
        }
        else if (!strcmp("exit", token[0]) || !strcmp("quit", token[0])) // exiting the shell
        {
            // write to history
            write_history();
            printf("\n\n");
            cyan();
            printf("GOODBYE\n\n");
            reset();
            exit(0);
        }
        else // foreground and background processes
        {
            if (check_back == 0)
            {
                // foreground process
                time_t begin = time(NULL);

                foreground(token);

                time_t end = time(NULL);
                if ((end - begin) >= 1)
                {
                    sprintf(foreground_text, "took %lds", (end - begin));
                }
            }
            else if (check_back > 0)
            {
                // background process
                background(token);
            }
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