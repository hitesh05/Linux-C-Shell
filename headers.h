#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h> // for pid_t
#include <sys/wait.h>

typedef long long int ll;
typedef char * ptr;
#define MAX_SIZE 100000

char username[MAX_SIZE];
char system_name[MAX_SIZE];
char cwd[2*MAX_SIZE];
char pseudo_home[1024];

typedef struct jobs{
    char name[MAX_SIZE];
    pid_t pid;
} job;

void main_loop(void);
void pwd();
void cd(ptr token[], ll k);
void echo(ptr token[], ll k);
ptr execcommand(ptr command);
