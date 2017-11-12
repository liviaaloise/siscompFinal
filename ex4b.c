
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <string.h>
#define MAX 150
#define key 8752

int main()
{
    int seg1;
    char *p;
    
    seg1 = shmget(key,MAX*sizeof(char), S_IRUSR | S_IWUSR);
    if (seg1==-1){
        printf("Memoria compartilhada inacessivel\n");
        exit(1);
    }
    
    p=(char*)shmat(seg1,0,0);
    if (p == (char*)-1) {
        printf("Erro no attach\n");
        exit(1);
    }
    printf("%s",p);
    printf("\n");
    
    if(shmdt(p)==-1){
        printf("Erro no detach\n");
        exit(1);
    }
    
    if(shmctl(seg1,IPC_RMID,NULL)==-1){
        printf("Nao foi possivel destruir a memoria\n");
        exit(1);
    }
 
    return 0;
}
