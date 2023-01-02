#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../utility.c"

sem_t mutex;
int total_sum = 0;

// Crea una chiave per il segmento di memoria condivisa
key_t key;

int foo(const char *whoami) {
    printf("\nI am a %s.\tMy pid is:%d\tmy ppid is %d\n\n", whoami, getpid(), getppid() );
    return 1;
}

void makeSum(int vet[], int start, int finish, int shmid){
	int internal_sum = 0;

	

	for(int i = start; i < finish; i++)
		internal_sum += vet[i];

	printf("\npid: %d\t ppid: %d\t internal_sum: %d\tfrom %d to %d\n", getpid(), getppid(), internal_sum, start, finish);
	

	sem_wait(&mutex);
	printf("\npid: %d entrato nella zona critica\n", getpid());
	//total_sum += internal_sum;
	int *data = shmat(shmid, (void*)0, 0);
	

	// Esegue una somma e salva il risultato nel segmento di memoria condivisa
    *data += internal_sum;

    printf("total sum so far: %d \n", *data);

    // Stacca il segmento di memoria condivisa dal processo
    shmdt(data);

	printf("pid: %d rilascia la zona critica\n\n", getpid());
	sem_post(&mutex);
	
	//return 1;
}

int main() {
		int status;

        sem_init(&mutex, 0, 1);

        key = ftok("/tmp", 'S');
        
        // Ottieni un segmento di memoria condivisa con dimensione di 20 byte
        int shmid = shmget(key, 20, 0644 | IPC_CREAT);
        int *total_sum = shmat(shmid, (void*)0, 0);
	    *total_sum = 0;

        int dim_array = getDimArray(); // Input dimensione array
		int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
		
		populateArrayRandom(vet, dim_array); // Popolamento array con valori casuali che vanno da 1 a MAX_LIMIT + 1
		outputArray(vet, dim_array);

		int dim_process = getNumProcess(); // Input numero thread
		

		int modulo = dim_array % dim_process;
		int quoziente;
		
		if(modulo == 0)
			quoziente = dim_array / dim_process;
		else 
			quoziente = dim_array / (dim_process - 1); // è più ottimizzato


        printf("Creating %d children\n", dim_process);
        foo("parent");

        for(int i = 0; i < dim_process; i++) {
            pid_t pid = fork();

            if (pid==0) { /* only execute this if child */
            	int start, finish;

				if(i == 0) {
					start = 0;
					finish = quoziente;
				} else if(i + 1 == dim_process) {
					start = quoziente * i;
					finish = dim_array;
				} else {
					start = quoziente * i;
					finish = quoziente * (i + 1);
				}
                
                //foo("child");
                makeSum(vet, start, finish, shmid);

                exit(0);

            }
            wait(&status);  /* only the parent waits */
        }

        total_sum = shmat(shmid, (void*)0, 0);
        printf("Processes sum: %d\n", *total_sum);

        printf("Main sum: %d\n", sumInMain(vet, dim_array));

        return 0;
}