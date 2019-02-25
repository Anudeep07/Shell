#include "shell.h"                        //shell.h contains all the declarations

//defining the variables declared in shell.h
char PWD[BUFFER_LENGTH];
char *HOME;

char commandline[BUFFER_LENGTH*2];       //stores the command line
int commandline_length;                  //stores the actual length of the commandline
char *command;                           //stores the command

int arg_count;                           //stores the no. of arguments
char *arg_values[100];                   //stores the arguments passed to the command


int main()
{
    HOME = getenv("HOME");

    do
    {
        setup_cwd();
        prompt();
        split_args();
        execute_command();

    }while(1);

    return 0;
}

void setup_cwd()
{
    if((getcwd(PWD, BUFFER_LENGTH)) == NULL)
    {
        fprintf(stderr, "Error: getcwd failed");
        exit(-1);
    }
}

void prompt()
{
    printf(ANSI_COLOR_GREEN     "%s"    ANSI_COLOR_RESET, PWD);
    printf(" $ ");

    fgets(commandline, sizeof(commandline), stdin);
    commandline_length = strlen(commandline);

    //to remove the \n added by fgets
    if(commandline[commandline_length-1] == '\n')
        commandline[commandline_length-1] = '\0';
}

void split_args()
{
    char *ptr;
    arg_count = 0;

    command = strtok(commandline, " ");
    arg_values[arg_count++] = command;

    while((ptr = strtok(NULL, " ")) != NULL)
        arg_values[arg_count++] = ptr;
}

void execute_command()
{
    int i;

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

    for(i=0 ; i<count_builtin ; i++)
    {
        if(!strcmp(builtin[i], command))
        {
            optind = 0;                         //setting getopt() index to 1 (starts scanning for options from arg_values[optind]
            (*functions[i])();                 //respective function will be called
            break;
        }
    }

    if(i == count_builtin)
    {
        printf(ANSI_COLOR_RED "Not a builtin command." ANSI_COLOR_RESET "\n");
        //external command
        //fork a child & exec
        //wait for child to exit
    }
}

void shell_exit()
{
    exit(0);
}


void cd(){}

void ls(){}

void makedir(){}

void removedir(){}

void cp(){}

void mv(){}

void ln(){}

void rm(){}

void echo(){}

void clearscreen(){}

void chmod(){}

void pwd(){}

void touch(){}

void home(){}

void path(){}

void help(){}

