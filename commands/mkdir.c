#include "../shell.h"

#define PERMISSION 0755 //RWX for user, RX for group & others

bool m_flag;    
int permission = PERMISSION;       //used to store the permissions if m_flag is set   

static int octal_to_decimal(int val)
{
    int dec=0;
    for(int i=0 ; val != 0 ; i++)
    {
        int digit = val%10;
        dec += digit * pow(8, i);
        val /= 10;       
    }
    return dec;
}

void makedir()
{
    if(arg_count == 1)
    {
        print_error("Usage: mkdir [-m permissions] <file(s)>", 0);
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

    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than mkdir all are files

    if(file_count < 1)
    {
        print_error("Usage: mkdir [-m permissions] <file(s)>", 0);
        return;
    }

    for(int i=optind ; i<arg_count ; i++)
    {
        //check if file exists
        if(access(arg_values[i], F_OK) == 0)
        {
            char message[30];
            snprintf(message, sizeof(message), "%s already exists.", arg_values[i]);
            print_error(message, 0);
            return;
        }

        if(mkdir(arg_values[i], permission) == -1)
        {
            print_error("mkdir", 1);
            continue;
        }
    }
}