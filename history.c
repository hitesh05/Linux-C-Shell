#include "headers.h"

void init_history()
{
    strcpy(hisfile_path, "");
    strcpy(hisfile_path, pseudo_home);
    strcat(hisfile_path, "/history.txt");

    FILE *fp = fopen(hisfile_path, "r");
    if (!fp)
    {
        fp = fopen(hisfile_path, "w");
        historyy.num = 0;
        fclose(fp);
        return;
    }

    ptr line_ptr = NULL;
    size_t n = 0;
    while (getline(&line_ptr, &n, fp) != -1)
    {
        int len = strlen(line_ptr) - 1;
        if (line_ptr[len] == '\n')
        {
            line_ptr[len] = '\0';
        }
        strcpy(historyy.name[historyy.num], line_ptr);
        historyy.num += 1;
    }
    int flag = fclose(fp);
    if (flag < 0)
    {
        perror("Error: init_history\n");
    }
    return;
}

void add_history(ptr token)
{
    if (!token)
    {
        return;
    }

    if (strcmp(token, historyy.name[historyy.num - 1]))
    {
        strcpy(historyy.name[historyy.num % 20], token);
        historyy.num += 1;
    }

    return;
}

void write_history()
{
    FILE *fp = fopen(hisfile_path, "w");
    if (!fp)
    {
        perror("error opening history.txt\n");
        return;
    }

    if(!historyy.num){
        return;
    }
    if (historyy.num <= 20)
    {
        for (int i = 0; i < historyy.num; i++)
        {
            fprintf(fp, "%s\n", historyy.name[i]);
        }
    }
    else
    {
        int start = historyy.num % 20;
        for (int i = start; i < 20; i++)
        {
            fprintf(fp, "%s\n", historyy.name[i]);
        }
        for (int i = 0; i < start; i++)
        {
            fprintf(fp, "%s\n", historyy.name[i]);
        }
    }

    int check = fclose(fp);
    if (check < 0)
    {
        perror("error: unable to write to history\n");
    }

    return;
}

void history(ptr token[], ll ind)
{
    if (ind > 2)
    {
        perror("error: too many arguments\n");
        return;
    }

    if (token[1] == NULL)
    {
        int size = (historyy.num < 10) ? historyy.num : 10;
        if (!size)
        {
            perror("no history found\n");
            return;
        }

        int x = 0;
        int i = historyy.num - 1;
        while (1)
        {
            if (i < 0)
            {
                i = size - 1;
            }
            printf("[%d]: %s\n", x + 1, historyy.name[i]);
            x++;
            i--;
            if (x == size)
            {
                break;
            }
        }
    }
    else
    {
        int tot = atoi(token[1]);

        if (tot < 0)
        {
            perror("negative integer not allowed\n");
            return;
        }

        int size = (historyy.num < 20) ? historyy.num : 20;
        if (!size)
        {
            perror("history is empty\n");
            return;
        }
        if (tot > historyy.num)
        {
            fprintf(stderr, "history only has %d entries\n\n", historyy.num);
            tot = historyy.num;
        }

        int x = 0;
        int i = historyy.num - 1;
        while (1)
        {
            if (i < 0)
            {
                i = size - 1;
            }
            printf("[%d]: %s\n", x + 1, historyy.name[i]);
            x++;
            i--;
            if (x == tot)
            {
                break;
            }
        }
    }
}