#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sem.h>
#include <pwd.h>
#include "header.h"

int main(int argc, char* argv[])
{
    int id, id2;
    int sema_set;

    if(argc!=2||strlen(argv[1])!=9)
    {
        fprintf(stderr, "usage: query <StudentID>\n");
        exit(3);
    }

    struct StudentInfo *infoptr  = NULL;
    struct ReadCounter *readpointer = NULL;

    /* get shared memory to store data*/
    id = shmget(KEY, STD_SEGSIZE, IPC_CREAT|0666);
    id2 = shmget(KEY, READ_SEGSIZE, IPC_CREAT|0666);
    if (id < 0 || id2 < 0) {
        perror("create: shmget failed");
        exit(1);
    }

    /*attach the shared memory segment to the process's address space */
    infoptr = (struct StudentInfo *)shmat(id ,0, 0); 
    readpointer = (struct ReadCounter *)shmat(id2, 0, 0);
    if (infoptr <= (struct StudentInfo *) (0) || readpointer <= (struct ReadCounter *) (0)) {
        perror("create: shmat failed");
        exit(2);
    }

    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS); /* get a set of NUM_SEMAPHS semaphores*/
    if ((sema_set < 0) ){
        perror("create: semget failed");
        exit(2);
    }

    printf("\n\n-----***************-------\n-----SHM QUERY  BEGIN------\n-----***************------\n\n");
    bool matchFound = false;
    sleep(5);
    for(int i=0; i<=50; i++) {
        Wait(sema_set, 1); 
        readpointer->readCount = readpointer->readCount + 1;

        if (strncmp(argv[1], infoptr->StudentId, 9) == 0) {
            matchFound = true;
            printf("%d: %s%s%s%s \n", i, 
                infoptr->Name,
                infoptr->StudentId,
                infoptr->address,
                infoptr->telNumber
            );
            sleep(0);
            Signal(sema_set,1);
            break;
        }       
        Signal(sema_set,1);
        infoptr++;
    }
    if (!matchFound) {
        printf("\n ***No matching records found!");
        Signal(sema_set,1);
    }
    printf("\n\n-----***************-------\n-----SHM  QUERY   END------\n-----***************-------\n\n");
    exit(0);
}