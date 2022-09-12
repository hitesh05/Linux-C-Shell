#include "headers.h"

char filepath[4000];
char *filename;
int is_file = 0;

void discover_d(char dir[], ptr token[], int is_f, int is_dir, char cwd[])
{
    struct dirent **read_files;
    // struct stat st;

    if (is_f)
    {
        // f flag is given
        if (!is_dir)
        {
            // cwd is the root
            char path2[1030];
            if (!strcmp(dir, pseudo_home))
            {
                strcpy(path2, "./");
            }
            int count = scandir(dir, &read_files, NULL, alphasort);
            if (count < 0)
            {
                // printf("error: no such file or directory exists\n");
                return;
            }
            while (count--)
            {
                if (read_files[count]->d_name[0] == '.')
                {
                    continue;
                }
                else
                {
                    char checker[4000];
                    strcpy(checker, dir);
                    strcat(checker, "/");
                    strcat(checker, read_files[count]->d_name);
                    struct stat st;
                    stat(checker, &st);
                    if (is_file)
                    {
                        if (!strcmp(read_files[count]->d_name, filename))
                        {
                            printf("%s%s\n", path2, read_files[count]->d_name);
                            return;
                        }
                    }
                    else
                    {
                        printf("%s%s\n", path2, read_files[count]->d_name);
                    }
                    if (S_ISDIR(st.st_mode))
                    {
                        char path3[1030];
                        sprintf(path3, "%s/%s", dir, read_files[count]->d_name);
                        discover_d(path3, NULL, is_f, 1, cwd);
                    }
                }
                free(read_files[count]);
            }
            free(read_files);
            // return;
        }
        else
        {
            if (token != NULL)
            {
                for (ll i = 1; token[i] != NULL; i++)
                {
                    if ((token[i][0] != '-' && token[i][0] != '"') || (token[i][0] == '-' && strlen(token[i]) == 1))
                    {
                        // token is a directory and not a flag

                        char path[1030];
                        char path2[1030];
                        if (token[i][0] != '\0' && token[i][0] == '~')
                        {
                            sprintf(path, "%s/%s", pseudo_home, &token[i][1]);
                            sprintf(path2, "./%s/", &token[i][1]);
                        }
                        else if (token[i][0] != '\0')
                        {
                            sprintf(path, "%s%s", pseudo_home, &token[i][1]);
                            sprintf(path2, "%s/", token[i]);
                            // printf("%s\n", path);
                        }

                        struct stat st;
                        int flag = stat(path, &st);
                        if (flag < 0)
                        {
                            red();
                            printf("error: ls\n");
                            reset();
                            return;
                        }

                        if (S_ISDIR(st.st_mode))
                        {
                            int count = scandir(path, &read_files, NULL, alphasort);
                            if (count < 0)
                            {
                                red();
                                printf("Error: no such files or directory\n");
                                reset();
                                return;
                            }
                            while (count--)
                            {
                                if (read_files[count]->d_name[0] == '.')
                                {
                                    continue;
                                }
                                else
                                {
                                    strcpy(filepath, path);
                                    strcat(filepath, "/");
                                    strcat(filepath, read_files[count]->d_name);
                                    struct stat s;
                                    stat(filepath, &s);
                                    if (is_file)
                                    {
                                        if (!strcmp(read_files[count]->d_name, filename))
                                        {
                                            printf("%s%s\n", path2, read_files[count]->d_name);
                                        }
                                    }
                                    else
                                    {
                                        printf("%s%s\n", path2, read_files[count]->d_name);
                                    }
                                    if (S_ISDIR(s.st_mode))
                                    {
                                        char path3[2000];
                                        sprintf(path3, "%s/%s", path, read_files[count]->d_name);
                                        discover_d(path3, NULL, is_f, 1, cwd);
                                    }
                                }
                                free(read_files[count]);
                            }
                            free(read_files);
                        }
                    }
                }
            }
            else
            {
                char path[1030];
                char path2[1030];
                char delim[2] = "/";
                // sprintf(path, "%s%s", pseudo_home, &token[i][1]);
                sprintf(path, "%s/", dir);
                // printf("%s\n", path);
                char ans[1030];

                int x = strlen(cwd);
                x++;

                for (int i = x; i < strlen(path); i++)
                {
                    ans[-x + i] = dir[i];
                }
                ans[strlen(dir) - x] = '\0';
                strcpy(path2, "./");
                strcat(path2, ans);
                strcat(path2, "/");

                struct stat st;
                int flag = stat(dir, &st);
                if (flag < 0)
                {
                    red();
                    printf("error: ls\n");
                    reset();
                    return;
                }
                if (S_ISDIR(st.st_mode))
                {
                    int count = scandir(dir, &read_files, NULL, alphasort);
                    if (count < 0)
                    {
                        // printf("Error: no such files or directory\n");
                        return;
                    }
                    while (count--)
                    {
                        if (read_files[count]->d_name[0] == '.')
                        {
                            continue;
                        }
                        else
                        {
                            struct stat s;
                            char checker[4000];
                            strcpy(checker, dir);
                            strcat(checker, "/");
                            strcat(checker, read_files[count]->d_name);
                            // printf("%s\n", checker);
                            stat(checker, &s);
                            if (is_file)
                            {
                                if (!strcmp(read_files[count]->d_name, filename))
                                {
                                    printf("%s%s\n", path2, read_files[count]->d_name);
                                }
                            }
                            else
                            {
                                printf("%s%s\n", path2, read_files[count]->d_name);
                            }
                            if (S_ISDIR(s.st_mode))
                            {
                                char path3[2000];
                                sprintf(path3, "%s/%s", dir, read_files[count]->d_name);
                                discover_d(path3, NULL, is_f, 1, cwd);
                            }
                        }
                        free(read_files[count]);
                    }
                    free(read_files);
                }
            }
        }
    }
    else
    {
        // f flag is not given
        if (!is_dir)
        {
            // cwd is the root
            char path2[1030];
            if (!strcmp(dir, pseudo_home))
            {
                strcpy(path2, "./");
            }
            int count = scandir(dir, &read_files, NULL, alphasort);
            if (count < 0)
            {
                // printf("error: no such file or directory exists\n");
                return;
            }
            while (count--)
            {
                if (read_files[count]->d_name[0] == '.')
                {
                    continue;
                }
                else
                {
                    struct stat st;
                    strcpy(filepath, dir);
                    strcat(filepath, "/");
                    strcat(filepath, read_files[count]->d_name);
                    stat(filepath, &st);
                    if (S_ISDIR(st.st_mode))
                    {
                        printf("%s%s\n", path2, read_files[count]->d_name);
                        char path3[1030];
                        sprintf(path3, "%s/%s", dir, read_files[count]->d_name);
                        discover_d(path3, NULL, is_f, 1, cwd);
                    }
                }
                free(read_files[count]);
            }
            free(read_files);
            // return;
        }
        else
        {
            if (token != NULL)
            {
                for (ll i = 1; token[i] != NULL; i++)
                {
                    if ((token[i][0] != '-' && token[i][0] != '"') || (token[i][0] == '-' && strlen(token[i]) == 1))
                    {
                        // token is a directory and not a flag

                        char path[1030];
                        char path2[1030];
                        if (token[i][0] != '\0' && token[i][0] == '~')
                        {
                            sprintf(path, "%s/%s", pseudo_home, &token[i][1]);
                            sprintf(path2, "./%s/", &token[i][1]);
                        }
                        else if (token[i][0] != '\0')
                        {
                            sprintf(path, "%s%s", pseudo_home, &token[i][1]);
                            sprintf(path2, "%s/", token[i]);
                            // printf("%s\n", path);
                        }

                        struct stat st;
                        int flag = stat(path, &st);
                        if (flag < 0)
                        {
                            red();
                            printf("error: ls\n");
                            reset();
                            return;
                        }

                        if (S_ISDIR(st.st_mode))
                        {
                            int count = scandir(path, &read_files, NULL, alphasort);
                            if (count < 0)
                            {
                                red();
                                printf("Error: no such files or directory\n");
                                reset();
                                return;
                            }
                            while (count--)
                            {
                                if (read_files[count]->d_name[0] == '.')
                                {
                                    continue;
                                }
                                else
                                {
                                    strcpy(filepath, path);
                                    strcat(filepath, "/");
                                    strcat(filepath, read_files[count]->d_name);
                                    struct stat s;
                                    stat(filepath, &s);
                                    if (S_ISDIR(s.st_mode))
                                    {
                                        printf("%s%s\n", path2, read_files[count]->d_name);
                                        char path3[1030];
                                        sprintf(path3, "%s/%s", dir, read_files[count]->d_name);
                                        discover_d(path3, NULL, is_f, 1, cwd);
                                    }
                                }
                                free(read_files[count]);
                            }
                            free(read_files);
                        }
                    }
                }
            }
            else
            {
                char path[1030];
                char path2[1030];
                char delim[2] = "/";
                // sprintf(path, "%s%s", pseudo_home, &token[i][1]);
                sprintf(path, "%s/", dir);
                // printf("%s\n", path);
                char ans[1030];

                int x = strlen(cwd);
                x++;

                for (int i = x; i < strlen(path); i++)
                {
                    ans[-x + i] = dir[i];
                }
                ans[strlen(dir) - x] = '\0';
                strcpy(path2, "./");
                strcat(path2, ans);
                strcat(path2, "/");

                struct stat st;
                int flag = stat(dir, &st);
                if (flag < 0)
                {
                    red();
                    printf("error: ls\n");
                    reset();
                    return;
                }
                if (S_ISDIR(st.st_mode))
                {
                    int count = scandir(dir, &read_files, NULL, alphasort);
                    if (count < 0)
                    {
                        // printf("Error: no such files or directory\n");
                        return;
                    }
                    while (count--)
                    {
                        if (read_files[count]->d_name[0] == '.')
                        {
                            continue;
                        }
                        else
                        {
                            struct stat s;
                            char checker[4000];
                            strcpy(checker, dir);
                            strcat(checker, "/");
                            strcat(checker, read_files[count]->d_name);
                            // printf("%s\n", checker);
                            stat(checker, &s);
                            if (S_ISDIR(s.st_mode))
                            {
                                printf("%s%s\n", path2, read_files[count]->d_name);
                                char path3[1030];
                                sprintf(path3, "%s/%s", dir, read_files[count]->d_name);
                                discover_d(path3, NULL, is_f, 1, cwd);
                            }
                        }
                        free(read_files[count]);
                    }
                    free(read_files);
                }
            }
        }
    }
    return;
}

void discover(char dir[], ptr token[], ll ind)
{
    int is_d = 0;
    int is_f = 0;
    int is_dir = 0;

    struct dirent **read_files;
    struct stat st;

    if (token[0] == NULL)
    {
        red();
        printf("error: invalid command\n");
        reset();
        return;
    }
    else if (ind == 1)
    {
        is_d = 1;
        is_f = 1;
    }
    else
    {
        for (ll i = 1; token[i] != NULL; i++)
        {
            if (!strcmp(token[i], "-d"))
            {
                is_d = 1;
            }
            else if (!strcmp(token[i], "-f"))
            {
                is_f = 1;
            }
            else if (!strcmp(token[i], "-df") || !strcmp(token[i], "-fd"))
            {
                is_d = 1;
                is_f = 1;
            }
            else if (token[i][0] != '-' && token[i][0] != '"')
            {
                is_dir = 1;
            }
            else if (token[i][0] == '-' && strlen(token[i]) == 1)
            {
                is_dir = 1;
            }
        }

        if (token[ind - 1][0] == '"')
        {
            token[ind - 1][strlen(token[ind - 1]) - 1] = '\0';
            filename = &token[ind - 1][1];
            is_file = 1;
        }
    }

    if (!is_f && !is_d)
    {
        is_d = 1;
        is_f = 1;
    }

    if (is_d)
    {
        discover_d(dir, token, is_f, is_dir, dir);
    }
    else if (is_f)
    {
        if (is_dir)
        {
            for (ll i = 1; token[i] != NULL; i++)
            {
                if ((token[i][0] != '-' && token[i][0] != '"') || (token[i][0] == '-' && strlen(token[i]) == 1))
                {
                    // token is a directory and not a flag

                    char path[1030];
                    char path2[1030];
                    if (token[i][0] != '\0' && token[i][0] == '~')
                    {
                        sprintf(path, "%s/%s", pseudo_home, &token[i][1]);
                        sprintf(path2, "./%s/", &token[i][1]);
                    }
                    else if (token[i][0] != '\0')
                    {
                        sprintf(path, "%s%s", pseudo_home, &token[i][1]);
                        sprintf(path2, "%s/", token[i]);
                        // printf("%s\n", path);
                    }

                    struct stat st;
                    int flag = stat(path, &st);
                    if (flag < 0)
                    {
                        red();
                        printf("error: ls\n");
                        reset();
                        return;
                    }

                    if (S_ISDIR(st.st_mode))
                    {
                        int count = scandir(path, &read_files, NULL, alphasort);
                        if (count < 0)
                        {
                            red();
                            printf("Error: no such files or directory\n");
                            reset();
                            return;
                        }
                        // printf("%d\n", count);
                        while (count--)
                        {
                            if (read_files[count]->d_name[0] == '.')
                            {
                                continue;
                            }
                            else
                            {
                                strcpy(filepath, path);
                                strcat(filepath, "/");
                                strcat(filepath, read_files[count]->d_name);
                                struct stat s;
                                stat(filepath, &s);
                                // printf("%s %d\n", read_files[count]->d_name, S_ISREG(s.st_mode));
                                if (!S_ISDIR(s.st_mode))
                                {
                                    if (is_file)
                                    {
                                        if (!strcmp(read_files[count]->d_name, filename))
                                        {
                                            printf("%s%s\n", path2, read_files[count]->d_name);
                                        }
                                    }
                                    else
                                    {
                                        printf("%s%s\n", path2, read_files[count]->d_name);
                                    }
                                }
                            }
                            free(read_files[count]);
                        }
                        free(read_files);
                    }
                }
            }
        }
        else
        {
            // print files of current directory
            int count = scandir(dir, &read_files, NULL, alphasort);
            if (count < 0)
            {
                printf("error: no such file or directory exists\n");
                return;
            }
            while (count--)
            {
                if (read_files[count]->d_name[0] == '.')
                {
                    continue;
                }
                else
                {
                    struct stat s;
                    strcpy(filepath, dir);
                    strcat(filepath, "/");
                    strcat(filepath, read_files[count]->d_name);
                    stat(read_files[count]->d_name, &s);
                    // printf("%s %d\n", read_files[count]->d_name, S_ISDIR(s.st_mode));
                    if (!S_ISDIR(s.st_mode))
                    {
                        if (is_file)
                        {
                            if (!strcmp(read_files[count]->d_name, filename))
                            {
                                printf("./%s\n", read_files[count]->d_name);
                            }
                        }
                        else
                        {
                            printf("./%s\n", read_files[count]->d_name);
                        }
                    }
                }
                free(read_files[count]);
            }
            free(read_files);
        }
    }

    return;
}