#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int main(){
    int i;
   for(i=0;i<2;i++){
   		printf("pid do ex: %d\n",getpid());
   		sleep(1);
   }
    kill(getpid(),SIGCHLD);
	return 0;
} 
