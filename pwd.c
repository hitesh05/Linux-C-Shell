#include "headers.h"

void pwd(){
    char cwd2[MAX_SIZE];
    getcwd(cwd2, sizeof(cwd2));
    printf("%s\n", cwd2);
}