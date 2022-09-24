#include "headers.h"
char autocomp_ans[10000][10000];
int x = 0;

int ls2(ptr cmd)
{
    ll count = 0;
    getcwd(cwd, MAX_SIZE);
    int x = 0;

    struct dirent **read_files;
    count = scandir(".", &read_files, NULL, alphasort);
    if (count < 0)
    {
        red();
        printf("error: autocomplete\n");
        reset();
    }

    while (count--)
    {
        if (read_files[count]->d_name[0] == '.')
        {
            continue;
        }
        else
        {
            char checker[10000];
            struct stat st;
            strcpy(checker, cwd);
            strcat(checker, "/");
            strcat(checker, read_files[count]->d_name);
            stat(checker, &st);

            int is_same = 1;
            int len1 = strlen(cmd), len2 = strlen(read_files[count]->d_name);
            if (len2 < len1)
            {
                len1 = len2;
            }

            for (int k = 0; k < len1; k++)
            {
                if (read_files[count]->d_name[k] != cmd[k])
                {
                    is_same -= 1;
                    break;
                }
            }
            if (is_same == 1)
            {
                char filename[MAX_SIZE];
                if (S_ISDIR(st.st_mode))
                {
                    strcpy(autocomp_ans[x], read_files[count]->d_name);
                    strcat(autocomp_ans[x], "/");
                }
                else
                {
                    strcpy(autocomp_ans[x], read_files[count]->d_name);
                    strcat(autocomp_ans[x], " ");
                }
                x++;
            }
            free(read_files[count]);
        }
    }

    return x;
}

int autocomplete(ptr cmd, ptr val)
{
    char last_token[MAX_SIZE];
    char exec[MAX_SIZE];
    strcpy(exec, cmd);
    getcwd(cwd, MAX_SIZE);

    char *ls_ = "ls";
    token[0] = strtok(ls_, " \t\n\r");

    char *tok = strtok(exec, " ");
    int num_files = 0;

    if (tok == NULL)
    {
        printf("\n");
        ls(cwd, 0, token);
        print_prompt();
        return 1;
    }
    else
    {
        char *space = " ";
        while (tok != NULL)
        {
            strcpy(last_token, tok);
            tok = strtok(NULL, space);
        }

        int len = strlen(cmd);
        if (cmd[len - 1] == ' ')
        {
            num_files = ls2("\0");
        }
        else
        {
            num_files = ls2(last_token);
        }

        if (num_files != 1)
        {
            char *eol = "\n";

            printf("%s", eol);
            while (num_files--)
            {
                printf("%s%s", autocomp_ans[num_files], eol);
            }
            print_prompt();
            for (int i = 0; i < strlen(cmd); i++)
            {
                printf("%c", cmd[i]);
                val[i] = cmd[i];
            }
            int l = strlen(cmd);
            val[l] = '\0';
        }
        else
        {
            int len = strlen(last_token);
            int len1 = strlen(cmd);
            int len2 = len1 - len;
            for (int k = 0; k < len; k++)
            {
                printf("\b \b");
            }

            for (int i = 0; i < len2; i++)
            {
                val[i] = cmd[i];
            }

            if (len2 != 0)
            {
                strcat(val, autocomp_ans[0]);
            }
            else
            {
                strcpy(val, autocomp_ans[0]);
            }

            printf("%s", autocomp_ans[0]);
        }
    }
    return num_files;
}