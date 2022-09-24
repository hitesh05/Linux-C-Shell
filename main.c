#include "headers.h"

pid_t curr_id;

void yellow()
{
    printf("\x1b[1;33m");
}
void blue()
{
    printf("\x1b[1;34m");
}
void red()
{
    printf("\x1b[1;31m");
}
void cyan()
{
    printf("\x1b[1;36m");
}
void green()
{
    printf("\x1b[1;32m");
}
void reset()
{
    printf("\x1b[0m");
}

void del_process(int id)
{
    // printf("Entered killproc with job count = %lld and proc number=%d\n", job_count, id);
    if (id == -1)
        job_count = 0;
    int flag = 0;
    for (ll i = 0; i < job_count; i++)
    {
        if (job_arr[i].pid == id)
        {
            flag = 1;
            for (ll j = i; j < job_count; j++)
                job_arr[j] = job_arr[j + 1];
            job_count--;
        }
    }

    id = -id;
    if (flag == 0 && id != 1)
    {
        red();
        printf("Error: no such process found\n");
        reset();
    }

    return;
}

void done()
{
    pid_t p;
    int status;
    p = waitpid(-1, &status, WNOHANG);
    for (ll z = 0; z < job_count; z++)
    {
        if (p < 0)
        {
            red();
            printf("\nwaitpid failed\n");
            reset();
        }
        const int exit = WEXITSTATUS(status);

        // printf("WIFEXITED (STATUS) OF KJOB = %d\nWEXITSTATUS %d  p= %d  job_arr[z] %d\n", WIFEXITED(status), WEXITSTATUS(status),p,job_arr[z].pid);

        if (((WIFEXITED(status) && p == job_arr[z].pid)))
        {
            if (exit == 0)
            {
                cyan();
                printf("\nExitted normally with exit status: %d\n", exit);
                reset();
            }
            else
            {
                red();
                printf("\nExitted abnormally\n");
                reset();
            }

            green();
            printf("%s with pid %d: exited\n", job_arr[z].name, p);
            reset();
            print_prompt();
            fflush(stdout);
            del_process(p);
        }
    }
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void main_loop(void)
{
    int status = 1;
    char *command = malloc(sizeof(char) * 1000);
    ssize_t command_size = 0;

    foreground_text[0] = '\0';

    while (status)
    {
        curr_foreground_job.pid = -1;

        signal(SIGCHLD, done);
        signal(SIGTSTP, control_z);
        signal(SIGINT, control_c);
        // add ctrl+d

        setbuf(stdout, NULL);
        enableRawMode();
        memset(command, '\0', 1000);

        print_prompt();

        char c;
        int pt = 0;
        int is_tab = 0;

        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == EOF || c == 4)
                {
                    exit(0);
                }
                else if (c == 10)
                {
                    // END OF LINE
                    printf("%c", c);
                    command[pt++] = c;
                    break;
                }
                else if (c == 9)
                {
                    // TAB
                    char *cmd = malloc(sizeof(char) * MAX_SIZE);
                    memset(cmd, '\0', MAX_SIZE);
                    int autocomp = autocomplete(command, cmd);
                    strcpy(command, cmd);
                    pt = strlen(command);
                    // printf("Command is:%d\n",pt);
                    is_tab = 1;
                }
                else if (c == 127)
                {
                    // BACKSPACE
                    if (pt > 0)
                    {
                        if (command[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        command[--pt] = '\0';
                        printf("\b \b");
                    }
                }
            }
            else
            {
                command[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        command[pt] = '\0';

        // getline(&command, &command_size, stdin);

        execcommand(command);
    }
}

int main(int argc, ptr argv[])
{
    SHELL_PID = getpid();
    curr_id = getpid();
    // printf("%d\n", curr_id);

    getcwd(cwd, sizeof(cwd));
    // char curr_dir[MAX_SIZE];
    getcwd(pseudo_home, sizeof(pseudo_home));
    getcwd(prev_dir, sizeof(prev_dir));
    // printf("%s\n", curr_dir);
    chdir(cwd);

    job_count = 0;

    init_history();

    main_loop();

    return 0;
}