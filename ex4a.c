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
    char men[MAX-1],*p;
    
    seg1 = shmget(key,MAX*sizeof(char),IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
    if (seg1==-1){
        printf("Memoria compartilhada nao criada");
        exit(1);
    }
    
    p=(char*)shmat(seg1,0,0);
    if (p == (char*)-1) {
        printf("Erro no attach");
        exit(1);
    }
    printf("Mensagem do dia: ");
    scanf(" %149[^\n]", men);
    
    strcpy(p,men);
    
    if(shmdt(p)==-1){
        printf("Erro no detach");
        exit(1);
    }

    return 0;
}
