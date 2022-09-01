#include "headers.h"

void cd(ptr token[], ll ind)
{
    // printf("%s\n", token[1][0]);
    if (ind > 2)
    {
        red();
        printf("Too many arguments for cd command");
        reset();
        return;
    }
    else if (ind > 1)
    {
        char dir[1024];
        if ((!strcmp(token[1], "~")) || (!strcmp(token[1], "~/")))
        {
            chdir(pseudo_home);
            strcpy(dir, pseudo_home);
            return;
        }
        else if (!strcmp(token[1], "-"))
        {
            printf("%s\n", prev_dir);
            getcwd(dir, sizeof(dir));
            int f = chdir(prev_dir);
            strcpy(prev_dir, dir);
            if (f < 0)
            {
                red();
                printf("CD: error with flag\n");
                reset();
            }
        }
        else if (token[1][0] == '~' && strlen(token[1]) > 1)
        {
            token[1][0] = '.';
            int flag = chdir(token[1]);
            if (flag < 0)
            {
                red();
                printf("CD: No such file or directory\n");
                reset();
            }
            else
            {
                getcwd(dir, sizeof(dir));
            }
        }
        else
        {
            int flag = chdir(token[1]);
            if (flag < 0)
            {
                red();
                printf("CD: No such file or directory\n");
                reset();
            }
            else
            {
                getcwd(dir, sizeof(dir));
            }
        }
        return;
    }
    else
    {
        chdir(pseudo_home);
    }
}