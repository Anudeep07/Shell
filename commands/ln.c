#include "../shell.h"

bool s_flag;

void ln()
{
    if(arg_count < 3)
    {
        print_error("Usage: ln [options] target linkname", 0);
        return;
    }

    int opt;
    s_flag = false;   
    char err[25];
    
    while((opt = getopt(arg_count, arg_values, "s")) != -1)
    {
        switch (opt)
        {
            case 's':                
                s_flag = true;                                
                break;
            default:
                snprintf(err, sizeof(err), "invalid option -- '%c'", optopt);
                print_error(err, 0);
                return;
        }
    }

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than mkdir all are files

    if(file_count != 2)
    {
        print_error("Usage: ln [options] target linkname", 0);
        return;
    }

    char *target = arg_values[optind];
    char *linkpath = arg_values[optind+1];

    if(s_flag)
    {
        if(symlink(target, linkpath) == -1)
        {
            print_error("symlink", 1);
            return;
        }
    }
    else
    {
        if(link(target, linkpath) == -1)
        {
            print_error("link", 1);
            return;
        }
    }    
}