# Linux-C-Shell
C-Shell built as part of Operating Systems and Networks course at IIIT

This is a basic replica of bash written in C with all the built-ins and some other commands implemented from scratch.

**To run the shell:**
```
make 
./shell
 ```

Features implemented 
1) A shell prompt of the form: <username@hostname:curr_dir> with path relative to home 
2) home directory is the directory from which the shell is invoked.
2) Semi-colon (;) separated commands 
3) & after any command  to run a process in background
4) Some built in commands like cd ,echo ,ls , and pwd re-implemented 
5) Type quit or exit to  exit the shell 
6) Implements foreground and background processes 
7) Process name and id printed when background process terminates 
8) Type pinfo in the prompt to print: pid, process status, memory and executable path (on Linux machines) 
9) As opposed to bash , also prints asynchronously the process name , id and exit status whenever a background process is terminated is terminated to stderr
10) Can take multiple semi colon separated commands as input and execute them sequentially 
11) apart from user defined commands , rest all can be executed in foreground or background 
12) history: if given with a argument number n , prints the previous n commands that the shell has executed . default value of n is teken to be 10 . preserves history accross shell sessions. a command beginning with " " is not added to the history (just like bash). Maximum 20 commands stored.
13) Pipelining and redirection: same as bash
14) jobs: This command prints a list of all currently running background processes spawned by the shell in
alphabetical order of the command name, along with their job number (a sequential number assigned by my
shell), process ID, and their state, which can either be running or stopped. There may be flags specified as well.
If the flag specified is -r, then we print only the running processes else if the flag is -s then the stopped
processes only are printed.
15) sig: Takes the job number (assigned by my shell) of a running job and sends the signal corresponding to the
signal number to that process.
16) fg: Brings the running or stopped background job corresponding to job number to the foreground, and changes
its state to running.
17) bg: Changes the state of a stopped background job to running (in the background).
18) CTRL+Z: pushes any currently running foreground job into the background, and change its state from
‘running’ to ‘stopped.’
19) CTRL-C: Interrupts any currently running foreground job, by sending it the SIGINT signal.
20) CTRL-D: Logs you out of the shell.
21) Autocompletion: The `tab` key can be pressed at any time of your prompt. When you press the tab key, it will print all files which have the same starting letters as the ls command and then again show the same prompt. If there is only one
file/folder which has the same starting characters, the prompt will be filled with that. If it's a file, a space will
be placed after the file. If it’s a folder, a ‘/’ will be put after the folder name.


Files in Directory:-

1. main.c :- contains the main()  , main_loop(),done(), del_process() and color functions.

2. prompt.c :- contains the print_prompt() function which is used to print the prompt mimicing the bash shell.

3. handlecmds.c :- contains -\
    splitcmd () - to tokenise the commands using ";". separate multiple commands \
    execute() - to call the self-implemented built in commands or pass others to execvp \
    checkand()- check if a command contains & , to run it separately in the background \
    execcommand()- to call all the above mentioned funtions 


4. ls.c  :- contains ls() ,ls_l() ,print_l() function. It handles ls command .\
**FEATURES** \
ls -
works with multiple directories given as input 
displays the executables in green and directries in blue 
works with -l , -a , -al/-la flags (in any order ) 
also displays total blocks of 1024 bytes occupied by any directory

5. echo.c :- contains echo function . It handles echo command.\

6. cd.c :- contains cd function . It handles cd command. \
**cd FEATURES**- \
works with cd - to jump to any previous directory 
use cd or cd ~ to jump to home 

7. headerfile.h :- contains all the definitions of functions and the definitions of the constants to be used.

8. pinfo.c :- contains pinfo function. It prints the process related info of our shell program. \
**FEATURE pinfo**  \
prints the process related information corresponding to the pid given with the command or to the  
the shell if no pid given

9. pwd.c   :- contains pwd function . It handles pwd command.

10. history.c :- contains the add_history(),init_history(),write_history() and history() functions , handles the history command.

11. foreground.c :- contains foreground() which runs the command in foreground. The time taken to execute the process is printed alongside the prompt if it is greater that 1s. \
*Assumption:* time is printed for latest process executed, not all.

12. background.c :- contains background() which runs any process in the background. Prints the status of the process on exiting.

13. discover.c :- contains discover_d() and discover(). works similar to the find command in bash. \
-d flag searches for all directories recursively \
-f flag searches for all files recursively \
Directory path name may also be provided in the argument. \
*Assumption:* Does not search for hidden files.

14. autocomplete.c :- conatains autocomplete() and ls2() (supporter for autocomplete) functions. \
Works similar to autocomplete command in bash on pressing `tab`
*Assumption:* If `tab` is pressed on NULL input, ls command is executed.

15. bg.c :- contains bg() and bg_exec() functions. \
To execute the bg command.

16. fg.c :- contains fg() and fg_exec() functions. \
To execute the bg command.

17. jobs.c :- main function is jobs(). \
To execute the jobs command and print all currently running processes.

18. redirection.c : Handles all the pipelining and redirection. \
