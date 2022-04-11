#include "headers.h"
void run_command(char *each_segment[], int num_segment)
{
    int is_redir = 0; // check whether we need to do redirection or not
    int count_inp_redir = 0;
    int count_out_redir = 0;
    bool which_out_dir = false; // false -> > , true -> >>
    int inp_file, out_file;
    int sign1_pos = num_segment, sign2_pos = num_segment;
    for (int i = 0; i < num_segment - 1; i++)
    {
        if (strcmp(each_segment[i], "<") == 0)
        {
            sign1_pos = i;
            is_redir = 1;
            count_inp_redir++;
            inp_file = num_segment - 1;
            for (int j = i + 1; j < num_segment; j++)
            {
                if (strcmp(each_segment[j], ">") == 0 || strcmp(each_segment[j], ">>") == 0)
                {
                    inp_file = j - 1;
                    break;
                }
            }
        }
        else if (strcmp(each_segment[i], ">") == 0 || strcmp(each_segment[i], ">>") == 0)
        {
            sign2_pos = i;
            is_redir = 1;
            count_out_redir++;
            if (strcmp(each_segment[i], ">>") == 0)
            {
                which_out_dir = true;
            }
            if (i < num_segment - 1)
            {
                out_file = i + 1;
            }
        }
    }
    if (count_inp_redir > 1 || count_out_redir > 1)
    {
        printf("Error : Enter a valid command");
        return;
    }
    int store_stdin, store_stdout, input_fd, output_fd;
    if (count_out_redir)
    {
        store_stdout = dup(1);
        if (which_out_dir)
        {
            output_fd = open(each_segment[out_file], O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            output_fd = open(each_segment[out_file], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (output_fd < 0)
        {
            printf("Error : Could not open file to redirect output");
            close(store_stdout);
            return;
        }
        if (dup2(output_fd, 1) == -1)
        {
            printf("Error : Could not redirect output");
            return;
        }
    }
    if (count_inp_redir)
    {
        store_stdin = dup(0);
        input_fd = open(each_segment[inp_file], O_RDONLY);
        if (input_fd < 0)
        {
            printf("Error : Could not open file to redirect input");
            close(store_stdin);
            return;
        }
        if (dup2(input_fd, 0) == -1)
        {
            printf("Error : Could not redirect input");
            return;
        }
    }
    char temp_command[1001], temp_num = 0;
    int till = sign1_pos;
    if (sign2_pos < sign1_pos)
    {
        till = sign2_pos;
    }
    num_segment = till;
    each_segment[till] = '\0';
    if (strcmp(each_segment[num_segment - 1], "&") == 0)
    {
        each_segment[num_segment - 1] = NULL;
        execute_background(each_segment);
        return;
    }
    if (each_segment[num_segment - 1][strlen(each_segment[num_segment - 1]) - 1] == '&')
    {
        each_segment[num_segment - 1][strlen(each_segment[num_segment - 1]) - 1] = '\0';
        execute_background(each_segment);
        return;
    }
    long long int num_repeat = 1;
    int index = 0;
    bool is_correct = true;
    if (strcmp(each_segment[0], "repeat") == 0)
    {
        bool is_present = false;
        for (int j = 0; j < num_segment; j++)
        {
            if (is_present)
            {
                bool is_number = true;
                for (int k = 0; k < strlen(each_segment[j]); k++)
                {
                    if (each_segment[j][k] < 48 || each_segment[j][k] > 57)
                    {
                        is_number = false;
                        break;
                    }
                }
                if (!is_number)
                {
                    is_correct = false;
                    break;
                }
                else
                {
                    int to_multiply = atoi(each_segment[j]);
                    num_repeat *= to_multiply;
                    is_present = false;
                }
            }
            else
            {
                if (strcmp(each_segment[j], "repeat") == 0)
                {
                    is_present = true;
                }
                else
                {
                    index = j;
                    for (int k = index; k < num_segment; k++)
                    {
                        strcpy(each_segment[k - index], each_segment[k]);
                    }
                    num_segment -= index;
                    each_segment[num_segment] = NULL;
                    break;
                }
            }
        }
        if (!is_correct || is_present == true)
        {
            perror("Error : Wrong syntax in repeat command");
            return;
        }
        if (num_segment == 0)
        {
            printf("Error : No command entered to repeat");
            return;
        }
    }
    if (is_sleep)
        num_repeat *= (period / interval);
    for (int p = 0; p < num_repeat; p++)
    {
        if (is_sleep)
        {
            sleep(interval);
        }
        if (strcmp(each_segment[0], "quit") == 0)
        {
            printf("\033[0;31m");
            printf(" Thank you for using my shell....adios\n");
            printf("\033[0m");

            exit(0);
        }
        else if (strcmp(each_segment[0], "pwd") == 0)
        {
            execute_pwd();
        }
        else if (strcmp(each_segment[0], "echo") == 0)
        {
            execute_echo(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "cd") == 0)
        {
            execute_cd(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "ls") == 0)
        {
            execute_ls(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "pinfo") == 0)
        {
            execute_pinfo(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "history") == 0)
        {
            execute_history(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "jobs") == 0)
        {
            execute_jobs(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "sig") == 0)
        {
            execute_sig(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "fg") == 0)
        {
            execute_fg(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "bg") == 0)
        {
            execute_bg(each_segment, num_segment);
        }
        else if (strcmp(each_segment[0], "baywatch") == 0)
        {
            execute_baywatch(each_segment, num_segment);
        }
        else
        {
            execute_foreground(each_segment);
        }
    }
    if (is_sleep)
    {
        sleep(period % interval);
    }
    if (count_inp_redir)
    {
        close(input_fd);
        dup2(store_stdin, 0);
    }
    if (count_out_redir)
    {
        close(output_fd);
        dup2(store_stdout, 1);
    }
}