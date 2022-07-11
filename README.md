## SETUP:

```
$ make
$ ./shell
```

## Features



* `cd`: Changes the working directory of the shell.
```bash
cd
cd - # To go to the previous working directory
cd ~
cd ../..
cd dir_1/dir_2/dir_3
```

* `echo`: Prints the text following "echo" on the terminal.
```bash
echo "Hello World"
echo Welcome to ASH
```

* `ls`: Lists the contents of a particular directory.
```bash
ls
ls -a
ls -l
ls -al
ls -la
ls -a -l
ls -l -a
ls ../../dir_1/dir_2
ls ~
ls dir_1/file_name
```

* `pinfo`: Displays information about a particular process.
```bash
pinfo # Displays information about the shell process itself
pinfo <pid>
```

* `fg`: Brings the running/stopped background job to the foreground.
```bash
fg <job_number>
```

* `bg`: Changes the state of a stopped background job to running.
```bash
bg <job_number>
```


* `pwd`: Prints the absolute path of the current working directory.
```bash
pwd
```
* `jobs`: Prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with
the job number, process ID and the state.
```bash
jobs
jobs -s
jobs -r
jobs -rs
jobs -s -r
```

* `history`: Displays a list of the commands previously used (at most the 20 latest commands).
```bash
history
history <n> # To display last n commands used
```

* `baywatch`: A modified version of the bash command watch, that works for three specific commands: <br>

  * <b>interrupt:</b> Prints the number of times the CPUs have been interrupted by the keyboard controller (i8042 with IRQ 1)

  * <b>newborn:</b> Prints the PID of the most recently created process.

  * <b>dirty:</b> Prints the size of the part of memory that is dirty.
```bash
baywatch -n <interval> interrupt
baywatch -n <interval> newborn
baywatch -n <interval> dirty
```

Press <kbd>Q</kbd> to terminate the command.

* `repeat`: Executes a given command n times.
```bash
repeat <n> <command>
```

* `replay`: Executes a particular command in fixed time interval for a certain period.
```bash
replay -command <command> -interval <interval> -period <period>
```

* `sig`: Sends the signal corresponding to signal number​ to the process with the particular job number.
```
sig <job_number> <signal_number>
```

* <b>System Commands: </b>ASH should run many of the processes that bash can, including `gedit`, `vim`, `clear`, etc.
```bash
gedit
vim
clear
ps
```

* <b>Background Processes: </b>Add `&` at the end of the command to run it as a background process. This however only works for system commands, and not shell built-ins.
```bash
gedit &
```

* <b>Arrow Keys: </b>Acts as a shortcut to easily view previous commands, and execute them, just like in bash.<br>
<kbd>UP</kbd> - View earlier commands<br>
<kbd>DOWN</kbd> - View more recent commands


## Assumptions:

* History will only be shown for at max 20 files.  

* For the ls command , if -l is given as a flag then if the file was modified before 6 months (i.e some day which is 6 months before the current date) then year would be shown else time.  

* The commands like ls,cd,pwd,echo,pinfo cannot be used as background commands.  

* The shell's home will be the directory from where it was excited.  

* The file for storing history will be created in the same directory from where shell was invoked.  

* In redirection , for input-output at max only one file will be given.  

* For < redirection , the last word would be cosidered as file before EOF or another redirection operator.  
* For > and >> , the immidiate next word would be considered for redirection.  

## File Strcuture

* main.c -> Contains all initializations,signal handling,input reading,function calling

* prompt.c -> Used for printing prompt

* background.c -> Handles the execution of background processes.

* cd.c -> Handles the execution of cd command . Can handle normal cd usage along with .,..,-,~.

* pwd.c -> Does the normal functioning of pwd

* echo.c -> Does the Does the normal functioning of echo but does not treat {",'} specially.

* ls.c -> Does normal functioning along with the -l and -a flags and it can also handle multiple directories and files.

* foreground.c -> Handles the execution of foreground processes.

* job_handling.c -> Handles the storing of background commands.

* pinfo.c -> Prints info of the shell process else of the process id given as argument . A '+' is added if process is foreground.

* signal_history.c -> For printing status when background processes finished.

* history.c -> For printing history of previous commands . Can also handle the commands which were run in prevoius sessions.

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
