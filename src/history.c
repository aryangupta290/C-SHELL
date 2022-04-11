#include "headers.h"
void init_history()
{
    FILE *fo = fopen(history_file_name, "ab+");
    if (fo == NULL)
    {
        printf("Error open : Could not open file history file\n");
        return;
    }
    while (fgets(history_commands[history_file_count], 1001, fo) != NULL)
    {
        history_file_count++;
    }
    fclose(fo);
}
void add_command(char from_print[])
{
    if (history_file_count > 0 && strcmp(history_commands[history_file_count - 1], from_print) == 0)
    {
        return;
    }
    if (history_file_count == 20)
    {
        for (int i = 0; i < 19; i++)
        {
            strcpy(history_commands[i], history_commands[i + 1]);
        }
        strcpy(history_commands[19], from_print);
    }
    else
    {
        strcpy(history_commands[history_file_count++], from_print);
    }
    FILE *fo = fopen(history_file_name, "w+");
    if (fo == NULL)
    {
        printf("Error open : Could not open file history file");
        return;
    }
    for (int i = 0; i < history_file_count; i++)
    {
        fputs(history_commands[i], fo);
    }
    fclose(fo);
}
void execute_history(char *from_print[], int num)
{
    int n = 10;
    if (num > 2)
    {
        printf("Error : Invalid syntax for history command");
        return;
    }
    if (num == 2)
    {
        bool is = true;
        for (int i = 0; i < strlen(each_segment[1]); i++)
        {
            if (each_segment[1][i] > 57 || each_segment[1][i] < 48)
            {
                is = false;
                break;
            }
        }
        if (is == false)
        {
            printf("Error : Invalid syntax for history command");
            return;
        }
        n = atoi(each_segment[1]);
    }
    int maxi = -1;
    if (maxi < history_file_count - 1 - n)
    {
        maxi = history_file_count - 1 - n;
    }
    for (int i = history_file_count - 1; i > maxi; i--)
    {
        for (int j = 0; j < strlen(history_commands[i]); j++)
            printf("%c", history_commands[i][j]);
    }
    return;
}