#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <wait.h>
#define MAX 5
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
};
// inicializa o valor do semáforo
int setSemValue(int semId);
// remove o semáforo
void delSemValue(int semId);
// operação P
int semaforoP(int semId);
//operação V
int semaforoV(int semId);



int main(){
    int seg, pid,semId,i;
    int *a;
    
    seg=shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    
    if (seg == -1) {
        printf("Erro na criacao da memoria compartilhada");
        exit(1);
    }
    
    a=(int*) shmat(seg,0,0);
    *a=0;
    
    semId = semget (IPC_PRIVATE, 1, 0666 | IPC_CREAT|IPC_EXCL);
    setSemValue(semId);
    if ((pid = fork()) < 0) { /* criar processo */
        printf("Impossivel criar processo");
        exit(1);
    }
    
    /* PAI -> soma 5*/
    else if(pid>0){
        for (i=0;i<MAX;i++){
            semaforoP(semId);
            (*a)+=5;
            printf("5:  a=%d\n",*a);
            fflush(stdout);
            semaforoV(semId);   
        }
    }
    
    /*FILHO -> soma 1*/
    else{
        for (i=0;i<MAX;i++){
            semaforoP(semId);
            (*a)++;
            printf("1:  a=%d\n",*a);
            fflush(stdout);
            semaforoV(semId);
            
        }
        if (shmdt(a) == -1) {
		    printf("Erro no detach \n");
		    exit(1);
		}
    }
    
    wait(NULL);
    
    if (shmdt(a) == -1) {
        printf("Erro no detach \n");
        exit(1);
    }
    
    if (shmctl(seg, IPC_RMID, NULL) == -1) {
        printf("Erro ao liberar memoria compartilhada\n");
    }
    
    delSemValue(semId);
    
    return 0;
}
int setSemValue(int semId)
{
    union semun semUnion;
    semUnion.val = 1;
    return semctl(semId, 0, SETVAL, semUnion);
}

void delSemValue(int semId)
{
    union semun semUnion;
    semctl(semId, 0, IPC_RMID, semUnion);
}

int semaforoP(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = -1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}
int semaforoV(int semId)
{
    struct sembuf semB;
    semB.sem_num = 0;
    semB.sem_op = 1;
    semB.sem_flg = SEM_UNDO;
    semop(semId, &semB, 1);
    return 0;
}

