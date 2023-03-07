#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];
    int pid;

    pipe(p1);
    pipe(p2);

    if(fork() == 0)
    {
        close(p1[1]);
        close(p2[0]);

        read(p1[0], 0, 1);
        close(p1[0]);
        pid = getpid();
        fprintf(2, "%d: received ping\n", pid);
        write(p2[1], " ", 1);
        close(p2[1]);
    }
    else
    {
        close(p1[0]);
        close(p2[1]);

        write(p1[1], " ", 1);
        close(p1[1]);
        read(p2[0], 0, 1);
        pid = getpid();
        fprintf(2, "%d: received pong\n", pid);
        close(p2[0]);
    }
    exit(0);
}