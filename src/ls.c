#include "headers.h"
int hid = 0, list = 0;
int num = 0;
char month[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static int comp(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}
int isDirectoryExists(const char *path)
{
    struct stat stats;
    stat(path, &stats);
    if (S_ISDIR(stats.st_mode))
        return 1;
    return 0;
}
int isFileExists(const char *path)
{
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL)
        return 0;
    fclose(fptr);
    return 1;
}
void get_dir_path(char *from_print[], char present_dir[], int i)
{
    if (from_print[i][0] == '~')
    {
        strcpy(present_dir, shell_dir);
        int start = strlen(shell_dir);
        for (int j = 1; j < strlen(from_print[i]); j++)
        {
            present_dir[start++] = from_print[i][j];
        }
        present_dir[start] = '\0';
    }
    else if (from_print[i][0] == '/' || from_print[i][0] == '.')
    {
        strcpy(present_dir, from_print[i]);
    }
    else
    {
        getcwd(present_dir, 10001);
        int len = strlen(present_dir);
        if (present_dir[len - 1] != '/')
        {
            present_dir[len] = '/';
            len++;
        }
        for (int j = 0; j < strlen(from_print[i]); j++)
        {
            present_dir[len + j] = from_print[i][j];
        }
    }
}
int get_block_size(char *present_dir)
{
    int size = 0;
    DIR *d;
    struct dirent *dir;
    struct stat fileStat;
    d = opendir(present_dir);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_name[0] == '.' && hid == 0)
            {
                continue;
            }
            char info_path[PATH_MAX + 1];
            strcpy(info_path, present_dir);
            if (present_dir[strlen(present_dir) - 1] != '/')
                strcat(info_path, "/");
            strcat(info_path, dir->d_name);

            stat(info_path, &fileStat);

            size += fileStat.st_blocks;
        }
    }

    return size / 2;
}
void print_content(char *present_dir, char from_print[], bool is_file, char file_name[])
{
    if (num > 1 && isDirectoryExists(present_dir) == 1)
    {
        if (from_print[0] == '~')
        {
            printf("%s", shell_dir);
            for (int j = 1; j < strlen(from_print); j++)
            {
                printf("%c", from_print[j]);
            }
            printf(":\n");
        }
        else
            printf("%s:\n", from_print);
    }
    struct dirent *read_dir;
    DIR *dir = opendir(present_dir);
    if (dir != NULL)
    {
        if (list && !is_file)
        {
            printf("total %d\n", get_block_size(present_dir));
        }
        char *input_dir[1001];
        int start = 0;
        while ((read_dir = readdir(dir)) != NULL)
        {
            input_dir[start] = (char *)malloc(1001);
            strcpy(input_dir[start], read_dir->d_name);
            start++;
        }
        qsort(input_dir, start, sizeof(const char *), comp);
        int from = 0;
        while (from != start)
        {
            if (hid == 0 && input_dir[from][0] == '.')
            {
                from++;
                continue;
            }
            if (is_file && strcmp(input_dir[from], file_name) != 0)
            {
                from++;
                continue;
            }
            if (list)
            {
                char permissions[13];
                struct stat get_p;
                char file_path[1001];
                strcpy(file_path, present_dir);
                strcat(file_path, "/");
                strcat(file_path, input_dir[from]);
                stat(file_path, &get_p);
                int track = 0;
                if (S_ISDIR(get_p.st_mode))
                {
                    permissions[track++] = 'd';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IRUSR)
                {
                    permissions[track++] = 'r';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IWUSR)
                {
                    permissions[track++] = 'w';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IXUSR)
                {
                    permissions[track++] = 'x';
                }
                else
                {
                    permissions[track++] = '-';
                }

                if (get_p.st_mode & S_IRGRP)
                {
                    permissions[track++] = 'r';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IWGRP)
                {
                    permissions[track++] = 'w';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IXGRP)
                {
                    permissions[track++] = 'x';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IROTH)
                {
                    permissions[track++] = 'r';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IWOTH)
                {
                    permissions[track++] = 'w';
                }
                else
                {
                    permissions[track++] = '-';
                }
                if (get_p.st_mode & S_IXOTH)
                {
                    permissions[track++] = 'x';
                }
                else
                {
                    permissions[track++] = '-';
                }
                permissions[10] = ' ';
                permissions[11] = '\0';

                int num_hardlinks = get_p.st_nlink;

                struct group *gro = getgrgid(get_p.st_gid);
                struct passwd *pawd = getpwuid(get_p.st_uid);

                long long int sz = get_p.st_size;
                struct tm *timee = localtime(&get_p.st_mtime);
                struct tm times;
                time_t now = time(NULL);
                localtime_r(&now, &times);
                int check = (times.tm_mon - timee->tm_mon);
                // timee -> file
                // times -> current
                if (times.tm_year != timee->tm_year)
                {
                    check += (times.tm_year - timee->tm_year) * 12;
                }
                if (check < 6)
                {
                    printf("%-12s%d %s %s %8lld %s %02d %02d:%02d %s \n", permissions, num_hardlinks, pawd->pw_name, gro->gr_name, sz, month[timee->tm_mon], timee->tm_mday, timee->tm_hour, timee->tm_min, input_dir[from]);
                }
                else if (check > 6)
                {
                    printf("%-12s%d %s %s %8lld %s %02d %04d %s \n", permissions, num_hardlinks, pawd->pw_name, gro->gr_name, sz, month[timee->tm_mon], timee->tm_mday, timee->tm_year + 1900, input_dir[from]);
                }
                else
                {
                    if (times.tm_mday >= timee->tm_mday)
                    {
                        printf("%-12s%d %s %s %8lld %s %02d %04d %s \n", permissions, num_hardlinks, pawd->pw_name, gro->gr_name, sz, month[timee->tm_mon], timee->tm_mday, timee->tm_year + 1900, input_dir[from]);
                    }
                    else
                    {
                        printf("%-12s%d %s %s %8lld %s %02d %02d:%02d %s \n", permissions, num_hardlinks, pawd->pw_name, gro->gr_name, sz, month[timee->tm_mon], timee->tm_mday, timee->tm_hour, timee->tm_min, input_dir[from]);
                    }
                }
            }
            else
            {
                printf("%s\n", input_dir[from]);
            }
            from++;
        }
    }
    else
    {
        perror("Error : Could not open directory");
        return;
    }
}
void execute_ls(char *from_print[], int num_lit)
{
    hid = 0;
    list = 0;
    char present_dir[10001];
    num = 0;
    getcwd(present_dir, 10001);
    int till_read = 1;
    for (int i = 1; i < num_lit; i++)
    {
        // Assumption :Flags will come before
        if (from_print[i][0] == '-')
        {
            if (strlen(from_print[i]) == 1)
            {
                continue;
            }
            till_read++;
            for (int j = 1; j < strlen(from_print[i]); j++)
            {
                if (from_print[i][j] == 'a')
                {
                    hid = 1;
                }
                else if (from_print[i][j] == 'l')
                {
                    list = 1;
                }
                else
                {
                    printf("Invalid Flag %c used in ls", from_print[i][j]);
                    return;
                }
            }
        }
    }
    num = 0; // to count no of dir in argument
    if (till_read == num_lit)
    {
        getcwd(present_dir, 10001);
        print_content(present_dir, from_print[1], false, present_dir);
        return;
    }
    for (int i = 1; i < num_lit; i++)
    {
        if (from_print[i][0] == '-' && strlen(from_print[i]) > 1)
        {
            continue;
        }
        get_dir_path(from_print, present_dir, i);
        num++;
        if (isDirectoryExists(present_dir) == 0 && isFileExists(present_dir) == 0)
        {
            printf("Error: Cannot Access %s: No such file or directory\n", present_dir);
        }
    }
    for (int i = 1; i < num_lit; i++)
    {
        if (from_print[i][0] == '-' && strlen(from_print[i]) > 1)
        {
            continue;
        }
        get_dir_path(from_print, present_dir, i);
        if (isDirectoryExists(present_dir))
        {
            print_content(present_dir, from_print[i], false, present_dir);
        }
        else if (isFileExists(present_dir))
        {
            char file_name[1001];
            int index = -1;
            for (int i = strlen(present_dir) - 1; i >= 0; i--)
            {
                if (present_dir[i] == '/')
                {
                    index = i;
                    break;
                }
            }
            if (index == -1)
            {
                getcwd(present_dir, 10001);
                strcpy(file_name, from_print[i]);
            }
            else
            {
                for (int j = index + 1; j < strlen(present_dir); j++)
                {
                    file_name[j - index - 1] = present_dir[j];
                }
                present_dir[index] = '\0';
            }
            print_content(present_dir, from_print[i], true, file_name);
        }
    }
}