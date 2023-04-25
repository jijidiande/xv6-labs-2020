#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char *argv[]){
//两个进程通过管道传递ping-pong
//父进程发送字节
//子进程收到后打印<pid>: received ping；并发送字节给父进程并退出
//父进程读字节后打印，然后退出

int fds[2];
int fds1[2];
char buf[1];
pipe(fds);//创建管道，读给fd[0],写给fd[1]
 pipe(fds1);
if(fork() == 0){//子进程
close(fds[1]);//关闭写
read(fds[0],buf,1);
close(fds[0]);
printf("%d: received ping\n",getpid());

 close(fds1[0]);
 write(fds1[1],"a",1);
 close(fds1[1]);
}else{//父进程
close(fds[0]);//关闭读
write(fds[1],"a",1);
close(fds[1]);//否则read会一直阻塞，等待新数据

close(fds1[1]);
read(fds1[0],buf,1);
close(fds1[0]);
printf("%d: received pong\n",getpid());

}
exit(0);
}