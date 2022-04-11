## How to run shell:

a) The makefile will compile the code.

b) Run 'make' and it will create an executable for us.

c) Run './shell' to run the shell.

## Division of code:

a) The entire shell is divided into several modular files.

b) header.h contains all the function declerations and global variables along with all the essential header files.

c) Then there are 12 other files including main.c and all the other files for the individual commands and functionalities. 
Features:

## Assumptions:

a) History will only be shown for at max 20 files.

b) For the ls command , if -l is given as a flag then if the file was modified before 6 months (i.e some day which is 6 months before the current date) then year would be shown else time.

c) The commands like ls,cd,pwd,echo,pinfo cannot be used as background commands.

d) The shell's home will be the directory from where it was excited.

e) The file for storing history will be created in the same directory from where shell was invoked.

f) In redirection , for input-output at max only one file will be given

g) For < redirection , the last word would be cosidered as file before EOF or another redirection operator.
 h) For > and >> , the immidiate next word would be considered for redirection.

## Files:

a) main.c -> Contains all initializations,signal handling,input reading,function calling

b) prompt.c -> Used for printing prompt

c) background.c -> Handles the execution of background processes.

d) cd.c -> Handles the execution of cd command . Can handle normal cd usage along with .,..,-,~.

e) pwd.c -> Does the normal functioning of pwd

f) echo.c -> Does the Does the normal functioning of echo but does not treat {",'} specially.

g) ls.c -> Does normal functioning along with the -l and -a flags and it can also handle multiple directories and files.

h) foreground.c -> Handles the execution of foreground processes.

i) job_handling.c -> Handles the storing of background commands.

j) pinfo.c -> Prints info of the shell process else of the process id given as argument . A '+' is added if process is foreground.

k) signal_history.c -> For printing status when background processes finished.

l) history.c -> For printing history of previous commands . Can also handle the commands which were run in prevoius sessions.

## Signal handling:

a) Ctrl + Z moves the foreground process to the background and stops it (SIGTSTP).

b) Ctrl + C interrupts the foreground process (SIGINT).

c) Ctrl + D exits the shell.

Directory structure:
```
Shell_directory
|_________main.c
|_________background.c
|_________cd.c
|_________echo.c
|_________foreground.c
|_________history.c
|_________job_handling.c
|_________ls.c
|_________pinfo.c
|_________prompt.c
|_________pwd.c
|_________signal_handler.c
|_________baywatch.c
|_________bg.c
|_________fg.c
|_________jobs.c
|_________run.c
|_________sig.c
```
