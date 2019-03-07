#include "../shell.h"

#define SAME_INODE(a, b) ((a).st_ino == (b).st_ino && (a).st_dev == (b).st_dev)

void change_name(char *source, char *destination)
{
    if (rename(source, destination) == -1)
        print_error("Unable to rename. Please check permissions & try again!");
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
    
    int opt;
    int i_flag = 0;

    /*    
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

            //check if both the file are not same
            if (SAME_INODE(source_buf, destination_buf))
            {
                print_error("Source and destination files can't be same.");
                return;
            }

            //check if source is regular file
            if (S_ISREG(source_buf.st_mode))
            {
                if (S_ISREG(destination_buf.st_mode))
                {
                    /*
                    if(i_flag)
                    {
                        char choice;
                        printf("Do you want to overwrite %s? (y/Y to overwrite) ", destination);
                        scanf("%c", &choice);

                        //if choice is other than y or Y, return
                        if(choice != 'y' && choice != 'Y')
                            return;                        
                    }*/
                    change_name(source, destination);
                }
                else if (S_ISDIR(destination_buf.st_mode))
                {
                    //XDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
                    char *new_source = last_component(source);
                    char new_destination[4096];
                    
                    snprintf(new_destination, sizeof(new_destination), "%s/%s", destination, new_source);
                    change_name(source, new_destination);

                    free(new_source);
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
                    //XDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
                    char *new_source = last_component(source);
                    char new_destination[4096];

                    snprintf(new_destination, sizeof(new_destination), "%s/%s", destination, new_source);
                    change_name(source, new_destination);

                    free(new_source);
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