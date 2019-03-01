#include "../shell.h"

void echo()
{
    for(int i=1 ; i<arg_count ; i++)
        printf("%s ", arg_values[i]);

    printf("\n");
}

