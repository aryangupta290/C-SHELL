#include "headers.h"
void execute_fg(char *from_print[], int num_lit)
{
    if (num_lit != 2)
    {
        printf("Error : fg requires only 2 arguments");
        return;
    }
    int job_no = atoi(from_print[1]);
    for (int i = 0; i < bg_process_count; i++)
    {
        if (job_no == bg_process_info[i].index_no)
        {
            signal(SIGINT, handler_ctrl_c);
            signal(SIGTSTP, handler_ctrl_z);
            int stats;
            int pid = bg_process_info[i].process_id;
            signal(SIGTTIN, SIG_IGN);                               // stops process if trying to read something from terminal
            signal(SIGTTOU, SIG_IGN);                               // stodps when process is trying to writing something to terminal
            tcsetpgrp(STDIN_FILENO, bg_process_info[i].process_id); // makes process with pgrp the foreground process group
            fg_process_info.process_id = bg_process_info[i].process_id;
            printf("%d\n", fg_process_info.process_id);
            strcpy(fg_process_info.command_name, bg_process_info[i].command_name);
            kill(bg_process_info[i].process_id, SIGCONT);
            remove_bg_process(pid);
            waitpid(pid, &stats, WUNTRACED); // waits for the process to finish execution
            if (WIFSTOPPED(stats))
            {
                printf("%s with PID %d is suspened", fg_process_info.command_name, fg_process_info.process_id);
                add_bg_process(fg_process_info.process_id, fg_process_info.command_name);
            }
            tcsetpgrp(0, getpgrp());
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            fg_process_info.process_id = -1;

            return;
        }
    }
    printf("Error : Process with job_no %d does not exist", job_no);
    return;
}