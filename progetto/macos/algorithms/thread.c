sem_t *mutex;
int t_sum = 0;

struct arg_struct {
	int *vet;
  	int start;
  	int finish;
  	int num_righe;
}; 

int isWorking(void) {
	void *tmp = malloc(5);
	free(tmp);
	return tmp ? 1 : 0;
}

void *myThread(struct arg_struct* arguments) {

	struct arg_struct *args = arguments;
	
	if (isWorking()) {
		int internal_sum = 0;
		for(int j = 0; j < args->num_righe; j++)
			for(int i = args->start; i < args->finish; i++)
				internal_sum += args->vet[i];
			
		sem_wait(mutex);
		t_sum += internal_sum;
		sem_post(mutex);
		
		pthread_exit(1);
	} 
	else 
		pthread_exit(0);
}

void threads() {
	printf("\n\nThreads\n\n");

	mutex = (sem_t *) malloc(sizeof(sem_t));

	sem_init(mutex, 0, 1);
	
	int num_righe = 1;
	int dim_array = getDimArray(); // Input dimensione array
	int dim_thread = getNumThreads(); // Input numero thread
	
	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array

	struct arg_struct *args = malloc(dim_thread * sizeof (struct arg_struct)); // Alloca spazio per la struct

	if (!vet || !args) { 
		printf("Errore nell'allocazione della memoria :(\n");
		exit(EXIT_FAILURE);
	}
	
	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");
		
	struct timespec start_array_population_time, start_exec_time_main, start_exec_time_threads;
	clock_gettime(CLOCK_REALTIME, &start_array_population_time);

	populateArrayRandom(num_righe, dim_array, vet); 
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));
	
	//outputArray(vet, dim_array);

	int status = 0; // Variabile di stato dei threads

	int modulo = dim_array % dim_thread;
	int quoziente = getQuoziente(modulo, dim_array, dim_thread);

	printf("\n\n***** LOADING *****\n\n");

	// ---------------- MAIN
	clock_gettime(CLOCK_REALTIME, &start_exec_time_main);
	int m_sum = sumInMain(num_righe, dim_array, vet);
	double time_exec_main = getTimeTaken(start_exec_time_main);

	// ---------------- JOIN
	clock_gettime(CLOCK_REALTIME, &start_exec_time_threads);
	pthread_t threads_join[dim_thread];
	
	for(int i = 0; i < dim_thread; i++){
		args[i].vet = vet;
		args[i].start = getStartIndex(i, quoziente);
		args[i].finish = getFinishIndex(i, quoziente, dim_thread, dim_array);
		args[i].num_righe = num_righe;
		
		pthread_create(&threads_join[i], NULL, myThread, &args[i]);
	}
	
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads_join[i], status);
		if(status != 0) 
			printf("Errore nel join del thread%d\nStatus: %d", i, status);
	}
	double total_exec_time_threads = getTimeTaken(start_exec_time_threads);

	// ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");

	printf("SUM: \t%d\n", m_sum);
	printf("Execution time %f sec \n", time_exec_main);
	
	printf("\nTHREADS (using join)\n");
	printf("SUM: \t%d\n", t_sum);
	printf("Execution time %f sec \n", total_exec_time_threads);
	
	// Distruzione del semaforo
	sem_destroy(mutex);

	// Deallocazione memoria dinamica utilizzata
	free(vet);  
	free(args);
	free(mutex);
	t_sum = 0;
}

void t_more_array_main(int num_righe, int dim_array, int dim_thread, int* vet) {

	mutex = (sem_t *) malloc(sizeof(sem_t));

	sem_init(mutex, 0, 1);	

	struct arg_struct *args = malloc(dim_thread * sizeof (struct arg_struct)); // Alloca spazio per la struct

	if (!vet || !args) { 
		printf("Errore nell'allocazione della memoria :(\n");
		exit(EXIT_FAILURE);
	}

	int status = 0; // Variabile di stato dei threads

	int modulo = dim_array % dim_thread;
	int quoziente = getQuoziente(modulo, dim_array, dim_thread);

	struct timespec start_exec_time_main, start_exec_time_threads;
	clock_gettime(CLOCK_REALTIME, &start_exec_time_threads);
 
	pthread_t threads[dim_thread]; // Creazione array di thread

	// ----------------THREADS
	for(int i = 0; i < dim_thread; i++){

		args[i].vet = vet;
		args[i].start = getStartIndex(i, quoziente);
		args[i].finish = getFinishIndex(i, quoziente, dim_thread, dim_array);
		args[i].num_righe = num_righe;
		
		pthread_create(&threads[i], NULL, myThread, &args[i]);
	}
	
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads[i], status);

		if(status != 0) 
			printf("Errore nel detach del thread%d\nStatus: %d", i, status);
	}

	double total_exec_time_threads = getTimeTaken(start_exec_time_threads);

	// ---------------- MAIN
	clock_gettime(CLOCK_REALTIME, &start_exec_time_main);
	int m_sum = sumInMain(num_righe, dim_array, vet);
	double time_exec_main = getTimeTaken(start_exec_time_main);

	// ---------------- Risultati finali
	printf("\nTHREADS (using join)\n");
	printf("SUM: \t%d\n", t_sum);
	printf("Exec time %f sec \n", total_exec_time_threads);
	
	// Distruzione del semaforo
	sem_destroy(mutex);

	// Deallocazione memoria dinamica utilizzata
	free(args);
	free(mutex);
	t_sum = 0;
}

// Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 