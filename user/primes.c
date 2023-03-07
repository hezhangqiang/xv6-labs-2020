#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void check_primes(int recv_fd)
{
    int prime;
    read(recv_fd, &prime, 4);
    fprintf(1, "prime %d\n", prime);

    int num = 0;
    while(read(recv_fd, &num, 4) > 0)
    {
        if(num % prime != 0)
        {
            break;
        }
        else
            num = 0;
    }

    if(num == 0)
    {
        close(recv_fd);
        exit(0);
    }

    int fd[2];
    pipe(fd);

    write(fd[1], &num, 4);
    while(read(recv_fd, &num, 4) > 0)
    {
        if(num % prime != 0)
            write(fd[1], &num, 4);
    }


    if(fork() == 0)
    {
        close(fd[1]);
        close(recv_fd);
        check_primes(fd[0]);
    }
    else
    {
        close(fd[0]);
        close(recv_fd);
        close(fd[1]);
        wait(0);
    }
    exit(0);
}

int
main(int argc, char *argv[])
{
    int fd[2];

    pipe(fd);
    int i=2;
    for(i=2; i<=35; ++i)
    {
        write(fd[1], &i, 4);
    }

    if(fork() == 0)
    {
        close(fd[1]);
        check_primes(fd[0]);
//        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        close(fd[1]);
        wait(0);
    }
    exit(0);
}