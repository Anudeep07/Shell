#include "shell.h"

int main()
{
    HOME = getenv("HOME");

    do
    {
        setup_cwd();
        prompt();
        command = strtok(commandline, " ");
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

    /*working of strtok

    first time pass the string to be parsed
    char *ptr = strtok(commandline, " ");
    while(ptr != NULL)
    {
        //next time pass NULL to parse the same string
        ptr = strtok(NULL, " ");
    }*/
}

void execute_command()
{
    int i;
    for(i=0 ; i<count_builtin ; i++)
    {
        if(!strcmp(builtin[i], command))
        {
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

void cat()
{
    printf("cat command ^_^\n");
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

