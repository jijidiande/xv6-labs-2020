#include "kernel/types.h"
#include "user/user.h"


void subprocess(int *oldfd){
int fds[2];
int i;
pipe(fds);
close(oldfd[1]);
if(read(oldfd[0],&i,4)==0){//读不到数据了
close(oldfd[0]);
exit(0);
}
printf("prime %d\n",i);
int num;
if(fork()==0){
subprocess(fds);
}else{
	close(fds[0]);
	while(read(oldfd[0],&num,4)>0){
        if(num%i!=0){
write(fds[1],&num,4);
        }
	}
	close(fds[1]);
	wait(0);
}
exit(0);
}


int main(){
int fd[2];
pipe(fd);
if(fork()==0){
	subprocess(fd);
}else{
//开始写
close(fd[0]);
for(int i=2;i<36;i++){
	write(fd[1],&i,4);
}
close(fd[1]);
wait(0);
}
exit(0);
}
