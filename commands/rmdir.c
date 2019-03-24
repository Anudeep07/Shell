#include "../shell.h"

bool v_flag;

void removedir()
{
    if(arg_count == 1)
    {
        print_error("Usage: rmdir [options] <file(s)>", 0);
        return;
    }

    int opt;
    v_flag = false;
    char err[25];

    while((opt = getopt(arg_count, arg_values, "v")) != -1)
    {
        switch (opt)
        {
            case 'v':                
                v_flag = true;                
                break;
            default:
                snprintf(err, sizeof(err), "invalid option -- '%c'", optopt);
                print_error(err, 0);
                return;
        }
    }

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than mkdir all are files

    if(file_count < 1)
    {
        print_error("Usage: rmdir [options] <file(s)>", 0);
        return;
    }

    for(int i=optind ; i<arg_count ; i++)
    {
        if(v_flag)
            printf("%s: removing directory '%s'\n", arg_values[0], arg_values[i]);
        
        if(rmdir(arg_values[i]) == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED);

            fprintf(stderr, "%s: failed to remove '%s' ", arg_values[0], arg_values[i]);
            perror("rmdir");

            fprintf(stderr, ANSI_COLOR_RESET);
        }
    }
}