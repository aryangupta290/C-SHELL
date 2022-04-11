#include "headers.h"
void init_bg_process_info()
{
    for (int i = 0; i < 1001; i++)
    {
        bg_process_info[i].process_id = -1;
        strcpy(bg_process_info[i].command_name, "");
        bg_process_info[i].index_no = -1;
    }
    return;
}
void add_bg_process(int id, char command[])
{
    int start = 0;
    for (int i = 0; i < bg_process_count; i++)
    {
        if (start < bg_process_info[i].index_no)
        {
            start = bg_process_info[i].index_no;
        }
    }
    start++;
    bg_process_info[bg_process_count].process_id = id;
    strcpy(bg_process_info[bg_process_count].command_name, command);
    bg_process_info[bg_process_count].index_no = start;
    for (int i = 0; i < bg_process_count; i++)
    {
        if (strcmp(bg_process_info[i].command_name, bg_process_info[bg_process_count].command_name) > 0)
        {
            for (int j = bg_process_count; j > i; j--)
            {
                struct jobs temp = bg_process_info[j];
                bg_process_info[j] = bg_process_info[j - 1];
                bg_process_info[j - 1] = temp;
            }
        }
    }
    bg_process_count++;

    return;
}
int check_bg_process(int id)
{
    for (int i = 0; i < bg_process_count; i++)
    {
        if (bg_process_info[i].process_id == id)
        {
            return i;
        }
    }
    return -1;
}
void remove_bg_process(int id)
{
    for (int i = 0; i < bg_process_count; i++)
    {
        if (bg_process_info[i].process_id == id)
        {
            for (int j = i + 1; j < bg_process_count; j++)
            {
                bg_process_info[j - 1].process_id = bg_process_info[j].process_id;
                strcpy(bg_process_info[j - 1].command_name, bg_process_info[j].command_name);
                bg_process_info[j - 1].index_no = bg_process_info[j].index_no;
            }
            bg_process_info[bg_process_count - 1].process_id = -1;
            strcpy(bg_process_info[bg_process_count - 1].command_name, "");
            bg_process_info[bg_process_count - 1].index_no = -1;
            bg_process_count--;
            break;
        }
    }
    return;
}