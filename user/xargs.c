#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


int
main(int argc, char *argv[])
{
    char* cmd = argv[1];
    char* args[20];

    int i;
    for(i = 1; i < argc; ++i)
    {
        args[i-1] = argv[i];
    }


    char single_char[1];
    char last_arg[20];
    i = 0 ;
    while(read(0, single_char, 1) > 0)
    {
        if(single_char[0] == '\n')
        {
            if(fork() == 0)
            {
                last_arg[i] = '\0';
                args[argc-1] = last_arg;
                args[argc] = 0;
                exec(cmd, args);
                printf("sub process failed\n");
                exit(1);
            }
            else
            {
                wait(0);
                memset(last_arg, '\0', sizeof(last_arg));
                i = 0;
            }
        }
        else
            last_arg[i++] = single_char[0];

    }

    exit(0);
}
