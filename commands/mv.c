#include "../shell.h"

bool i_flag;

bool interactive(char *destination)
{
    char choice;
    printf("Do you want to overwrite %s? (y/Y to overwrite) ", destination);
    scanf("%c", &choice);
    getchar();                  //to remove \n from the input buffer

    //if choice is other than y or Y, return
    if(choice != 'y' && choice != 'Y')
        return false;                        
    return true;
}

//renames source to destination
static void change_name(char *source, char *destination)
{
    bool choice = true;

    if(i_flag && access(destination, F_OK) == 0)
    {
        //file exists
        choice = interactive(destination);
    }

    if (choice && rename(source, destination) == -1)
        print_error("rename", 1);
}

//returns the pointer to the last component in a path. Eg: /home/anudeep/abc, returns char pointer to "abc"
char *last_component(char *str)
{
    int start = 0, previous;
    int i;

    int len = strlen(str);
    for(i=0 ; i<len ; i++)
        if(str[i] == '/')
        {
            //to handle cases like dir1/abc//// 
            if(str[start] != '/')
                previous = start;

            start = i+1;
        }      
 
    char *last;
    if(start == i)
    {
        //has / at the end, Eg: /dir1/abc/ 
        last = (char *) malloc(sizeof(char) * (start-previous+1));

        for(i=previous ; i<start ; i++)
        {
            last[i-previous] = str[i];
        }

        last[i-previous] = '\0';
    }
    else
    {
        last = (char *) malloc(sizeof(char) * (i-start+1)) ;

        for(i=start ; str[i] != '\0' ; i++)
            last[i-start] = str[i];    
        
        last[i-start] = '\0';
    }        

    return last;
}

//moves source file inside destination directory (destination must be dir)
char *append_source(char *source, char *destination)
{

    char *new_source = last_component(source);
    char *new_destination = (char *) malloc(sizeof(char) * 4096);
    
    snprintf(new_destination, sizeof(new_destination), "%s/%s", destination, new_source);
    //change_name(source, new_destination);

    free(new_source);
    return new_destination;
}

void mv()
{
    if (arg_count <= 2)
    {
        print_error("Usage: mv <source(s)> <destination>", 0);
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

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than mv all are files

    if (file_count == 2)
    {
        int index = optind ? optind : 1;
        char *source = arg_values[index];
        char *destination = arg_values[index+1];

        struct stat source_buf;
        struct stat destination_buf;

        if (stat(source, &source_buf) == -1) //store source file in source_buf
        {
            print_error("stat", 1);
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
                print_error("stat", 1);
                return;
            }

            //check if both the file are not same
            if (SAME_INODE(source_buf, destination_buf))
            {
                print_error("Source and destination files can't be same.", 0);
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
                    char *new_destination = append_source(source, destination);
                    change_name(source, new_destination);

                    free(new_destination);
                }
                else
                {
                    print_error("Only directory file or regular file is allowed!", 0);
                    return;
                }
            }
            else if (S_ISDIR(source_buf.st_mode)) //check if source is directory
            {
                if (S_ISDIR(destination_buf.st_mode))
                {
                    char *new_destination = append_source(source, destination);
                    change_name(source, new_destination);

                    free(new_destination);
                }
                else
                {
                    print_error("Can't overwrite non-directory with directory!", 0);
                    return;
                }
            }
            else
            {
                print_error("Only directory file or regular file is allowed!", 0);
                return;
            }
        }
    }
    else
    {
        //arg_count > 3

        //check if all the files exist
        for(int i=optind ; i<arg_count ; i++)
        {
            if(access(arg_values[i], F_OK) == -1)
            {
                print_error("access", 1);
                return;
            }
        }
        
        char *destination = arg_values[arg_count-1];
        struct stat destination_buf;

        if (stat(destination, &destination_buf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        //destination must be directory
        if(!S_ISDIR(destination_buf.st_mode))
        {
            print_error("Destination must be a directory", 0);
            return;
        }

        //scan through every file & mv it to destination (argc-1 because dont have to mv last one)
        for(int i=optind ; i<arg_count-1 ; i++)
        {
            char *new_destination = append_source(arg_values[i], destination);
            change_name(arg_values[i], new_destination);

            free(new_destination);
        }        
    }
}