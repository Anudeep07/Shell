#include "../shell.h"

#define PERMISSION 0664 //RWX for owner, group & others

bool interactive(char *destination);                    //defined in mv.c
char *last_component(char *str);                        //defined in mv.c
char *append_source(char *source, char *destination);   //defined in mv.c

bool i_flag;

void copy_file(char *source, char *destination)
{
    int f1, f2, n;
	char buf[4096];	
    bool choice = true;

	if((f1 = open(source, O_RDONLY, 0)) == -1)
	{
		perror("open");
		return;
	}

    //if iflag is set, then we have to see if file exists.
    //if file exists, then ask for prompt
    if(i_flag && access(destination, F_OK) == 0)
        choice = interactive(destination);          //function defined in mv.c
    
    if(choice)
    {
        if((f2 = open(destination, O_WRONLY | O_CREAT | O_TRUNC, PERMISSION)) == -1)
        {
            perror("open");
            return;
        }

        while((n = read(f1, buf, sizeof(buf))) > 0)
            if(write(f2, buf, n) != n)
            {
                perror("write");
                return;
            }
    }
}

/*
 * Minimum 3 args, cp src dest
 * If more than 3, then last arg must be destination directory
 *  
 * if 2 args, then last one can be regular file or directory file
 * if directory file, then cp src dir1 => overwrite dir1/src
 * if regular file, then cp src dest => overwrite dest
 * 
 * Note: src must always be a regular file (otherwise, have to recursively cp)
 */
void cp()
{
    if(arg_count <= 2)
    {
        print_error("Usage: cp [options] <source(s)> <destination>", 0);
        return;
    }

    int opt;
    i_flag = false;   
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

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than cp all are files

    if(file_count < 2)
    {
        print_error("Usage: cp [options] <source(s)> <destination>", 0);
        return;
    }

    if(file_count == 2)
    {
        int index = optind ? optind : 1;
        char *source = arg_values[index];
        char *destination = arg_values[index+1];

        struct stat source_buf, destination_buf;

        if(access(source, F_OK) == -1)
        {
            char message[30];
            snprintf(message, sizeof(message), "%s doesn't exist.", source);
            print_error(message, 0);
            return;
        }

        if (stat(source, &source_buf) == -1) //store source file in source_buf
        {
            print_error("stat", 1);
            return;
        }

        if(S_ISREG(source_buf.st_mode))
        {
            /*
             * Source is a regular file.
             * if destination doesn't exist, then directly write
             * else, check if its a directory/regular
             * if directory then place regular inside directory
             * else (regular), overwrite
             */

            if(access(destination, F_OK) == -1)
            {
                //destination file doesn't exist
                copy_file(source, destination);
            }
            else
            {
                //destination file exists. Therefore we check if its directory or regular
                if(stat(destination, &destination_buf) == -1)
                {
                    print_error("stat", 1);
                    return;
                }

                if(S_ISDIR(destination_buf.st_mode))
                {
                    /* destination file is a directory
                     * Therefore, get the last component of src and append to destination 
                     * Using function written in mv.c
                     */
                    char *new_dest = append_source(source, destination);
                    copy_file(source, new_dest);                    
                }
                else
                {
                    //destination file is a regular file 
                    copy_file(source, destination);
                }                
            }            
        }
        else
        {
            /*
             * Source is a directory file
             */
            char message[30];
            snprintf(message, sizeof(message), "%s must be a regular file.", source);
            print_error(message, 0);
        }//else (source is directory)        
    }//if(file_count == 2)
    else
    {
        char *source, *destination = arg_values[arg_count-1];

        //destination must be a directory & it must exist
        if(access(destination, F_OK) == -1)
        {
            //destination doesn't exist
            char message[30];
            snprintf(message, sizeof(message), "%s not found.", destination);
            print_error(message, 0);
            return;
        }

        struct stat destbuf;
        if(stat(destination, &destbuf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        if(!S_ISDIR(destbuf.st_mode))
        {
            char message[30];
            snprintf(message, sizeof(message), "%s must be a directory.", destination);
            print_error(message, 0);
            return;
        }

        //last one is the destination & must be a directory
        for(int i=optind ; i<arg_count-1 ; i++)
        {
            source = arg_values[i];
            if(access(source, F_OK) == -1)
            {
                char message[30];
                snprintf(message, sizeof(message), "%s doesn't exist.", source);
                continue;
            }

            //source must be a directory file
            struct stat sourcebuf;
            if(stat(source, &sourcebuf) == -1)
            {
                print_error("stat", 1);
                return;
            }

            if(S_ISREG(sourcebuf.st_mode))
            {                
                char *new_destination = append_source(source, destination);
                copy_file(source, destination);
            }
            else
            {
                char message[30];
                snprintf(message, sizeof(message), "%s must be a regular file.", source);
                continue;
            }
        }
    }
}//function