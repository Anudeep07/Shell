#include "../shell.h"

void change_name(char *source, char *destination)
{
    if (rename(source, destination) == -1)
        print_error("Unable to rename. Please check permissions & try again!");
}

void mv()
{
    if (arg_count == 1)
    {
        print_error("Missing file operand");
        return;
    }

    if (arg_count == 2)
    {
        print_error("Missing destination file operand");
        return;
    }
    /*
    int opt;
    int i_flag = 0;

    
    while((opt = getopt(arg_count, arg_values, "i")) != -1)
    {
        switch (opt)
        {
            case 'i':
                i_flag = 1;
                break;
        }
    }*/

    if (arg_count == 3)
    {
        char *source = arg_values[1];
        char *destination = arg_values[2];

        struct stat source_buf;
        struct stat destination_buf;

        //check if source file exists
        if (access(source, F_OK) == -1)
        {
            print_error("Source directory or file doesn't exist.");
            return;
        }

        if (stat(source, &source_buf) == -1) //store source file in source_buf
        {
            print_error("Stat error.");
            return;
        }

        if (access(destination, F_OK) == -1)
        {
            //destination file doesn't exist
            change_name(source, destination);
        }
        else
        {
            if (stat(destination, &destination_buf) == -1)
            {
                print_error("Stat error.");
                return;
            }

            //check if source is regular file
            if (S_ISREG(source_buf.st_mode))
            {
                if (S_ISREG(destination_buf.st_mode))
                {
                    change_name(source, destination);
                }
                else if (S_ISDIR(destination_buf.st_mode))
                {
                    char new_destination[4096];
                    
                    snprintf(new_destination, sizeof(new_destination), "%s/%s", destination, source);
                    change_name(source, new_destination);
                }
                else
                {
                    print_error("Only directory file or regular file is allowed!");
                    return;
                }
            }
            else if (S_ISDIR(source_buf.st_mode)) //check if source is directory
            {
                if (S_ISDIR(destination_buf.st_mode))
                {
                    char new_destination[4096];

                    snprintf(new_destination, sizeof(new_destination), "%s/%s", destination, source);
                    change_name(source, new_destination);
                }
                else
                {
                    print_error("Can't overwrite non-directory with directory!");
                    return;
                }
            }
            else
            {
                print_error("Only directory file or regular file is allowed!");
                return;
            }
        }
    }
}