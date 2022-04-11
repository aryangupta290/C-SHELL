#include "headers.h"
void sigchld_handler()
{
    pid_t check_pid;
    int status;
    bool did_reap = false;
    while ((check_pid = waitpid(-1, &status, WNOHANG)) > 0) // -1 indicates wait for any child process
    {
        status = WIFEXITED(status);
        int bg_index = check_bg_process(check_pid);
        if (bg_index == -1)
        {
            continue;
        }
        did_reap = true;
        if (status == 0)
        {
            printf("\n%s with pid %d exited abnormally\n", bg_process_info[bg_index].command_name, bg_process_info[bg_index].process_id);
            remove_bg_process(bg_process_info[bg_index].process_id);
        }
        else
        {
            printf("\n%s with pid %d exited normally\n", bg_process_info[bg_index].command_name, bg_process_info[bg_index].process_id);
            remove_bg_process(bg_process_info[bg_index].process_id);
        }
    }
    if (did_reap == true)
    {
        printf("\033[0;34m");
        p_prompt();
        printf("\033[0m");
        fflush(stdout);
    }
    return;
}
void handler_ctrl_c()
{
    pid_t pid = getpid();
    // pid!=shell_id
    if (fg_process_info.process_id == -1)
    {
        return;
    }
    kill(fg_process_info.process_id, SIGINT);
}
void handler_ctrl_z()
{
    pid_t pid = getpid();
    if (fg_process_info.process_id == -1)
    {
        return;
    }
    kill(fg_process_info.process_id, SIGTSTP);
    fg_process_info.process_id == -1;
    add_bg_process(fg_process_info.process_id, fg_process_info.command_name);
    return;
}