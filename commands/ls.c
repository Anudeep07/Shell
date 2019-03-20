#include "../shell.h"

#define regular_pr(name) printf(ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, name);
#define dir_pr(name) printf(ANSI_COLOR_MAGENTA "%s\n" ANSI_COLOR_RESET, name);

bool aflag;
bool lflag;
bool dflag;

//alphasort declared in dirent.h sorts according to ASCII
//Eg: E<a
int alphasorti(const struct dirent **a, const struct dirent **b)
{
    return(strcasecmp((*(const struct dirent **)a)->d_name,
                    (*(const struct dirent **)b)->d_name));
}

//Used to filter out . .. and hidden files
int filter(const struct dirent *directory)
{
    //if directory name is . or .. or hidden (starts with .), then don't show it
    if(!aflag && ( !strcmp(directory->d_name, ".") || !strcmp(directory->d_name, "..") || directory->d_name[0] == '.' ))
        return 0;
    return 1;
}

void print_directories(char *dirpath)
{
    int n;
    struct dirent **namelist;

    n = scandir(dirpath, &namelist, filter, alphasorti);
    if(n == -1)
    {
        print_error("scandir", 1);
        return;
    }

    struct stat statbuf;

    for(int i=0 ; i<n ; i++)
    {
        char *file = namelist[i]->d_name;
        if(stat(file, &statbuf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        if(S_ISDIR(statbuf.st_mode))
        {
            dir_pr(file);
        }
        else if(S_ISREG(statbuf.st_mode))
        {
            regular_pr(file);
        }
        else
        {
            printf("%s\n", file);
        }        

        free(namelist[i]);
    }

    free(namelist);
}

void ls()
{
    int opt;
    char err[25];
    aflag = lflag = dflag = false; 

    while((opt = getopt(arg_count, arg_values, "adl")) != -1)
    {
        switch (opt)
        {
            case 'a':
                aflag = true;
                break;

            case 'd':
                dflag = true;
                break;

            case 'l':
                lflag = true;
                break;
            
            default:
                snprintf(err, sizeof(err), "invalid option -- '%c'", optopt);
                print_error(err, 0);
                return;
        }
    }

    //no filenames (only ls and options)
    if(optind == arg_count)
    {
        print_directories(".");
        return;
    }

    int index = optind ? optind : 1;    //if no options, then start from 1 else start from optind
    struct stat statbuf;


    for(int i=index ; i<arg_count ; i++)
    {
        if (stat(arg_values[i], &statbuf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        if(S_ISREG(statbuf.st_mode))
        {
            regular_pr(arg_values[i]);
        }
        else
            print_directories(arg_values[i]);
    }
}