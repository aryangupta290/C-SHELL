#ifndef _HEADERS_
#define _HEADERS_

// Including all header files
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
// declaring all the global variables to be used
extern char shell_dir[10001]; // directory from where we invoked shell
extern char user_name[10001];
extern char system_name[10001];
extern char cur_dir[10001]; // directory where we are currently
extern char *commands[10001];
extern char *each_segment[1001];
extern char last_dir[10001];
struct jobs
{
    pid_t process_id;
    char command_name[1001];
    int index_no;
};
extern struct jobs bg_process_info[1001];
extern struct jobs fg_process_info;
extern int bg_process_count;
extern char history_file_name[1001];
extern int history_file_count;
extern char history_commands[20][1001];
extern int shell_id;
extern bool is_sleep;
extern int interval;
extern int period;
// All functions name
void p_prompt();
bool check_prefix();
void run_command(char *each_segment[], int num_segment);
void execute_pwd();
void execute_echo(char *from_print[], int num_lit);
void execute_cd(char *from_print[], int num_lit);
void execute_ls(char *from_print[], int num_lit);
void get_dir_path(char *from_print[], char present_dir[], int i);
void print_content(char *present_dir, char from_print[], bool is_file, char file_name[]);
int get_block_size(char *present_dir);
void execute_foreground(char *from_print[]);
void execute_background(char *from_print[]);
void execute_pinfo(char *from_print[], int k);
bool check_if_add(int ppid, int pgrp);
void sigchld_handler();
void init_bg_process_info();
void add_bg_process(int id, char command[]);
void remove_bg_process(int id);
int check_bg_process(int id);
int isFileExists(const char *path);
int isDirectoryExists(const char *path);
void print_error_cd();
void execute_history(char *from_print[], int num);
void add_command(char from_print[]);
void init_history();
void execute_jobs(char *from_print[], int num_lit);
void execute_sig(char *from_print[], int num_lit);
void execute_fg(char *from_print[], int num_lit);
void execute_bg(char *from_print[], int num_lit);
void handler_ctrl_c();
void handler_ctrl_z();
void execute_baywatch(char *from_print[], int num_lit);
void function_call(int arg);
bool was_keyboard_pressed();
void execute_dirty();
void execute_newborn();
void print_line();
void execute_interrupt();
#endif