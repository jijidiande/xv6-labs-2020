#include "kernel/types.h"
#include "user/user.h"

void subprocess(int oldfd[]){
int i;
close(oldfd[1]);
if(read(oldfd[0],&i,sizeof(i))==0){
	close(oldfd[0]);
	exit(0);
}
printf("prime %d\n",i);
int num = 0;

int fds[2];
pipe(fds);

if(fork()==0){
	subprocess(fds);
}
else{
	close(fds[0]);
	while(read(oldfd[0],&num,sizeof(num)) > 0){
		if(num % i!=0){
			write(fds[1],&num,sizeof(num));
		}
	}
    close(fds[1]);
	wait(0);
}
exit(0);
}
int main(int argc, char *argv[]){
int fd[2];
pipe(fd);

if(fork()==0){
	subprocess(fd);
}else{
close(fd[0]);
for(int i=2;i<36;i++){
write(fd[1],&i,sizeof(i));
}
close(fd[1]);
wait(0);
}
exit(0);
}