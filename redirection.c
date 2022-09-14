#include "headers.h"

int ind = 0;

ptr *split(ptr command)
{
    ll x = sizeof(ptr);
    char **cmdarray = malloc(100 * x);
    if (cmdarray == NULL)
    {
        red();
        printf("malloc failed in redirection\n");
        reset();
    }

    char *cmd = strtok(command, " \t");
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
                printf("realloc failed in redirection\n");
                reset();
            }
        }

        cmdarray[pt] = cmd;
        cmd = strtok(NULL, " ");
    }
    cmdarray[count] = 0;

    return cmdarray;
}

ptr *getpipeargs(ptr command)
{
    ll x = sizeof(ptr);
    ptr *cmdarray = malloc(100 * x);

    if (cmdarray == NULL)
    {
        red();
        printf("malloc failed in redirection\n");
        reset();
    }

    char *cmd = strtok(command, "|");
    int pt;
    for (pt = 0; cmd != NULL; pt++)
    {
        cmdarray[pt] = cmd;

        ll y = sizeof(cmdarray) / x;
        if (pt > y)
        {
            cmdarray = realloc(cmdarray, 100 * x);
            if (cmdarray == NULL)
            {
                red();
                printf("realloc failed in redirection\n");
                reset();
            }
        }
        cmd = strtok(NULL, "|");
    }

    cmdarray[pt] = NULL;

    return cmdarray;
}

int cal_pipes(ptr *cmd)
{
    int n = 0;
    while (cmd[n])
    {
        n++;
    }

    return n;
}

void get_exec(ptr *args, ptr *toret)
{
    // ptr toret[100];

    int i = 0;
    ind = 0;
    for (int j = 0; args[j] != NULL; j++)
    {
        if (!strcmp(">>", args[j]) || !strcmp(args[j], "<") || !strcmp(args[j], ">"))
        {
            continue;
        }
        else if (j > 0)
        {
            if (!strcmp(">>", args[j - 1]) || !strcmp(args[j - 1], "<") || !strcmp(args[j - 1], ">"))
            {
                continue;
            }
            else
            {
                toret[i] = args[j];
                i++;
                ind++;
            }
        }
        else
        {
            toret[i] = args[j];
            i++;
            ind++;
        }
    }

    toret[i] = 0;
    return;
}

void exec_pipe(ptr *args)
{
    ptr token1[100];
    get_exec(args, token1);

    // printf("%s\n", token1[0]);
    // for (int q = 0; token1[q] != NULL; q++)
    // {
    //     printf("%s\n", token1[q]);
    // }

    int check_back = check_for_background(token1, ind);

    if (!strcmp(all_commands[0], token1[0])) // pwd
    {
        pwd();
    }
    else if (!strcmp(all_commands[1], token1[0])) // cd
    {
        cd(token1, ind);
    }
    else if (!strcmp(all_commands[2], token1[0])) // echo
    {
        echo(token1, ind);
    }
    else if (!strcmp(all_commands[3], token1[0])) // ls
    {
        ls(cwd, ind, token1);
    }
    else if (!strcmp(all_commands[4], token1[0])) // pinfo
    {
        pinfo(token1, ind);
    }
    else if (!strcmp(all_commands[5], token1[0])) // history
    {
        history(token1, ind);
    }
    else if (!strcmp(all_commands[6], token1[0])) // discover
    {
        discover(cwd, token1, ind);
    }
    else if (!strcmp(all_commands[7], token1[0])) // fg
    {
        fg(token1, ind);
    }
    else if (!strcmp(all_commands[8], token1[0])) // bg
    {
        bg(token1, ind);
    }
    else if (!strcmp(all_commands[9], token1[0])) // jobs
    {
        jobs(token1, ind);
    }
    else if (!strcmp(all_commands[10], token1[0])) // sig
    {
        sig(token1, ind);
    }
    else if (!strcmp("exit", token1[0]) || !strcmp("quit", token1[0])) // exiting the shell
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

            foreground(token1);

            // printf("%s\n", token1[0]);
            time_t end = time(NULL);
            if ((end - begin) >= 1)
            {
                sprintf(foreground_text, "took %lds", (end - begin));
            }
        }
        else if (check_back > 0)
        {
            // background process
            background(token1);
        }
    }

    return;
}

void pipe_(ptr cmd)
{

    if (!cmd)
    {
        return;
    }

    int dup_stdin = dup(STDIN_FILENO);
    if (dup_stdin < 0)
    {
        printf("error duplicating stdin\n");
        return;
    }

    int dup_stdout = dup(STDOUT_FILENO);
    if (dup_stdout < 0)
    {
        printf("error duplicating stdout\n");
        return;
    }

    ptr *pipeargs = getpipeargs(cmd);

    int num_pipes = cal_pipes(pipeargs);
    // printf("%d\n", num_pipes);

    // int fd_in, fd_out;
    char input_file[1024], output_file[1024];

    for (int i = 0; i < num_pipes; i++)
    {
        ptr *args = split(pipeargs[i]);

        int in = -1, out = -1, append = -1;

        for (int j = 0; args[j] != NULL; j++)
        {
            if (!strcmp(">>", args[j]))
            {
                append = j;
            }

            if (!strcmp(">", args[j]))
            {
                out = j;
            }

            if (!strcmp("<", args[j]))
            {
                in = j;
            }
        }

        if (in > 0)
        {
            strcpy(input_file, args[in + 1]);
            // printf("%s\n", input_file);
            int fd_in = open(input_file, O_RDONLY);
            if (fd_in == -1)
            {
                printf("fd_in: error in redirection, invalid input file\n");
                close(dup_stdin);
                close(dup_stdout);
                return;
            }

            int flag = dup2(fd_in, STDIN_FILENO);
            if (flag < 0)
            {
                printf("dup2: error in redirection, invalid input file\n");
                close(fd_in);
                close(dup_stdin);
                close(dup_stdout);
                return;
            }
        }

        if (out > 0)
        {
            strcpy(output_file, args[out + 1]);

            int fd_out;
            if (append > 0)
            {
                fd_out = open(output_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
            }
            else
            {
                fd_out = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            if (fd_out < 0)
            {
                printf("error in redirection, invalid output file\n");
                close(dup_stdin);
                close(dup_stdout);
                return;
            }

            int flag = dup2(fd_out, STDOUT_FILENO);
            if (flag < 0)
            {
                printf("error in redirection, ivalid input file\n");
                close(dup_stdin);
                close(dup_stdout);
                return;
            }
        }

        if (append > 0 && out > 0)
        {
            printf("Invalid command, cannot have both > and >>\n");
            break;
        }

        int x = num_pipes - 1;
        if (i != x)
        {
            // printf("here\n");
            int pipearr[2];

            int flag = pipe(pipearr);
            if (flag < 0)
            {
                printf("Could not pipe the commands!\n");
                execute("quit");
            }
            // printf("%d\n", pipearr[1]);
            // printf("hii\n");
            int flag2 = dup2(pipearr[1], STDOUT_FILENO);
            // char buf[5];
            // sprintf(buf,"hi\n");
            // write(STDOUT_FILENO,buf,3);
            if (flag2 < 0)
            {
                printf("Could not pipe the commands!\n");
                execute("quit");
            }
            // printf("%s\n", args[0]);
            exec_pipe(args);

            dup2(pipearr[0], STDIN_FILENO);
            dup2(dup_stdout, STDOUT_FILENO);
            close(pipearr[1]);
        }
        else
        {
            // printf("%s\n", args[0]);
            exec_pipe(args);
        }
    }

    dup2(dup_stdin, STDIN_FILENO);
    dup2(dup_stdout, STDOUT_FILENO);
    close(dup_stdin);
    close(dup_stdout);
}
