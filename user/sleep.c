#include "kernel/types.h"
#include "user/user.h"
int main(int argc, char *argv[]){
//sleep(int);
if(argc != 2){
	fprintf(2, "Usaging: sleep times\n");
	exit(1);
}else{
	int time;
	time = atoi(argv[1]);
	sleep(time);
	exit(0);
}

}