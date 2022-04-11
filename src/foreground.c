#include "headers.h"
void execute_foreground(char *from_print[])
{
    pid_t child_process = fork();
    fg_process_info.process_id = child_process;
    strcpy(fg_process_info.command_name, from_print[0]);
    if (child_process < 0)
    {
        perror("Error : Fork Failed");
    }
    else if (child_process == 0)
    {
        int exec = execvp(from_print[0], from_print);
        if (exec < 0)
        {
            printf("Error : %s incorrect", from_print[0]);
        }
        exit(0);
    }
    else
    {
        int stats = 0;
        waitpid(child_process, &stats, WUNTRACED);
    }
    return;
}