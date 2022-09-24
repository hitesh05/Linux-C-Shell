#include "headers.h"

int compare(const void *p1, const void *p2)
{
    const struct jobs *elem1 = p1;
    const struct jobs *elem2 = p2;
    if (strcmp(elem1->name, elem2->name) == 0)
    {
        return 0;
    }
    else
    {
        int x = strlen(elem1->name), retval = -1;
        if (strlen(elem2->name) < x)
        {
            x = strlen(elem2->name);
            retval = 1;
        }
        for (int i = 0; i < x; i++)
        {
            if (elem1->name[i] > elem2->name[i])
            {
                return 1;
            }
            else if (elem1->name[i] < elem2->name[i])
            {
                return -1;
            }
        }
        return retval;
    }
}

char *get_status(pid_t pid)
{
    char proc_stat_file[1024];
    sprintf(proc_stat_file, "/proc/%d/status", pid);
    FILE *status_file = fopen(proc_stat_file, "r");
    if (!status_file)
    {
        red();
        printf("jobs: error opening status file");
        reset();
        return NULL;
    }
    int x = 0;
    char buffer[500];
    char delim[2] = ":";

    char *ret = NULL;
    while (fgets(buffer, sizeof(buffer), status_file))
    {
        if (x == 2)
        {
            // printf("%s", buffer);
            char *str = strtok(buffer, delim);
            str = strtok(NULL, " \t");
            if (str)
            {
                ret = (char *)malloc(sizeof(char) * (strlen(str) + 1));
                strcpy(ret, str);
                fclose(status_file);
                return ret;
            }
            else
            {
                fclose(status_file);
                return NULL;
            }
        }
        x++;
    }
    fclose(status_file);
    return NULL;
}

void jobs(ptr token[], ll ind)
{
    if (job_count == 0)
    {
        printf("No jobs in background\n");
        return;
    }

    int is_r = 0;
    int is_s = 0;

    for (ll i = 1; i < ind && token[i] != NULL; i++)
    {
        if (!strcmp(token[i], "-r"))
        {
            is_r = 1;
        }
        else if (!strcmp(token[i], "-s"))
        {
            is_s = 1;
        }
        else if (!strcmp(token[i], "-rs") || !strcmp(token[i], "-sr"))
        {
            is_r = 1;
            is_s = 1;
        }
        else
        {
            printf("Invalid flag\n Only -r and -s flags allowed.\n");
            return;
        }
    }

    int is_both = 0;
    if ((is_r == 1 && is_s == 1) || (is_r == 0 && is_s == 0))
    {
        is_both = 1;
    }
    struct jobs whyisthis;
    qsort(job_arr, job_count, sizeof(whyisthis), compare);
    for (ll i = 0; i < job_count; i++)
    {
        char *status = get_status(job_arr[i].pid);
        if (!status)
        {
            printf("unable to get status\n");
            continue;
        }

        if (is_both)
        {
            printf("[%lld] ", job_arr[i].num);

            if (!strcmp(status, "R") || !strcmp(status, "S"))
            {
                printf("Running ");
            }
            else if (!strcmp(status, "T"))
            {
                printf("Stopped ");
            }

            printf("%s ", job_arr[i].name);
            printf("[%d]\n", job_arr[i].pid);
        }
        else if (is_r)
        {
            if (!strcmp(status, "R") || !strcmp(status, "S"))
            {
                printf("[%lld] ", job_arr[i].num);

                printf("Running ");

                printf("%s ", job_arr[i].name);
                printf("[%d]\n", job_arr[i].pid);
            }
        }
        else if (is_s)
        {
            if (!strcmp(status, "T"))
            {
                printf("[%lld] ", job_arr[i].num);

                printf("Stopped ");

                printf("%s ", job_arr[i].name);
                printf("[%d]\n", job_arr[i].pid);
            }
        }
    }
}