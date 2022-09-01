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
        red();
        printf("Error: init_history\n");
        reset();
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
        red();
        printf("error opening history.txt\n");
        reset();
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
        red();
        printf("error: unable to write to history\n");
        reset();
    }

    return;
}

void history(ptr token[], ll ind)
{
    if (ind > 2)
    {
        red();
        printf("error: too many arguments\n");
        reset();
        return;
    }

    if (token[1] == NULL)
    {
        int size = (historyy.num < 10) ? historyy.num : 10;
        if (!size)
        {
            red();
            printf("no history found\n");
            reset();
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
            red();
            printf("negative integer not allowed\n");
            reset();
            return;
        }

        int size = (historyy.num < 20) ? historyy.num : 20;
        if (!size)
        {
            red();
            printf("history is empty\n");
            reset();
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