#include "headers.h"
char shell_dir[10001]; // directory from where we invoked shell
char user_name[10001];
char system_name[10001];
char cur_dir[10001]; // directory where we are currently
char *commands[10001];
char *each_segment[1001];
char last_dir[10001];
struct jobs bg_process_info[1001];
int bg_process_count = 0;
char history_file_name[1001];
int history_file_count = 0;
char history_commands[20][1001];
struct jobs fg_process_info;
int shell_id;
bool is_sleep;
int interval = 0;
int period = 0;

int main(void)
{
    // First we should store the sys name and the path of directory from where we invoke shell
    shell_id = getpid();
    signal(SIGINT, handler_ctrl_c);
    signal(SIGTSTP, handler_ctrl_z);
    getcwd(shell_dir, sizeof(shell_dir));
    getlogin_r(user_name, sizeof(user_name));
    gethostname(system_name, sizeof(system_name));
    strcpy(history_file_name, "");
    strcat(history_file_name, shell_dir);
    strcat(history_file_name, "/store_history.txt");
    init_bg_process_info();
    init_history();
    strcpy(last_dir, "");
    printf("\033[0;31m");
    printf("\n WELCOME TO MY SHELL \n");
    printf("\033[0m");
L:
    while (true)
    {
        signal(SIGCHLD, sigchld_handler);
        printf("\033[0;34m");
        p_prompt();
        printf("\033[0m");
        fg_process_info.process_id = -1;
        ssize_t sz = 0;
        char *instruction_line;
        instruction_line = NULL;
        int length = getline(&instruction_line, &sz, stdin);
        if (length <= 0)
        {
            printf("\n");
            fflush(stdout);
            exit(0);
        }
        char for_history[1001];
        for (int i = 0; i < length; i++)
        {
            for_history[i] = instruction_line[i];
        }
        for_history[length] = '\0';

        add_command(for_history);
        if (instruction_line[0] == '\0')
        {
            continue;
        }
        int how_many = 0;
        commands[how_many] = strtok(instruction_line, ";\n");
        while (commands[how_many] != NULL)
        {
            how_many++;
            commands[how_many] = strtok(NULL, ";\n");
        }
        if (how_many == 0)
        {
            perror("Please Enter a valid command");
            continue;
        }
        for (int i = 0; i < how_many; i++)
        {
            // now prcesss every command
            int num_segment = 0;
            each_segment[num_segment] = strtok(commands[i], " \t\r\n");
            while (each_segment[num_segment] != NULL)
            {
                num_segment++;
                each_segment[num_segment] = strtok(NULL, " \n\r\t");
            }
            each_segment[num_segment] = NULL;
            if (num_segment == 0)
            {
                continue;
            }
            is_sleep = false;
            if (strcmp(each_segment[0], "replay") == 0)
            {
                is_sleep = true;
                if (num_segment < 6)
                {
                    printf("Error : Wrong syntax for replay command");
                    continue;
                }
                if (strcmp(each_segment[1], "-command") != 0 || strcmp(each_segment[num_segment - 2], "-period") != 0 || strcmp(each_segment[num_segment - 4], "-interval") != 0)
                {
                    printf("Error : Wrong syntax for replay command");
                    continue;
                }
                interval = atoi(each_segment[num_segment - 3]);
                period = atoi(each_segment[num_segment - 1]);
                int start = 0;
                for (int i = 2; i < num_segment - 4; i++)
                {
                    strcpy(each_segment[start], each_segment[i]);
                    start++;
                }
                num_segment -= 6;
                each_segment[num_segment] = NULL;
            }
            if (strcmp(each_segment[0], "quit") == 0)
            {
                printf("\033[0;31m");
                printf(" Thank you for using my shell....adios\n");
                printf("\033[0m");
                exit(0);
            }
            // handling pipes
            int num_pipes = 0;
            for (int i = 0; i < num_segment; i++)
            {
                if (strcmp(each_segment[i], "|") == 0)
                {
                    num_pipes++;
                }
            }
            if (num_pipes == 0)
                run_command(each_segment, num_segment);
            else
            {
                int pipe_pos[num_pipes + 2];
                pipe_pos[0] = -1;
                int start = 1;
                for (int i = 0; i < num_segment; i++)
                {
                    if (strcmp(each_segment[i], "|") == 0)
                    {
                        pipe_pos[start++] = i;
                    }
                }
                pipe_pos[start] = num_segment;
                int file_des[2 * num_pipes];
                for (int i = 0; i < 2 * num_pipes; i += 2)
                {
                    if (pipe(file_des + i))
                    {
                        printf("Error : Could not create pipe");
                        goto L;
                    }
                }
                for (int i = 0; i < num_pipes + 1; i++)
                {
                    int pid = fork();
                    if (pid == 0)
                    {
                        if (i < num_pipes)
                        {
                            dup2(file_des[2 * i + 1], 1); // set output stream
                        }
                        if (i > 0)
                        {
                            dup2(file_des[2 * i - 2], 0);
                        }
                        char *piped_command[pipe_pos[i + 1] - pipe_pos[i]];
                        start = 0;
                        for (int j = pipe_pos[i] + 1; j < pipe_pos[i + 1]; j++)
                        {
                            piped_command[start] = (char *)malloc(sizeof(char) * strlen(each_segment[j]));
                            strcpy(piped_command[start], each_segment[j]);
                            start++;
                        }
                        piped_command[pipe_pos[i + 1] - pipe_pos[i] - 1] = (char *)malloc(2);
                        strcpy(piped_command[pipe_pos[i + 1] - pipe_pos[i] - 1], "\0");
                        for (int j = 0; j < 2 * num_pipes; j++)
                        {
                            close(file_des[j]);
                        }
                        run_command(piped_command, pipe_pos[i + 1] - pipe_pos[i] - 1);
                        for (int j = 0; j < pipe_pos[i + 1] - pipe_pos[i] - 1; j++)
                        {
                            free(piped_command[j]);
                        }
                        exit(2);
                    }
                }
                for (int i = 0; i < 2 * num_pipes; i++)
                {
                    close(file_des[i]);
                }
                for (int i = 0; i <= num_pipes; i++)
                {
                    wait(NULL);
                }
            }
        }
    }
    return (0);
}