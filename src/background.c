#include "headers.h"
void execute_background(char *from_print[])
{
    pid_t child_process = fork();
    setpgid(0, 0);
    if (child_process < 0)
    {
        perror("Error:Fork Failed");
    }
    else if (child_process == 0) 
    {

        int exec = execvp(from_print[0], from_print);
        if (exec < 0)
        {
            printf("Error : %s command was not found", from_print[0]);
        }
        pid_t child_p = getpid();
        exit(0);
    }
    else
    {
        printf("pid : %d\n", child_process);
        add_bg_process(child_process, from_print[0]);
    }
    return;
}