/*
B. Akshay Kamath ^_^
*/

#include "../shell.h"

void concat(FILE * fp);
int n,E,s;
void cat()
{
    int opt;
    n = s = E = 0;
    while((opt = getopt(arg_count, arg_values, "nEs")) != -1)
    {

        switch(opt)
        {
        case 'n':
            printf("option n\n");
            n = 1;
            break;
        case 'E':
            printf("option E\n");
            E = 1;
            break;
        case 's':
            printf("option s\n");
            s = 1;
            break;
        default:
            printf("Default: %c , char = %c\n", opt, optopt);
        }
    }
    if(arg_count == optind)
        concat(stdin);

    FILE *fp;
    for(int i=optind ; i<arg_count ; i++){
        printf("Non-option arg = %s\n", arg_values[i]);
        if((fp = fopen(arg_values[i],"r")) == NULL)
            printf("cat : %s: No such file\n",arg_values[i]);
        else{
            concat(fp);
        }
    }
}

void concat(FILE * fp)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    int no = 1;
            while ((nread = getline(&line, &len, fp)) != -1) {
                int len = strlen(line);
                line[len-1] = '\0';
                if(n == 1)
                    printf("\t%d\t", no);
                printf("%s%c\n",line,(E == 1)?'$':'\0');
                no++;
            }
}