#include "headers.h"

void echo(ptr token[], ll ind)
{
    char str[MAX_SIZE] = "";
    char space[2] = " ";
    for (ll i = 1; i <= ind - 1; i++)
    {
        strcat(str, token[i]);
        strcat(str, space);
    }

    printf("%s\n", str);
    return;
}