#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <string.h>
#define MAX 150
#define key 9005
#define key2 8889

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



int main()
{
    int seg1,i,semId;
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
    
	semId = semget (key2, 1, 0666);
    setSemValue(semId);


	for (i=0;i<10;i++){
		//sleep(3);
		semaforoP(semId);
		printf("%s",p);
		printf("\n");
		semaforoV(semId);
    }
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

