#include "headers.h"
void execute_jobs(char *from_print[], int num_lit)
{
    if (num_lit > 2)
    {
        printf("Error : More arguments given than allowed in jobs command");
        return;
    }
    int flag_r = 1; // Allow running process
    int flag_s = 1; // Allow stoopped process
    if (num_lit == 2 && strcmp(from_print[1], "-r") == 0)
    {
        flag_s = 0;
    }
    else if (num_lit == 2 && strcmp(from_print[1], "-s") == 0)
    {
        flag_r = 0;
    }
    else if (num_lit == 2 && strcmp(from_print[1], "-rs") == 0)
    {
        flag_r = 1;
        flag_s = 1;
    }
    else if (num_lit == 2 && strcmp(from_print[1], "-sr") == 0)
    {
        flag_r = 1;
        flag_s = 1;
    }
    else if (num_lit == 2)
    {
        printf("Error : Incorrect flag provided with jobs command");
        return;
    }
    for (int i = 0; i < bg_process_count; i++)
    {
        char file_name[100];
        int process_id = bg_process_info[i].process_id;
        sprintf(file_name, "/proc/%d/stat", process_id);
        FILE *f = fopen(file_name, "r");
        if (!f)
        {
            printf("Error : File could not be opened");
            return;
        }
        char status;
        fscanf(f, "%*d %*s %c", &status);
        fclose(f);
        if (status == 'T' && flag_s == 0)
        {
            continue;
        }
        if ((status == 'R' || status == 'S') && flag_r == 0)
        {
            continue;
        }

        printf("[%d] ", bg_process_info[i].index_no);
        if (status == 'T')
        {
            printf("Stopped %s [%d]\n", bg_process_info[i].command_name, bg_process_info[i].process_id);
        }
        else if (status == 'R' || status == 'S')
        {
            printf("Running %s [%d]\n", bg_process_info[i].command_name, bg_process_info[i].process_id);
        }
    }

    return;
}