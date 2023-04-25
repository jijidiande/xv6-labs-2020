#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include <stddef.h>

int subProcess(int *oldFd){
    close(oldFd[1]);
    int fd[2];
    int prime;
    int num;
    if(read(oldFd[0],&prime,4)){
        printf("prime %d\n",prime);
        pipe(fd);
        if(fork()==0){
            subProcess(fd);
        }else{
            close(fd[0]);
            while(read(oldFd[0],&num,4)){
                if(num%prime != 0){
                    write(fd[1],&num,4);
                }
            }
            close(oldFd[0]);
            close(fd[1]);
            // wait((int *)0);
            wait(NULL);
        }
    }else{
        close(oldFd[0]);
    }
    exit(0);
}

int main(int argc,char *argv[]){
//concurrent version of prime sieve using pipes 质数筛选器
//use pipe and fork to set up the pipeline.
//子进程全部结束 第一个进程才能结束

    int i;
    int fd[2];
    pipe(fd);
    if(fork()==0){
        subProcess(fd);
    }else{
        close(fd[0]);
        for(i=2;i<=35;i++){
            write(fd[1],&i,4);
        }
        close(fd[1]);
        //父进程并不关心子进程的状态，只需要等待子进程退出即可
        // wait((int *) 0);
        wait(NULL);
    }
exit(0);
}