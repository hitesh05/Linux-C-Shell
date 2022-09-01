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
typedef char *ptr;
#define MAX_SIZE 100000

// const int num_commands = 5;

char username[MAX_SIZE];
char system_name[MAX_SIZE];
char cwd[2 * MAX_SIZE];
char pseudo_home[1024];
char prev_dir[1024];
ptr token[MAX_SIZE];
char foreground_text[1024];
ll job_count;
char hisfile_path[1024]; // path to history.txt file

char all_commands[7][30];

typedef struct jobs
{
    char name[10000];
    pid_t pid;
} job;

struct jobs curr_foreground_job;
struct jobs job_arr[100000];

struct his
{
    char name[20][1024];
    int num;
};

struct his historyy;

void main_loop(void);
ptr execcommand(ptr command);

// colours
void yellow();
void blue();
void reset();

// commands
void print_prompt();
void pwd();
void cd(ptr token[], ll ind);
void echo(ptr token[], ll ind);
void ls(char dir[], ll ind, ptr token[]);
void pinfo(ptr token[], ll ind);
void foreground(ptr token[]);
void background(ptr token[]);

void init_history();
void write_history();
void add_history(ptr token);
void history(ptr token[], ll ind);

void discover(char dir[], ptr token[], ll ind);