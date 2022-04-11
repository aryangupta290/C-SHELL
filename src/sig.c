#include "headers.h"
void execute_sig(char *from_print[], int num_lit)
{
    if (num_lit != 3)
    {
        printf("Error : sig expects 3 arguments ");
        return;
    }
    int job_no = atoi(from_print[1]);
    int signal = atoi(from_print[2]);
    for (int i = 0; i < bg_process_count; i++)
    {
        if (job_no == bg_process_info[i].index_no)
        {
            if (kill(bg_process_info[i].process_id, signal) != 0)
            {
                printf("Error : sig could not be sent");
            }
            return;
        }
    }
    printf("Error : No such job exists");
    return;
}