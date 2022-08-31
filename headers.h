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

// const int num_commands = 5;

char username[MAX_SIZE];
char system_name[MAX_SIZE];
char cwd[2*MAX_SIZE];
char pseudo_home[1024];
char prev_dir[1024];
ptr token[MAX_SIZE];
char foreground_text[1024];

char all_commands[5][30];


typedef struct jobs{
    char name[MAX_SIZE];
    pid_t pid;
} job;

struct jobs curr_foreground_job;

void main_loop(void);
ptr execcommand(ptr command);

// colours
void yellow();
void blue();
void reset();

// commands
void pwd();
void cd(ptr token[], ll ind);
void echo(ptr token[], ll ind);
void ls(char dir[], ll ind, ptr token[]);
void pinfo(ptr token[], ll ind);
void foreground(ptr token[]);
// void background(char *token[]);
