#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

//echo hello too | xargs echo bye
int main(int argc,char *argv[]){
char *cmd = argv[1];

int args_index=0;
char* params[MAXARG];//新参数
for(int i=1;i<argc;i++){
//argv[0] echo
//argv[1] bye
//argc = 2
params[args_index++] = argv[i];//原参数放到新参数
}
char line[1024];//上一条命令
int n = read(0,line,1024);
if(fork()==0){//每次都是fork子程序来执行命令
char *temp = (char*) malloc(sizeof(line));//temp是一个字符串
int index = 0;
for(int i=0;i<n;i++){
	if(line[i] == ' ' || line[i] == '\n'){
params[args_index++] = temp;
index = 0;
temp = (char*) malloc(sizeof(line));
	}else{
		temp[index++] = line[i];//临时变量
	}
}
exec(cmd,params);
	}else{
		wait(0);
	}
exit(0);
}