#include "../shell.h"

void cat()
{
    int opt;

    while((opt = getopt(arg_count, arg_values, "nEs")) != -1)
    {

        switch(opt)
        {
        case 'n':
            printf("option n\n");
            break;
        case 'E':
            printf("option E\n");
            break;
        case 's':
            printf("option s\n");
            break;
        default:
            printf("Default: %c , char = %c\n", opt, optopt);
        }
    }

    for(int i=optind ; i<arg_count ; i++)
        printf("Non-option arg = %s\n", arg_values[i]);
}

