#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

char PWD[BUFFER_LENGTH];
char *HOME;

char commandline[BUFFER_LENGTH*2];      //stores the command line
int commandline_length;
char *command;                          //stores the command

int main()
{
    HOME = getenv("HOME");

    do
    {
        setup_cwd();
        prompt();

        command = strtok(commandline, " ");

        /*
        execute command ^_^
        also remove while condition ^_^
        */

    }while(strcmp(commandline, "exit"));

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
