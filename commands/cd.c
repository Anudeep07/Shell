#include "../shell.h"

void change_dir(char *str)
{
    char temp[BUFFER_LENGTH];
    strcpy(temp, PWD);                 //update PWD to prev_dir if cd is successful, otherwise dont modify prev_dir

    if (chdir(str) == -1)
        print_error("chdir", 1);
    else
        strcpy(previous_directory, temp);
}

void check(char *str)
{
    //check if file exists
    if(access(str, F_OK) == -1)
    {
        print_error("access", 1);
        return;
    }

    struct stat buf;

    if(stat(str, &buf) == -1)       //stat stores the attributes of file str in buf
    {
        print_error("stat", 1);
        return;
    }

    if(! S_ISDIR(buf.st_mode))
    {
        //not a directory
        print_error("Not a directory.", 0);
        return;
    }

    //check if file has execute permissions
    if(access(str, X_OK) == -1)
    {
        print_error("access", 1);
        return;
    }
    
    change_dir(str);
}

void cd()
{
    if (arg_count > 2)
    {
        print_error("Too many arguments", 0);
        return;
    }

    if (arg_count == 1)
    {
        change_dir(HOME);
    }
    else
    {
        if (!strcmp(arg_values[1], "-"))   //check if its -
            check(previous_directory);
        else
            check(arg_values[1]);
    }
}