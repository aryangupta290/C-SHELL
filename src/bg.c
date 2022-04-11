#include "headers.h"
void execute_bg(char *from_print[], int num_lit)
{
    if (num_lit != 2)
    {
        printf("Error : fg requires only 2 arguments");
        return;
    }
    int job_no = atoi(from_print[1]);
    for (int i = 0; i < bg_process_count; i++)
    {
        if (bg_process_info[i].index_no == job_no)
        {
            kill(bg_process_info[i].process_id, SIGCONT);
            return;
        }
    }
    printf("Error : Process with job_no %d does not exist", job_no);
    return;
}