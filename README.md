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


Files in Directory:-

1) main.c :- contains the main()  , main_loop(),done(), del_process() and color functions.

2) prompt.c :- contains the print_prompt() function which is used to print the prompt mimicing the bash shell.

3) handlecmds.c :- contains -\
    splitcmd () - to tokenise the commands using ";". separate multiple commands \
    execute() - to call the self-implemented built in commands or pass others to execvp \
    checkand()- check if a command contains & , to run it separately in the background \
    execcommand()- to call all the above mentioned funtions 


4) ls.c  :- contains ls() ,ls_l() ,print_l() function. It handles ls command .\
**FEATURES** \
ls -
works with multiple directories given as input 
displays the executables in green and directries in blue 
works with -l , -a , -al/-la flags (in any order ) 
also displays total blocks of 1024 bytes occupied by any directory

5) echo.c :- contains echo function . It handles echo command.\

6) cd.c :- contains cd function . It handles cd command. \
**cd FEATURES**- \
works with cd - to jump to any previous directory 
use cd or cd ~ to jump to home 

7) headerfile.h :- contains all the definitions of functions and the definitions of the constants to be used.

8) pinfo.c :- contains pinfo function. It prints the process related info of our shell program. \
**FEATURE pinfo**  \
prints the process related information corresponding to the pid given with the command or to the  
the shell if no pid given

9) pwd.c   :- contains pwd function . It handles pwd command.

10) history.c :- contains the add_history(),init_history(),write_history() and history() functions , handles the history command.

11) foreground.c :- contains foreground() which runs the command in foreground. The time taken to execute the process is printed alongside the prompt if it is greater that 1s. \
*Assumption:* time is printed for latest process executed, not all.

12) background.c :- contains background() which runs any process in the background. Prints the status of the process on exiting.

13) discover.c :- contains discover_d() and discover(). works simialar to the find command in bash. \
-d flag searches for all directories recursively \
-f flag searches for all files recursively \
Directory path name may also be provided in the argument. \
*Assumption:* Does not search for hidden files.
