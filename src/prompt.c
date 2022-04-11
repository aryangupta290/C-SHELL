#include "headers.h"
bool check_prefix()
{
    if (strlen(cur_dir) < strlen(shell_dir))
    {
        return false;
    }
    for (int i = 0; i < strlen(shell_dir); i++)
    {
        if (cur_dir[i] != shell_dir[i])
        {
            return false;
        }
    }
    return true;
}
void p_prompt()
{
    getcwd(cur_dir, sizeof(cur_dir));
    bool is = check_prefix();
    char to_print[10001]; // contains what would be printed
    if (is)
    {
        // means that we are inside the directory from where shell was invoked
        to_print[0] = '~';
        int start = 1;
        for (int i = strlen(shell_dir); i < strlen(cur_dir); i++)
        {
            to_print[start++] = cur_dir[i];
        }
        to_print[start] = '\0';
    }
    else
    {
        strcpy(to_print, cur_dir);
    }
    printf("\n<");
    printf("%s", user_name);
    printf("@");
    printf("%s", system_name);
    printf(":");
    printf("%s", to_print);
    printf("> ");
}