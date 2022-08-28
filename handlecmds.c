#include "headers.h"

ptr *splitcmd(ptr command){
    ll x = sizeof(ptr);
    char **cmdarray = malloc(100 * x);
    if(cmdarray == NULL){
        printf("malloc failed in handlecmds\n");
    }

    char *cmd = strtok(command, ";\n");
    ll count = 0;
    for(int pt = 0; cmd != NULL; pt++, count++){
        ll y = sizeof(cmdarray)/x;
        if(pt > y){
            cmdarray = realloc(cmdarray, 100* x);
            if(cmdarray == NULL){
                printf("realloc failed in handlecmds\n");
            }
        }

        cmdarray[pt] = cmd;
        cmd = strtok(NULL, ";\n");
    }
    cmdarray[count] = 0;

    return cmdarray;
}

void execute(char *cmd){
    
}

ptr execcommand(ptr command){
    ptr* cmdarray = splitcmd(command);

    for(int i=0; cmdarray[i] != NULL; i++){
        execute(cmdarray[i]);
    }
}