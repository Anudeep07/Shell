#include "../shell.h"

bool aflag;
bool lflag;
bool dflag;

struct file_attributes
{
    char type;                      
	char permissions[10];               //last one for \0 (simplifies print)
	int hardlink_count;
	char owner[100];
	char group[100];
	long long size;
	char last_modification_time[13];
    char *name;
};

//alphasort declared in dirent.h sorts according to ASCII
//Eg: E<a
static int alphasorti(const struct dirent **a, const struct dirent **b)
{
    return(strcasecmp((*(const struct dirent **)a)->d_name,
                    (*(const struct dirent **)b)->d_name));
}

//comparator to sort strings
static int cmpstring(const void *p1, const void *p2)
{
    return strcasecmp(*(char * const *)p1, *(char * const *)p2);
}

//Used to filter out . .. and hidden files
static int filter(const struct dirent *directory)
{
    //if aflag is false, then if directory name is . or .. or hidden (starts with .), then don't show it
    if(!aflag && ( !strcmp(directory->d_name, ".") || !strcmp(directory->d_name, "..") || directory->d_name[0] == '.' ))
        return 0;
    return 1;
}

static int get_file_details(struct file_attributes *filebuf)
{
    struct stat statbuf;

    if(lstat(filebuf->name, &statbuf) == -1)
    {
        perror("lstat");
        return -1;
    }

    __mode_t mode = statbuf.st_mode;

    //type
	if(S_ISREG(mode))
		filebuf->type = '-';
	else if(S_ISDIR(mode))
		filebuf->type = 'd';
	else if(S_ISCHR(mode))
		filebuf->type = 'c';
	else if(S_ISBLK(mode))
		filebuf->type = 'b';
	else if(S_ISFIFO(mode))
		filebuf->type = 'p';
	else if(S_ISLNK(mode))
		filebuf->type = 'l';
	else if(S_ISSOCK(mode))
		filebuf->type = 's';
	else
		filebuf->type = '@';	//unknown mode
    
    //permissions
	filebuf->permissions[0] = (mode & S_IRUSR) ? 'r' : '-';
	filebuf->permissions[1] = (mode & S_IWUSR) ? 'w' : '-';
	filebuf->permissions[2] = (mode & S_IXUSR) ? 'x' : '-';

	filebuf->permissions[3] = (mode & S_IRGRP) ? 'r' : '-';
	filebuf->permissions[4] = (mode & S_IWGRP) ? 'w' : '-';
	filebuf->permissions[5] = (mode & S_IXGRP) ? 'x' : '-';

	filebuf->permissions[6] = (mode & S_IROTH) ? 'r' : '-';
	filebuf->permissions[7] = (mode & S_IWOTH) ? 'w' : '-';
	filebuf->permissions[8] = (mode & S_IXOTH) ? 'x' : '-';

    filebuf->permissions[9] = '\0';

    //hardlink count
	filebuf->hardlink_count = statbuf.st_nlink;

    //userid
	struct passwd *user_details = getpwuid(statbuf.st_uid);
	strcpy(filebuf->owner, user_details->pw_name);

    //groupid
	struct group *group_details = getgrgid(statbuf.st_gid);
	strcpy(filebuf->group, group_details->gr_name);

    //file size
	filebuf->size = statbuf.st_size;

    //modification time

	//ctime returns 27 byte string, Eg: (one extra space between Feb & 10, 26 otherwise)
	//Tue Feb  10 18:27:38 2004 \n\0
	char tmp[30];
	strcpy(tmp, ctime(&statbuf.st_mtime));

	for(int i=4 ; i<16 ; i++)			
		filebuf->last_modification_time[i-4] = tmp[i];
	filebuf->last_modification_time[12] = '\0';

    return 0;
}

/*
 *prints name of file if lflag is false.
 *Otherwise prints all the attributes of the file
 *regular = 0 implies name is a directory file,
 *regular = 1 implies name is a regular file
 */
static void print_file(char *name, int regular)
{
    char start_color[20];

    if(regular == 1)
        strcpy(start_color, ANSI_COLOR_YELLOW);
    else if(regular == 0)
        strcpy(start_color, ANSI_COLOR_MAGENTA);
    else
        start_color[0]='\0';
    
    if(lflag)
    {
        struct file_attributes file_details;
        file_details.name = name;

        if(get_file_details(&file_details) == -1)
            return;
        
        printf("%c%s %3d %10s %10s %10lld %20s ", file_details.type, file_details.permissions, file_details.hardlink_count, file_details.owner, file_details.group, file_details.size, file_details.last_modification_time);
    }

    printf("%s%s\n" ANSI_COLOR_RESET, start_color, name);
}

static void print_directories(char *dirpath)
{
    if(dflag)
    {
        //dir_pr(dirpath);
        print_file(dirpath, 0);
        return;
    }

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
        char pathname[8192];

        snprintf(pathname, sizeof(pathname), "%s/%s", dirpath, file);

        if(stat(pathname, &statbuf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        if(S_ISDIR(statbuf.st_mode))
        {
            print_file(file, 0);
        }
        else if(S_ISREG(statbuf.st_mode))
        {
            print_file(file, 1);
        }
        else
        {
            print_file(file, -1);
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

    int index = optind ? optind : 1;    //if no options, then start from 1, else start from optind
    int file_count = optind ? arg_count - optind : arg_count - 1;       //if no options, then other than ls all are files
    struct stat statbuf;

    qsort(arg_values+index, file_count, sizeof(char *), cmpstring);

    for(int i=index ; i<arg_count ; i++)
    {
        if (stat(arg_values[i], &statbuf) == -1)
        {
            print_error("stat", 1);
            return;
        }

        printf("\n");
        if(S_ISREG(statbuf.st_mode))
        {
            //regular_pr(arg_values[i]);
            print_file(arg_values[i], 1);
        }
        else
        {
            if(file_count > 1 && !dflag)
                printf("------------------\n%s:\n------------------\n", arg_values[i]);

            print_directories(arg_values[i]);
        }
    }
}