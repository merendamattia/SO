#include <sys/shm.h> // shmget(), shmat(), shmdt()
#include "../utilities/utility.c"
  
sem_t mutex;
int total_sum = 0;

// Crea una chiave per il segmento di memoria condivisa
key_t key; 
 
void makeSum(int vet[], int start, int finish, int shmid){
	int internal_sum = 0;

	for(int i = start; i < finish; i++)
		internal_sum += vet[i];

	//printf("\tpid: %d\t ppid: %d\t internal_sum: %d\t\tfrom %d to %d\n", getpid(), getppid(), internal_sum, start, finish);
	
	sem_wait(&mutex);
	//printf("\npid: %d entrato nella zona critica\n", getpid());

	int *data = shmat(shmid, (void*)0, 0);
	
	// Esegue una somma e salva il risultato nel segmento di memoria condivisa
    *data += internal_sum;

    // Stacca il segmento di memoria condivisa dal processo
    shmdt(data);

	//printf("pid: %d rilascia la zona critica\n\n", getpid());
	sem_post(&mutex);
}

int main() {
	printf("Merenda Saverio Mattia [MAT: 330503]\n");
	printf("Processes (using shared memory)\n\n");
		
	int status;

    sem_init(&mutex, 0, 1);

    key = ftok("./", 'S');
        
    // Ottieni un segmento di memoria condivisa con dimensione di 20 byte
    int shmid = shmget(key, 20, 0644 | IPC_CREAT);
    int *p_sum = shmat(shmid, (void*)0, 0);
	*p_sum = 0;

   	int dim_array = getDimArray(); // Input dimensione array
	int dim_process = getNumProcess(); // Input numero processi

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	if (!vet) { 
		printf("Errore nell'allocazione della memoria :(\n");
		return -1;
	}

	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");

	struct timespec start_array_population_time, start_exec_time_main, start_exec_time_processes;
	
	clock_gettime(CLOCK_REALTIME, &start_array_population_time);
	populateArrayRandom(1, dim_array, vet); 
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));
	
	//outputArray(vet, dim_array);

	int modulo = dim_array % dim_process;
	int quoziente = getQuoziente(modulo, dim_array, dim_process);

	printf("\n\n***** LOADING *****\n\n");

	clock_gettime(CLOCK_REALTIME, &start_exec_time_processes);

    //printf("\nStart processes...\n");
    for(int i = 0; i < dim_process; i++) {
        pid_t pid = fork();

        if (pid==0) { 
        	// Solo i processi figli eseguono questo blocco di codice
            
           	int start = getStartIndex(i, quoziente);
            int finish = getFinishIndex(i, quoziente, dim_process, dim_array);
                
            makeSum(vet, start, finish, shmid);

            exit(0);
        }
        // Solo il processo padre attende
        wait(&status);
    }
    //printf("...Finish processes\n");

    p_sum = shmat(shmid, (void*)0, 0);
    double total_exec_time_processes = getTimeTaken(start_exec_time_processes);
   
    //printf("\nStart main...\n");

	clock_gettime(CLOCK_REALTIME, &start_exec_time_main);
	int m_sum = sumInMain(1, dim_array, vet);
	double time_exec_main = getTimeTaken(start_exec_time_main);

	//printf("...Finish main\n");

    // ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");

	printf("SUM: \t%d\n", m_sum);
	printf("Execution time %f sec \n", time_exec_main);

	printf("\nPROCESSES\n");
	printf("SUM: \t%d\n", *p_sum);
	printf("Execution time %f sec \n", total_exec_time_processes);

	// Deallocazione memoria dinamica utilizzata
	free(vet);
	// Distruzione del semaforo
	sem_destroy(&mutex);

    return 0;
}

// Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 