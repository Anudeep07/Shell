#include "shell.h"                          //shell.h contains all the declarations

//function declarations used in main.c
void setup_cwd();

void prompt();

void split_args();

void execute_command();

void loop_cleanup();

void cleanup();

//defining the variables declared in shell.h
char PWD[BUFFER_LENGTH];
char *HOME;
char *PATH;
char *USER;

char previous_directory[BUFFER_LENGTH];

char *commandline;                          //stores the command line
size_t commandline_length;                  //stores length of command line
char *command;                              //stores the command

int arg_count;                              //stores the no. of arguments
char *arg_values[BUFFER_LENGTH];            //stores the arguments passed to the command

int main()
{
    HOME = getenv("HOME");
    PATH = getenv("PATH");
    USER = getenv("USER");
    chdir(HOME);                            //initially pwd is HOME
    strcpy(previous_directory, HOME);       //initially prev_dir is HOME
    rl_bind_key('\t', rl_complete);         //readline library, bind tab to auto complete

    while(1)
    {
        setup_cwd();
        prompt();

        if(commandline_length == 0)
            continue;

        split_args();
        execute_command();     
        loop_cleanup();
    }

    cleanup();
    return 0;
}

void setup_cwd()
{
    if((getcwd(PWD, BUFFER_LENGTH)) == NULL)
    {
        print_error("getcwd", 1);
        exit(-1);
    }
}

void prompt()
{
    char prompt[100];
    snprintf(prompt, sizeof(prompt), ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ":" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET " $ ", USER, PWD);

    //readline library (display prompt & read input, returns char* to the input string)
    commandline = readline(prompt);  
    commandline_length = strlen(commandline);

    //readline library (don't add empty lines to history)
    if(commandline && *commandline)
        add_history(commandline);    
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
        &change_permission,
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
            opterr = 0;
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

void loop_cleanup()
{
    free(commandline);
}

void cleanup()
{
}

void print_error(char *str, int perr)
{
    fprintf(stderr, ANSI_COLOR_RED);

    //fprintf(stderr, "Error: %s", str);
    fprintf(stderr, "%s: ", arg_values[0]);

    if(perr)
        perror(str);
    else
    {
        fprintf(stderr, "%s\n", str);
    }

    fprintf(stderr, ANSI_COLOR_RESET);
}

void shell_exit()
{
    loop_cleanup();
    cleanup();
    exit(0);
}

void removedir(){}

void ln(){}

void rm(){}

void change_permission(){}

void touch(){}

void help(){}

