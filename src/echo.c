#include "headers.h"
void execute_echo(char *from_print[], int num_lit)
{
    char to_print[10001];
    to_print[0] = '\0';
    int start = 0;
    for (int i = 1; i < num_lit; i++)
    {
        for (int j = 0; j < strlen(from_print[i]); j++)
        {
            to_print[start++] = from_print[i][j];
        }
        to_print[start++] = ' ';
    }
    to_print[start - 1] = '\0';
    printf("%s\n", to_print);

    return;
}