//Header files that are needed
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>

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

void print_error(char *str);

void loop_cleanup();

void cleanup();

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

void change_permission();

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

extern char PWD[BUFFER_LENGTH];
extern char *HOME;
extern char *PATH;
extern char *USER;

extern char previous_directory[BUFFER_LENGTH];

extern char *commandline;                      //stores the command line
extern size_t commandline_length;               //stores the length of command line
extern char *command;                          //stores the command

extern int arg_count;                          //stores the no. of arguments
extern char *arg_values[BUFFER_LENGTH];                      //stores the arguments passed to the command

extern int optind;
extern int optopt;