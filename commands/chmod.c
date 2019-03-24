#include "../shell.h"

#define regular_permissions 0644     //rw-r--r--
#define directory_permissions 0755   //rwxr-xr-x

int octal_to_decimal(int val);

int permission;
int m_flag;

/*
 * chmod files => changes permission to the default value
 * chmod -m o1o2o3 files => changes permission to o1o2o3
 */
void change_permission()
{
    if(arg_count == 1)
    {
        print_error("Usage: chmod [-m o1o2o3] file(s)", 0);
        return;
    }

    int opt;
    m_flag = false;   
    char err[25];
    
    while((opt = getopt(arg_count, arg_values, "m:")) != -1)
    {
        switch (opt)
        {
            case 'm':                
                m_flag = true;                
                permission = octal_to_decimal(atoi(optarg));            //if invalid optarg, then considered as 0
                break;
            default:
                snprintf(err, sizeof(err), "invalid option -- '%c'", optopt);
                print_error(err, 0);
                return;
        }
    }

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than chmod all are files

    if(file_count < 1)
    {
        print_error("Usage: chmod [-m o1o2o3] file(s)", 0);
        return;
    }    

    for(int i=optind ; i<arg_count ; i++)
    {
        //if options are present, then no need to check the type of file
        if(m_flag)
        {
            if(chmod(arg_values[i], permission) == -1)
            {
                print_error("chmod", 1);
                continue;
            }
        }
        else
        {
            //check if file is dir or regular
            struct stat statbuf;
            if(stat(arg_values[i], &statbuf) == -1)
            {
                print_error("stat", 1);
                continue;
            }

            if(S_ISDIR(statbuf.st_mode))
                permission = directory_permissions;
            else 
                permission = regular_permissions;
            
            if(chmod(arg_values[i], permission) == -1)
            {
                print_error("chmod", 1);
                continue;
            }
        } 
    }
}