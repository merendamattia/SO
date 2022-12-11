#include "./utility.c"

int total_sum = 0;
  
// Pipe
int pipefd[2];

void makeSum(int vet[], int start, int finish){
	int internal_sum = 0;

	for(int i = start; i < finish; i++)
		internal_sum += vet[i];

	//printf("\tpid: %d\t ppid: %d\t internal_sum: %d\t\tfrom %d to %d\n", getpid(), getppid(), internal_sum, start, finish);

	close(pipefd[0]);
	
    write(pipefd[1], &internal_sum, sizeof(internal_sum));

    close(pipefd[1]);
}

int main() {
	printf("Merenda Saverio Mattia [MAT: 330503]\n");
	printf("Processes (using pipe)\n\n");
		
	int sum = 0;

    // Creo la pipe
    if (pipe(pipefd) == -1){
		printf("Errore nella creazione della pipe :(\n");
		return -1;
	}

   	int dim_array = getDimArray(); // Input dimensione array
	int dim_process = getNumProcess(); // Input numero processi

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	if (!vet) { 
		printf("Errore nell'allocazione della memoria :(\n");
		return -1;
	}

	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");
	
	clock_t start_array_population_time = clock(); // Start cronometro
	populateArrayRandom(vet, dim_array); 
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));
	
	//outputArray(vet, dim_array);

	int modulo = dim_array % dim_process;
	int quoziente = getQuoziente(modulo, dim_array, dim_process);

	printf("\n\n***** LOADING *****\n\n");

    clock_t start_exec_time_processes = clock(); 

    // printf("\nStart processes...\n");
    for(int i = 0; i < dim_process; i++) {
        pid_t pid = fork();

        if (pid==0) { 
        	// Solo i processi figli eseguono questo blocco di codice
            
           	int start = getStartIndex(i, quoziente);
            int finish = getFinishIndex(i, quoziente, dim_process, dim_array);
                
            makeSum(vet, start, finish);

            exit(0);
        }
    }

    close(pipefd[1]);

    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) != -1) {
        int tmp_sum;

        if (read(pipefd[0], &tmp_sum, sizeof(tmp_sum)) != sizeof(tmp_sum)) {
            fprintf(stderr, "Parent process got a short read on the pipe\n");
            exit(EXIT_FAILURE);
        }
      
        sum += tmp_sum;
    }

    //printf("...Finish processes\n");
  
    double total_exec_time_processes = getTimeTaken(start_exec_time_processes);
   
    //printf("\nStart main...\n");

	clock_t start_exec_time_main = clock();
	int m_sum = sumInMain(vet, dim_array);
	double time_exec_main = getTimeTaken(start_exec_time_main);

	//printf("...Finish main\n");

    // ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");

	printf("SUM: \t%d\n", m_sum);
	printf("Execution time %f sec \n", time_exec_main);

	printf("\nPROCESSES\n");
	printf("SUM: \t%d\n", sum);
	printf("Execution time %f sec \n", total_exec_time_processes);

	free(vet);

    return 0;
}