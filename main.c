#include "shell.h"                          //shell.h contains all the declarations

//defining the variables declared in shell.h
char PWD[BUFFER_LENGTH];
char *HOME;
char *PATH;

char previous_directory[BUFFER_LENGTH];

char *commandline = NULL;                   //stores the command line
size_t commandline_buffer_length = 0;       //stores the length of the commandline buffer (buffer created using getline)
size_t commandline_length;                  //stores the actual length of the commandline
char *command;                              //stores the command

int arg_count;                              //stores the no. of arguments
char *arg_values[100];                      //stores the arguments passed to the command


int main()
{
    HOME = getenv("HOME");
    PATH = getenv("PATH");
    chdir(HOME);                            //initially pwd is HOME
    strcpy(previous_directory, HOME);       //initially prev_dir is HOME

    while(1)
    {
        setup_cwd();
        prompt();

        if(commandline_length == 0)         //otherwise segmentation fault when executing strcmp(builtin[i], command) in exec_command
            continue;

        split_args();
        execute_command();     
    }

    return 0;
}

void setup_cwd()
{
    if((getcwd(PWD, BUFFER_LENGTH)) == NULL)
    {
        print_error("getcwd failed");
        exit(-1);
    }
}

void prompt()
{
    printf(ANSI_COLOR_GREEN     "%s"    ANSI_COLOR_RESET, PWD);
    printf(" $ ");

    getline(&commandline, &commandline_buffer_length, stdin);
    commandline_length = strlen(commandline);

    //getline inserts \n at the end
    if(commandline[commandline_length-1] == '\n')
        commandline[--commandline_length] = '\0';
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

void print_error(char *str)
{
    fprintf(stderr, ANSI_COLOR_RED);

    fprintf(stderr, "Error: %s", str);

    fprintf(stderr, ANSI_COLOR_RESET "\n");
}

void shell_exit()
{
    exit(0);
}

void ls(){}

void makedir(){}

void removedir(){}

void cp(){}

void mv(){}

void ln(){}

void rm(){}

void change_permission(){}

void touch(){}

void help(){}

