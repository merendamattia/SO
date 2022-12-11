#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/*!
\mainpage Esempio memoria condivisa
\section intro Introduzione
Esempio di gestione segmenti di memoria condivisa\n

\date 04/05/2006
\version   0.0.0.1
\author Alessandro Dal Palu

*/


#define SIZE 10

int main() {
    int shmid;
    int att, pid, siz;
  
    char* memoria;
    char s[SIZE];

    key_t key;

    key = ftok(".", 'a'); /*crea chiave*/

    shmid = shmget(key, sizeof(char)*SIZE, IPC_CREAT | IPC_EXCL | 0600 );

    if (shmid == -1) {
        perror("Segmento shared memory gia' esistente");
        exit(1);
    }

    memoria = shmat(shmid, NULL, 0);

    if (memoria == NULL) {
        perror("shmat"); 
        exit(2);
    }

    switch(pid = fork()) {
        case -1:
            perror("fork");
            exit(2);
        case 0: /* Figlio: eredita ID di shared memory */
            fprintf(stderr, "Figlio: entra nel ciclo \n");
            for (;;) {
                sleep(1);
                if (memoria[0] >0) {

                    printf("Figlio legge %d bytes\nContenuto: ",memoria[0]);
                    if (memoria[0]==SIZE)
                        memoria[SIZE]='\0';
                    fputs(memoria+1,stdout);
                    fputs("\n",stdout);

                    if (shmdt(memoria) == -1)
                        perror("Figlio: detach");

                    exit(0);
                }
            }
        default: /* Padre: produce */

            fgets(s, SIZE, stdin); 
            strncpy(memoria+1, s, SIZE-1); 
            if (strlen(s) < SIZE) 
                memoria[0] = strlen(s)+1;
            else 
                memoria[0] = SIZE;
    }
    
    pid = wait(&att);
    printf("Padre: terminato figlio. Rimuove shm\n");
    
    if (shmdt(memoria) == -1) 
      perror("Padre: detach");
    
    if (shmctl(shmid, IPC_RMID, 0) == -1)
      perror("Padre: rimozione segmento");
    
    printf("Padre: finito\n");
    
    exit(0);

}
