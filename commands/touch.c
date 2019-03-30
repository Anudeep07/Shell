#include "../shell.h"

#define PERMISSION 0644

void touch()
{
    if(arg_count < 0)
    {
        print_error("Usage: touch file(s)", 0);
        return;
    }

    for(int i=1 ; i<arg_count ; i++)
    {
        if(access(arg_values[i], F_OK) == -1)
        {
            //if file doesn't exist
            int fd;
            if((fd = open(arg_values[i], O_WRONLY | O_CREAT | O_TRUNC, PERMISSION)) == -1)
            {
                perror("open");
                continue;
            }          
            close(fd);      
        }

        if(utime(arg_values[i], NULL) == -1)
            print_error("utime", 1);
    }
}