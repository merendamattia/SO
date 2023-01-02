// Pipe
int pipefd[2];
sem_t *mutex;

void makeSum_pipe(int num_righe, int start, int finish, int* vet){
	int internal_sum = 0;

	for(int j = 0; j < num_righe; j++)
		for(int i = start; i < finish; i++)
			internal_sum += vet[i];

	sem_wait(mutex);
	close(pipefd[0]);
    write(pipefd[1], &internal_sum, sizeof(internal_sum));
    close(pipefd[1]);
	sem_post(mutex);
}

void processes() {
	printf("\nProcesses\n\n");

	int sum = 0;
	int num_righe = 1;

    // Creo la pipe
    if (pipe(pipefd) == -1){
		printf("Errore nella creazione della pipe :(\n");
		exit(EXIT_FAILURE);
	}

	mutex = (sem_t *) malloc(sizeof(sem_t));

	if(sem_init(mutex, 1, 1)){ 
		printf("Errore nella creazione del semaforo :(\n");
		free(mutex);
		exit(EXIT_FAILURE);
	}

   	int dim_array = getDimArray(); // Input dimensione array
	int dim_process = getNumProcess(); // Input numero processi

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	if (!vet) { 
		printf("Errore nell'allocazione della memoria :(\n");
		exit(EXIT_FAILURE);
	}

	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");
	
	struct timespec start_array_population_time, start_exec_time_main, start_exec_time_processes;
	
	clock_gettime(CLOCK_REALTIME, &start_array_population_time); // Start cronometro
	populateArrayRandom(num_righe, dim_array, vet); 
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));

	int modulo = dim_array % dim_process;
	int quoziente = getQuoziente(modulo, dim_array, dim_process);

	printf("\n\n***** LOADING *****\n\n");

	// ----------------------------- PIPE
	clock_gettime(CLOCK_REALTIME, &start_exec_time_processes);
    for(int i = 0; i < dim_process; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
           	int start = getStartIndex(i, quoziente);
            int finish = getFinishIndex(i, quoziente, dim_process, dim_array);
                
            makeSum_pipe(num_righe, start, finish, vet);

            exit(0);
        }
    }

    close(pipefd[1]);

    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) != -1) {
        int tmp_sum;

        if (read(pipefd[0], &tmp_sum, sizeof(tmp_sum)) != sizeof(tmp_sum)) {
            printf("Errore nella lettura dalla pipe :(\n");
            exit(EXIT_FAILURE);
        }
      
        sum += tmp_sum;
    }
    double total_exec_time_processes = getTimeTaken(start_exec_time_processes);

	// ----------------------------- MAIN
	clock_gettime(CLOCK_REALTIME, &start_exec_time_main);
	int m_sum = sumInMain(num_righe, dim_array, vet);
	double time_exec_main = getTimeTaken(start_exec_time_main);

    // ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");

	printf("SUM: \t%d\n", m_sum);
	printf("Exec time %f sec \n", time_exec_main);

	printf("\nPROCESSES (using pipe)\n");
	printf("SUM: \t%d\n", sum);
	printf("Exec time %f sec \n", total_exec_time_processes);

	free(vet);
	close(pipefd[0]);
	close(pipefd[1]);
	sem_destroy(mutex);
	free(mutex);
}

void p_more_array_main(int num_righe, int dim_array, int dim_process, int* vet) {
		
	int sum = 0;

    // Creo la pipe
    if (pipe(pipefd) == -1){
		printf("Errore nella creazione della pipe :(\n");
		exit(EXIT_FAILURE);
	}

	mutex = (sem_t *) malloc(sizeof(sem_t));

	if(sem_init(mutex, 1, 1)){ 
		printf("Errore nella creazione del semaforo :(\n");
		free(mutex);
		exit(EXIT_FAILURE);
	}

	int modulo = dim_array % dim_process;
	int quoziente = getQuoziente(modulo, dim_array, dim_process);

	struct timespec start_exec_time_processes;
	
	clock_gettime(CLOCK_REALTIME, &start_exec_time_processes);

    // printf("\nStart processes...\n");
    for(int i = 0; i < dim_process; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
        	// Solo i processi figli eseguono questo blocco di codice
            
           	int start = getStartIndex(i, quoziente);
            int finish = getFinishIndex(i, quoziente, dim_process, dim_array);
                
            makeSum_pipe(num_righe, start, finish, vet);

            exit(0);
        }
    }

    close(pipefd[1]);

    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) != -1) {
        int tmp_sum;

        if (read(pipefd[0], &tmp_sum, sizeof(tmp_sum)) != sizeof(tmp_sum)) {
            printf("Errore nella lettura dalla pipe :(\n");
            exit(EXIT_FAILURE);
        }
      
        sum += tmp_sum;
    }

    double total_exec_time_processes = getTimeTaken(start_exec_time_processes);

	printf("\nPROCESSES (using pipe)\n");
	printf("SUM: \t%d\n", sum);
	printf("Exec time %f sec \n", total_exec_time_processes);

	close(pipefd[0]);
	close(pipefd[1]);
	sem_destroy(mutex);
	free(mutex);
}

// Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 