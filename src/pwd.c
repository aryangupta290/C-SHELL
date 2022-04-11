#include "headers.h"
void execute_pwd()
{
    char store_pwd[1001];
    getcwd(store_pwd, 1001);
    if (store_pwd != NULL)
    {
        printf("%s", store_pwd);
    }
    else
    {
        perror("Error : Directory Path could not be displayed");
    }
    return;
}