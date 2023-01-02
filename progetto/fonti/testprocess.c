#include <stdio.h>          /* printf()                 */
#include <stdlib.h>         /* exit(), malloc(), free() */
#include <sys/types.h>      /* key_t, sem_t, pid_t      */
#include <sys/shm.h>        /* shmat(), IPC_RMID        */
#include <errno.h>          /* errno, ECHILD            */
#include <semaphore.h>      /* sem_open(), sem_destroy(), sem_wait().. */
#include <fcntl.h>          /* O_CREAT, O_EXEC          */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>


// int main (int argc, char **argv){
//     int i;                        /*      loop variables          */
//     key_t shmkey;                 /*      shared memory key       */
//     int shmid;                    /*      shared memory id        */
//     sem_t *sem;                   /*      synch semaphore         *//*shared */
//     pid_t pid;                    /*      fork pid                */
//     int *p;                       /*      shared variable         *//*shared */
//     unsigned int n;               /*      fork count              */
//     unsigned int value;           /*      semaphore value         */

//     /* initialize a shared variable in shared memory */
//     shmkey = ftok ("/dev/null", 5);       /* valid directory name and a number */
//     printf ("shmkey for p = %d\n", shmkey);
//     shmid = shmget (shmkey, sizeof (int), 0644 | IPC_CREAT);
//     if (shmid < 0){                           /* shared memory error check */
//         perror ("shmget\n");
//         exit (1);
//     }

//     p = (int *) shmat (shmid, NULL, 0);   /* attach p to shared memory */
//     *p = 0;
//     printf ("p=%d is allocated in shared memory.\n\n", *p);

//     /********************************************************/

//     printf ("How many children do you want to fork?\n");
//     printf ("Fork count: ");
//     scanf ("%u", &n);

//     printf ("What do you want the semaphore value to be?\n");
//     printf ("Semaphore value: ");
//     scanf ("%u", &value);

//     /* initialize semaphores for shared processes */
//     sem = sem_open ("pSem", O_CREAT | O_EXCL, 0644, value); 
//     /* name of semaphore is "pSem", semaphore is reached using this name */

//     printf ("semaphores initialized.\n\n");


//     /* fork child processes */
//     for (i = 0; i < n; i++){
//         pid = fork ();
//         if (pid < 0) {
//         /* check for error      */
//             sem_unlink ("pSem");   
//             sem_close(sem);  
//             /* unlink prevents the semaphore existing forever */
//             /* if a crash occurs during the execution         */
//             printf ("Fork error.\n");
//         }
//         else if (pid == 0)
//             break;                  /* child processes */
//     }

//     /******************************************************/
//     /******************   PARENT PROCESS   ****************/
//     /******************************************************/
//     if (pid != 0){
//         /* wait for all children to exit */
//         while (pid = waitpid (-1, NULL, 0)){
//             if (errno == ECHILD)
//                 break;
//         }

//         printf ("\nParent: All children have exited.\n");

//         /* shared memory detach */
//         shmdt (p);
//         shmctl (shmid, IPC_RMID, 0);

//         /* cleanup semaphores */
//         sem_unlink ("pSem");   
//         sem_close(sem);  
//         /* unlink prevents the semaphore existing forever */
//         /* if a crash occurs during the execution         */
//         exit (0);
//     }

//     /******************************************************/
//     /******************   CHILD PROCESS   *****************/
//     /******************************************************/
//     else{
//         sem_wait (sem);           /* P operation */
//         printf ("  Child(%d) is in critical section.\n", i);
//         //sleep (1);
//         *p += i % 3;              /* increment *p by 0, 1 or 2 based on i */
//         printf ("  Child(%d) new value of *p=%d.\n", i, *p);
//         sem_post (sem);           /* V operation */
//         exit (0);
//     }
// }


#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

const char *semName = "asdfsd";

void parent(void){
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);

    if (sem_id == SEM_FAILED){
        perror("Parent  : [sem_open] Failed\n"); return;
    }

    printf("Parent  : Wait for Child to Print\n");
    if (sem_wait(sem_id) < 0)
        printf("Parent  : [sem_wait] Failed\n");
    printf("Parent  : Child Printed! \n");
    
    if (sem_close(sem_id) != 0){
        perror("Parent  : [sem_close] Failed\n"); return;
    }

    if (sem_unlink(semName) < 0){
        printf("Parent  : [sem_unlink] Failed\n"); return;
    }
}

void child(void)
{
    sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);

    if (sem_id == SEM_FAILED){
        perror("Child   : [sem_open] Failed\n"); return;        
    }

    printf("Child %d : I am done! Release Semaphore\n", getpid());
    if (sem_post(sem_id) < 0)
        printf("Child   : [sem_post] Failed \n");
}

int main(int argc, char *argv[])
{
    pid_t pid[5];

    for(int i = 0; i < 5; i++) {

        if((pid[i] = fork()) == 0)
        
        if (pid[i] < 0){
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (!pid[i]){
            child();
            printf("Child %d: Done with sem_open \n", getpid());
        }
        else{
            int status;
            parent();
            wait(&status);
            printf("Parent  : Done with sem_open \n");
        }

    }

    

    return 0;
}
