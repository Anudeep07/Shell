//Header files that are needed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define BUFFER_LENGTH 4096

//colour codes for printf
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

/*
--------------------------------------------------------------------------------
 Function declarations
--------------------------------------------------------------------------------
*/

void setup_cwd();

void prompt();

void split_args();

void execute_command();

//command functions start ====================================================
void cat();

void cd();

void ls();

void makedir();

void removedir();

void cp();

void mv();

void ln();

void rm();

void echo();

void clearscreen();

void chmod();

void pwd();

void touch();

void home();

void path();

void help();

void shell_exit();
//command functions end ====================================================

/*
--------------------------------------------------------------------------------
 Declaration of variables
--------------------------------------------------------------------------------
*/

char PWD[BUFFER_LENGTH];
char *HOME;

char commandline[BUFFER_LENGTH*2];       //stores the command line
int commandline_length;                 //stores the actual length of the commandline
char *command;                          //stores the command

int arg_count;                          //stores the no. of arguments
char *arg_values[100];                      //stores the arguments passed to the command

//builtin commands
char *builtin[] = {
    "cat",
    "cd",
    "ls",
    "mkdir",
    "rmdir",
    "cp",
    "mv",
    "ln",
    "rm",
    "echo",
    "clear",
    "chmod",
    "pwd",
    "touch",
    "home",
    "path",
    "help",
    "exit"
};

int count_builtin = sizeof(builtin) / sizeof(char*);

//function pointer to the respective commands
void (*functions[])() = {
    &cat,
    &cd,
    &ls,
    &makedir,
    &removedir,
    &cp,
    &mv,
    &ln,
    &rm,
    &echo,
    &clearscreen,
    &chmod,
    &pwd,
    &touch,
    &home,
    &path,
    &help,
    &shell_exit
};
