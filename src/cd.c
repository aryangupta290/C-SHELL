#include "headers.h"
void print_error_cd()
{
    perror("Error: cd could not be executed");
    return;
}
void execute_cd(char *from_print[], int num_lit)
{
    if (num_lit > 2)
    {
        perror("Error : More Arguments than needed");
        return;
    }
    char present_dir[10001];
    getcwd(present_dir, 10001);
    if (num_lit == 1)
    {
        int check = chdir(shell_dir);
        if (check != 0)
        {
            print_error_cd();
        }
        strcpy(last_dir, present_dir);
        return;
    }
    if (strcmp(from_print[1], "..") == 0)
    {
        int check = chdir("..");
        if (check != 0)
            print_error_cd();
    }
    else if (strcmp(from_print[1], "-") == 0)
    {
        if (strcmp(last_dir, "") == 0)
        {
            printf("Error: cd: OLDPWD not set");
            return;
        }
        int check = chdir(last_dir);
        if (check != 0)
        {
            print_error_cd();
        }
        bool is = true;
        if (strlen(last_dir) < strlen(shell_dir))
        {
            is = false;
        }
        for (int i = 0; is && i < strlen(shell_dir); i++)
        {
            if (last_dir[i] != shell_dir[i])
            {
                is = false;
                break;
            }
        }
        if (is)
        {
            printf("~%s", last_dir + strlen(shell_dir));
        }
        else
        {
            printf("%s", last_dir);
        }
    }
    else if (from_print[1][0] == '~')
    {
        if (strlen(from_print[1]) == 1)
        {
            int check = chdir(shell_dir);
            if (check != 0)
            {
                printf("Error: Invalid path specified");
                return;
            }
        }
        else
        {
            char abs_path[10001];
            strcpy(abs_path, shell_dir);
            int start = strlen(shell_dir);
            for (int i = 1; i < strlen(from_print[1]); i++)
            {
                abs_path[start++] = from_print[1][i];
            }
            abs_path[start] = '\0';
            int check = chdir(abs_path);
            if (check != 0)
            {
                printf("Error : Invalid path specified");
                return;
            }
        }
    }
    else
    {
        int check = chdir(from_print[1]);
        if (check != 0)
        {
            printf("Error : Invalid path specified");
            return;
        }
    }
    strcpy(last_dir, present_dir);
}