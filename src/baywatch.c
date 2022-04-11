#include "headers.h"
void print_line()
{
    int f = open("/proc/interrupts", O_RDONLY);
    char temp[100001];
    read(f, temp, 100001);
    char *check = strtok(temp, "\n");
    char *to = check;
    while (isspace((unsigned char)*to))
    {
        to++;
    }
    printf("%s\n", to);
}
void execute_interrupt()
{
    FILE *f = fopen("/proc/interrupts", "r");
    char line[140];
    for (int i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), f);
    }
    int start = 0;
    for (int i = 12; i < strlen(line); i++)
    {
        if (line[i] != ' ' && line[i] > 57)
        {
            start = i;
        }
    }
    line[start] = '\0';
    printf("%s\n", line + 12);
    return;
}
void execute_newborn()
{
    int f = open("/proc/loadavg", O_RDONLY);
    char temp[100001];
    read(f, temp, 100001);
    temp[10000] = '\0';
    char *check = strtok(temp, " ");
    char ans[1000];
    while (check != NULL)
    {
        strcpy(ans, check);
        check = strtok(NULL, " ");
    }
    ans[100] = '\0';
    printf("%s", ans);
    fflush(stdout);
    return;
}
void execute_dirty()
{
    int f = open("/proc/meminfo", O_RDONLY);
    char temp[100001];
    read(f, temp, 100001);
    char *check = strtok(temp, "\n");
    for (int i = 0; i < 16; i++)
    {
        check = strtok(NULL, "\n");
    }
    char *to = check + 6;
    while (isspace((unsigned char)*to))
    {
        to++;
    }
    printf("%s\n", to);
    fflush(stdout);
}
bool was_keyboard_pressed()
{
    struct termios old, new;
    tcgetattr(0, &old); // storing attribute of terminal so that we can restore later
    new = old;
    new.c_lflag &= ~(ICANON | ECHO); // ICANON -> reads each charachter as it comes( don't need new line)
    tcsetattr(0, TCSANOW, &new);
    int check;
    ioctl(0, FIONREAD, &check); // ioctl checks number of bytes read
    tcsetattr(0, TCSANOW, &old);
    if (check > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void function_call(int arg)
{
    if (arg == 0)
    {
        execute_interrupt();
    }
    else if (arg == 1)
    {
        execute_newborn();
    }
    else
    {
        execute_dirty();
    }
}
void execute_baywatch(char *from_print[], int num_lit)
{
    if (num_lit != 4)
    {
        printf("Error : Baywatch expects 4 arguments");
        return;
    }
    if (strcmp(from_print[1], "-n") != 0)
    {
        printf("Error : Incorrect syntax for baywatch");
        return;
    }
    int interval = atoi(from_print[2]);
    if (interval <= 0)
    {
        printf("Error : Baywatch expects a positive integer for interval period");
        return;
    }
    int which = 0; // 0 for interrupt , 1 for newborn , 2 for dirty
    if (strcmp(from_print[3], "interrupt") == 0)
    {
        print_line();
        which = 0;
    }
    else if (strcmp(from_print[3], "newborn") == 0)
    {
        which = 1;
    }
    else if (strcmp(from_print[3], "dirty") == 0)
    {
        which = 2;
    }
    else
    {
        printf("Error : Incorrect syntax for baywatch");
        return;
    }

    time_t when = time(NULL);
    int iterations = 0;
    while (true)
    {
        int time_passed = (time(NULL) - when) / interval;
        if (time_passed > iterations)
        {
            function_call(which);
            iterations++;
        }
        if (was_keyboard_pressed())
        {
            char c = getchar();
            if (c == 'q')
            {
                return;
            }
        }
    }
}