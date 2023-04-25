#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char *argv[]){
// ''ping-pong'' a byte between two processes over a pair of pipes
//The parent should send a byte to the child
//the child should print "<pid>: received ping",
//write the byte on the pipe to the parent, and exit

//the parent should read the byte from the child,
//print "<pid>: received pong", and exit.

//fd[0] → r； fd[1] → w
//pipefd1[2]父进程写子进程读
//pipefd2[2]父进程读子进程写
int pipefd1[2],pipefd2[2];
pipe(pipefd1);
pipe(pipefd2);
char buf[1];
if (fork() == 0){
//子进程
close(pipefd1[1]);//子关写只读
read(pipefd1[0],buf,1);
close(pipefd1[0]);//子关写关读
printf("%d: received ping\n",getpid());

close(pipefd2[0]);
write(pipefd2[1],buf,1);
close(pipefd2[1]);

}else{
//父进程
close(pipefd1[0]);//父关读只写
write(pipefd1[1],"a",1);
close(pipefd1[1]);//父关读关写

close(pipefd2[1]);
read(pipefd2[0],buf,1);
close(pipefd2[0]);
printf("%d: received pong\n",getpid());

}
exit(0);
  }