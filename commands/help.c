#include "../shell.h"

void help()
{
    if(arg_count != 1)
    {
        print_error("Usage: help", 0);
        return;
    } 

    clearscreen();
    printf("\t\t\t\tWelcome to the Shell Manual\n");    
    printf("\t\t\t\t---------------------------\n");    

    printf("This shell has the following in-built commands:\n");
    printf("    1. cat      -   concatenate files and print on the standard output\n");
    printf("    2. cd       -   change working directory\n");
    printf("    3. chmod    -   change permissions of a file\n");
    printf("    4. clear    -   clear the terminal screen\n");
    printf("    5. cp       -   copy files and directories\n");
    printf("    6. echo     -   display a line of text\n");
    printf("    7. help     -   show the manual of the shell\n");
    printf("    8. home     -   display the home directory of the shell\n");
    printf("    9. ln       -   make links between files\n");
    printf("    10. ls      -   list directory contents\n");
    printf("    11. mkdir   -   make directories\n");
    printf("    12. mv      -   move (rename) files\n");
    printf("    13. path    -   display the path environment variable of the shell\n");
    printf("    14. pwd     -   display the current working directory\n");
    printf("    15. rm      -   remove files\n");
    printf("    16. rmdir   -   remove empty directories\n");
    printf("    17. touch   -   change file timestamps\n");
    
    char c;
    while((c = getchar()) != '\n')
        ;
    clearscreen();
}