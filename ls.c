#include "headers.h"

char filepath[MAX_SIZE];
char checker[4000];

void print_l(char *filename, char *file)
{
    // sample:
    // -rw-rw-r-- 1 hitesh hitesh  1492 Aug 29 15:21 cd.c
    struct stat st;
    stat(filename, &st);

    // getting filetype
    char filetype = ' ';
    if (S_ISREG(st.st_mode))
    {
        filetype = '-';
    }
    else if (S_ISDIR(st.st_mode))
    {
        filetype = 'd';
    }
    else if (S_ISCHR(st.st_mode))
    {
        filetype = 'c';
    }
    else if (S_ISBLK(st.st_mode))
    {
        filetype = 'b';
    }
    else if (S_ISFIFO(st.st_mode))
    {
        filetype = 'p';
    }
    else if (S_ISLNK(st.st_mode))
    {
        filetype = 'l';
    }
    else if (S_ISSOCK(st.st_mode))
    {
        filetype = 's';
    }

    // getting permissions
    char str[10] = "";
    if (!(st.st_mode & S_IRUSR) ? strcat(str, "-") : strcat(str, "r"))
        ;
    if (!(st.st_mode & S_IWUSR) ? strcat(str, "-") : strcat(str, "w"))
        ;
    if (!(st.st_mode & S_IXUSR) ? strcat(str, "-") : strcat(str, "x"))
        ;
    if (!(st.st_mode & S_IRGRP) ? strcat(str, "-") : strcat(str, "r"))
        ;
    if (!(st.st_mode & S_IWGRP) ? strcat(str, "-") : strcat(str, "w"))
        ;
    if (!(st.st_mode & S_IXGRP) ? strcat(str, "-") : strcat(str, "x"))
        ;
    if (!(st.st_mode & S_IROTH) ? strcat(str, "-") : strcat(str, "r"))
        ;
    if (!(st.st_mode & S_IWOTH) ? strcat(str, "-") : strcat(str, "w"))
        ;
    if (!(st.st_mode & S_IXOTH) ? strcat(str, "-") : strcat(str, "x"))
        ;

    // gettng links
    ll links = st.st_nlink;

    // getting owner name and group name
    struct passwd *pw = getpwuid(st.st_uid);
    struct group *gr = getgrgid(st.st_gid);

    // getting size of file
    ll size = st.st_size;

    // time when file was last accessed
    char date[25];
    strftime(date, 20, "%b %d   %H:%M", localtime(&(st.st_ctime)));

    // printing elaborate permissions
    printf("%c%-9s %-5lld %-12s %-12s %-8lld %-24s ", filetype, str, links, pw->pw_name, gr->gr_name, size, date);

    if (S_ISDIR(st.st_mode))
    {
        blue();
    }
    else if (st.st_mode & S_IXUSR)
    {
        yellow();
    }
    printf("%s\n", file);
    reset();

    return;
}

void ls_l(ptr token[], int is_a, int is_dir, char dir[])
{
    struct dirent **read_files;
    struct stat st;

    ll count = 0;

    if (!is_dir)
    {
        // no directory mentioned
        count = scandir(".", &read_files, NULL, alphasort);
        if (count < 0)
        {
            red();
            printf("error: no such file or directory exists\n");
            reset();
        }
        else if (is_a)
        {
            // -a flag exists
            ll total = 0;
            ll copy = count;
            while (count--)
            {
                strcpy(checker, dir);
                strcat(checker, "/");
                strcat(checker, read_files[count]->d_name);
                stat(checker, &st);
                total += st.st_blocks;
            }
            total /= 2; // 512 block len
            printf("Total %lld\n", total);
            count = copy;
            while (count--)
            {
                print_l(read_files[count]->d_name, read_files[count]->d_name);
                free(read_files[count]);
            }
            free(read_files);
            reset();
        }
        else
        {
            // -a flag does not exist
            ll total = 0;
            ll copy = count;
            while (count--)
            {
                if (read_files[count]->d_name[0] == '.')
                {
                    continue;
                }
                else
                {
                    strcpy(checker, dir);
                    strcat(checker, "/");
                    strcat(checker, read_files[count]->d_name);
                    stat(read_files[count]->d_name, &st);
                    total += st.st_blocks;
                }
            }
            total /= 2; // 512 block len
            printf("Total %lld\n", total);
            count = copy;
            while (count--)
            {
                if (read_files[count]->d_name[0] == '.')
                {
                    continue;
                }
                else
                {
                    print_l(read_files[count]->d_name, read_files[count]->d_name);
                    free(read_files[count]);
                }
            }
            free(read_files);
            reset();
        }
    }
    else
    {
        // directory is present in cmd
        for (ll i = 1; token[i] != NULL; i++)
        {
            if (token[i][0] != '-' || (token[i][0] == '-' && strlen(token[i]) == 1))
            {
                // token is a directory and not a flag
                char path[1030];
                if (token[i][0] != '\0' && token[i][0] == '~')
                {
                    sprintf(path, "%s/%s", pseudo_home, &token[i][1]);
                }
                else if (token[i][0] != '\0')
                {
                    strcpy(path, token[i]);
                }
                // printf("%s\n", path);

                struct stat s;
                int flag = stat(path, &s);
                if (flag < 0)
                {
                    red();
                    printf("error: ls\n");
                    reset();
                    return;
                }

                if (S_ISDIR(s.st_mode))
                {
                    count = scandir(path, &read_files, NULL, alphasort);
                    if (count < 0)
                    {
                        red();
                        printf("Error: no such files or directory\n");
                        reset();
                    }
                    else if (!is_a)
                    {
                        // no -a flag in cmd
                        ll total = 0;
                        ll copy = count;

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
                                stat(filepath, &st);

                                total += st.st_blocks;
                            }
                        }
                        total /= 2;
                        printf("Total %lld\n", total);
                        count = copy;
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
                                stat(filepath, &st);
                                print_l(filepath, read_files[count]->d_name);
                                reset();
                            }
                            free(read_files[count]);
                            reset();
                        }
                        free(read_files);
                        reset();
                    }
                    else
                    {
                        // -a flag exists
                        ll total = 0;
                        ll copy = count;

                        while (count--)
                        {
                            strcpy(filepath, path);
                            strcat(filepath, "/");
                            strcat(filepath, read_files[count]->d_name);
                            stat(filepath, &st);

                            total += st.st_blocks;
                        }
                        total /= 2;
                        printf("Total %lld\n", total);
                        count = copy;
                        while (count--)
                        {
                            strcpy(filepath, path);
                            strcat(filepath, "/");
                            strcat(filepath, read_files[count]->d_name);
                            stat(filepath, &st);
                            print_l(filepath, read_files[count]->d_name);
                            free(read_files[count]);
                            reset();
                        }
                        free(read_files);
                        reset();
                    }
                }
                else
                {
                    // its a file
                    print_l(path, path);
                }
            }
        }
    }

    return;
}

void ls(char dir[], ll ind, ptr token[])
{
    int is_l = 0;
    int is_a = 0;
    int is_dir = 0;
    ll count = 0;

    struct dirent **read_files;
    struct stat st;

    for (ll i = 1; token[i] != NULL; i++)
    {
        if (!strcmp(token[i], "-l"))
        {
            is_l = 1;
        }
        else if (!strcmp(token[i], "-a"))
        {
            is_a = 1;
        }
        else if (!strcmp(token[i], "-al") || !strcmp(token[i], "-la"))
        {
            is_l = 1;
            is_a = 1;
        }
        else if (token[i][0] != '-')
        {
            is_dir = 1;
        }
        else if (token[i][0] == '-' && strlen(token[i]) == 1)
        {
            is_dir = 1;
        }
    }

    if (is_l)
    {
        ls_l(token, is_a, is_dir, dir);
    }
    else
    {
        // no -l flag
        if (!is_dir)
        {
            // no directory mentioned alongside ls
            count = scandir(".", &read_files, NULL, alphasort);
            if (count < 0)
            {
                red();
                printf("error: no such file or directory exists\n");
                reset();
            }
            else if (is_a)
            {
                // -a flag exists
                while (count--)
                {
                    strcpy(checker, dir);
                    strcat(checker, "/");
                    strcat(checker, read_files[count]->d_name);
                    stat(checker, &st);
                    if (S_ISDIR(st.st_mode))
                    {
                        blue();
                    }
                    else if (st.st_mode & S_IXUSR)
                    {
                        yellow();
                    }
                    printf("%s ", read_files[count]->d_name);
                    reset();
                    free(read_files[count]);
                }
                free(read_files);
            }
            else
            {
                // -a flag does not exist
                while (count--)
                {
                    if (read_files[count]->d_name[0] == '.')
                    {
                        continue;
                    }
                    else
                    {
                        strcpy(checker, dir);
                        strcat(checker, "/");
                        strcat(checker, read_files[count]->d_name);
                        stat(checker, &st);
                        if (S_ISDIR(st.st_mode))
                        {
                            blue();
                        }
                        else if (st.st_mode & S_IXUSR)
                        {
                            yellow();
                        }
                        printf("%s ", read_files[count]->d_name);
                        reset();
                    }
                    free(read_files[count]);
                }
                free(read_files);
            }
        }
        else
        {
            // directories mentioned alongside ls
            for (ll i = 1; token[i] != NULL; i++)
            {
                if (token[i][0] != '-' || (token[i][0] == '-' && strlen(token[i]) == 1))
                {
                    // token is a directory and not a flag

                    char path[1030];
                    if (token[i][0] != '\0' && token[i][0] == '~')
                    {
                        sprintf(path, "%s/%s", pseudo_home, &token[i][1]);
                    }
                    else if (token[i][0] != '\0')
                    {
                        strcpy(path, token[i]);
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
                        count = scandir(path, &read_files, NULL, alphasort);
                        if (count < 0)
                        {
                            printf("Error: no such files or directory\n");
                        }
                        else if (!is_a)
                        {
                            // no -a flag in cmd
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
                                    stat(filepath, &st);
                                    if (S_ISDIR(st.st_mode))
                                    {
                                        blue();
                                    }
                                    else if (st.st_mode & S_IXUSR)
                                    {
                                        yellow();
                                    }
                                    printf("%s ", read_files[count]->d_name);
                                    reset();
                                }
                                free(read_files[count]);
                                reset();
                            }
                            free(read_files);
                            reset();
                        }
                        else
                        {
                            // -a flag exists
                            while (count--)
                            {
                                strcpy(filepath, path);
                                strcat(filepath, "/");
                                strcat(filepath, read_files[count]->d_name);
                                stat(filepath, &st);
                                if (S_ISDIR(st.st_mode))
                                {
                                    blue();
                                }
                                else if (st.st_mode & S_IXUSR)
                                {
                                    yellow();
                                }
                                printf("%s ", read_files[count]->d_name);
                                reset();
                                free(read_files[count]);
                            }
                            free(read_files);
                            reset();
                        }
                    }
                    else
                    {
                        // its a file
                        printf("%s\n", path);
                    }
                }
            }
        }
    }

    return;
}