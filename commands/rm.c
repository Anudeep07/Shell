#include "../shell.h"

bool i_flag;
bool choice;

static bool interactive_rm(char *file)
{
    char choice;
    printf("Do you want to remove %s? (y/Y to remove) ", file);
    scanf("%c", &choice);
    getchar();                  //to remove \n from the input buffer

    //if choice is other than y or Y, return
    if(choice != 'y' && choice != 'Y')
        return false;                        
    return true;
}

void rm()
{
    if(arg_count == 1)
    {
        print_error("Usage: rm [options] <file(s)>", 0);
        return;
    }

    int opt;
    i_flag = false;
    choice = true;
    char err[25];

    while((opt = getopt(arg_count, arg_values, "i")) != -1)
    {
        switch (opt)
        {
            case 'i':                
                i_flag = true;                
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
        print_error("Usage: rm [options] <file(s)>", 0);
        return;
    }

    for(int i=optind ; i<arg_count ; i++)
    {       
        if(i_flag)
            choice = interactive_rm(arg_values[i]);

        if(choice && unlink(arg_values[i]) == -1)
        {
            fprintf(stderr, ANSI_COLOR_RED);

            fprintf(stderr, "%s: failed to remove '%s' ", arg_values[0], arg_values[i]);
            perror("rm");

            fprintf(stderr, ANSI_COLOR_RESET);
        }
    }
}