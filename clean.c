#include <stdlib.h>
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

int main()
{
    int id, id2;
    int sema_set;

    FILE* ptr = fopen("sample_input_file.txt", "w+");
    struct StudentInfo *infoptr = NULL;
    struct ReadCounter *readpointer = NULL;

    if(ptr == NULL){
        printf("file cann't be opend\n");
    }

    /* get shared memory to scleartore data*/
    id = shmget(KEY, STD_SEGSIZE, 0);
    id2 = shmget(KEY, READ_SEGSIZE, 0);
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

     /* get a set of NUM_SEMAPHS semaphores*/
    sema_set = GetSemaphs(SEMA_KEY, NUM_SEMAPHS);
    if ((sema_set < 0) ){
        perror("create: semget failed");
        exit(2);
    }

    printf("\n\n-----***************------\n-----FILE SAVE BEGIN------\n-----***************------\n\n");
    for(int i = 0; i< 50; i++)
    {
        Wait(sema_set, 1);
        readpointer->readCount = readpointer->readCount+1;

        if(strcmp("", infoptr->StudentId)!=0)
        {
            fprintf(ptr, "%s%s%s%s",
                infoptr->Name,
                infoptr->StudentId,
                infoptr->address,
                infoptr->telNumber
            );

            printf("%d: %s%s%s%s \n", i, 
                infoptr->Name,
                infoptr->StudentId,
                infoptr->address,
                infoptr->telNumber
            );
        }
        sleep(0);
        Signal(sema_set, 1);
        infoptr++;
    }
    Signal(sema_set,1);
    printf("\n\n-----***************------\n-----FILE  SAVE  END------\n-----***************------\n\n");
    fclose(ptr);
    
    shmdt((char  *)infoptr); /* detach the shared memory segment */
    shmdt((char  *)readpointer); /* detach the shared memory segment */
    
    shmctl(id, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
    shmctl(id2, IPC_RMID,(struct shmid_ds *)0); /* destroy the shared memory segment*/
    
    semctl(sema_set, 0, IPC_RMID); /*Remove the semaphore set */
    exit(0);
}