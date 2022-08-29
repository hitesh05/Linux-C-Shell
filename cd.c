#include "headers.h"

void cd(ptr token[], ll ind)
{
    // printf("%s\n", token[1][0]);
    if (ind > 2)
    {
        perror("Too many arguments for cd command");
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
                printf("CD: error with flag\n");
            }
        }
        else if (token[1][0] == '~' && strlen(token[1]) > 1)
        {
            token[1][0] = '.';
            int flag = chdir(token[1]);
            if (flag < 0)
            {
                printf("CD: No such file or directory\n");
            }
            else
            {
                getcwd(dir, sizeof(dir));
            }
        }
        else
        {
            if (!strcmp(token[1], ".."))
            {
                char dir2[1024];
                getcwd(dir2, sizeof(dir2));
                if (!strcmp(dir2, pseudo_home))
                {
                    printf("%s\n", dir2);
                    return;
                }
            }
            int flag = chdir(token[1]);
            if (flag < 0)
            {
                printf("CD: No such file or directory\n");
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