#include "headers.h"
bool check_if_add(int ppid, int pgrp)
{
    if (ppid == pgrp)
    {
        return true;
    }
    return false;
}
void execute_pinfo(char *from_print[], int k)
{
    if (k > 2)
    {
        perror("Error : More arguments than expected");
        return;
    }
    int pid = getpid();
    if (k > 1)
    {
        pid = atoi(from_print[1]);
    }
    char file_info[10001];
    sprintf(file_info, "/proc/%d/stat", pid);
    FILE *fo = fopen(file_info, "r");
    if (fo == 0)
    {
        printf("Error : Process does not exist");
        return;
    }
    char status = ' ';
    int ppid, pgrp;
    long long int memory;
    char temp[1001];
    int start = 0;
    while (fscanf(fo, "%s", temp) == 1)
    {
        start++;
        if (start == 3)
        {
            status = temp[0];
        }
        else if (start == 5)
        {
            ppid = atoi(temp);
        }
        else if (start == 8)
        {
            pgrp = atoi(temp);
        }
        else if (start == 23)
        {
            memory = atoi(temp);
        }
    }
    printf("pid -- %d\n", pid);
    printf("Process Status -- %c", status);
    if (check_if_add(ppid, pgrp))
    {
        printf("+\n");
    }
    else
    {
        printf("\n");
    }
    printf("memory -- %lld{Virtual Memory}\n", memory);
    char executable_path[10001];
    sprintf(file_info, "/proc/%d/exe", pid);
    int sz = readlink(file_info, executable_path, 10001);
    executable_path[sz] = '\0';
    bool is = true;
    printf("Executable Path -- ");
    if (strlen(executable_path) < strlen(shell_dir))
    {
        is = false;
    }
    for (int i = 0; is && i < strlen(shell_dir); i++)
    {
        if (executable_path[i] != shell_dir[i])
        {
            is = false;
            break;
        }
    }
    if (is)
    {
        printf("~%s\n", executable_path + strlen(shell_dir));
    }
    else
    {
        printf("%s\n", executable_path);
    }
    return;
}