#include <pthread.h>
#include "./utility.c"
    
// Variabili globali
sem_t mutex;
int t_sum = 0;

struct arg_struct {
  int *vet;
  int start;
  int finish;
}; 

int isWorking(void) {
  // Funzione basilare
  // Verifico se il thread è effettivamente in funzione
	void *tmp = malloc(5);
	free(tmp);
	return tmp ? 1 : 0;
}

void *myThread(struct arg_struct* arguments) {

	struct arg_struct *args = arguments;
	
	if (isWorking()) {

		int internal_sum = 0;
		for(int i = args->start; i < args->finish; i++)
			internal_sum += args->vet[i];

		sem_wait(&mutex);
		
		t_sum += internal_sum;

		sem_post(&mutex);

		// printf("\ntid_%d", pthread_self());
		// //printf("\n\texec time %f sec", time_taken);
		// printf("\n\tinternal_sum: %d", internal_sum);
		// printf("\n\tsub-array_length: %d", args->finish - args->start);
		// printf("\n\tsub-array_index: from %d to %d", args->start, args->finish);
		// printf("\n");
		// fflush(stdout);

		// printf("tid%d contenuto array\n", pthread_self());
		// outputArrayWithIndex(args->vet, args->start, args->finish);
		// printf("\n");

		pthread_exit(1);
	} 
	else pthread_exit(0);
	
}

int main() {
	printf("Merenda Saverio Mattia [MAT: 330503]\n");
	printf("Threads\n\n");

	sem_init(&mutex, 0, 1);	

	int dim_array = getDimArray(); // Input dimensione array
	int dim_thread = getNumThreads(); // Input numero thread

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	struct arg_struct *args = malloc(dim_thread * sizeof (struct arg_struct)); // Alloca spazio per la struct

	if (!vet || !args) { 
		printf("Errore nell'allocazione della memoria :(\n");
		return -1;
	}
	
	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");
	
	clock_t start_array_population_time = clock(); // Start cronometro
	populateArrayRandom(vet, dim_array); // Popolamento array con valori casuali che vanno da 1 a MAX_LIMIT + 1
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));
	
	//outputArray(vet, dim_array);

	int status = 0; // Variabile di stato dei threads

	/* Usando questa formula avremo l'ultimo thread con meno numeri (se modulo = 0)
	* Dim array: 21
	* Num thread: 6
	* Crea 6 array 
	* 5 array hanno 4 elementi
	* 1 array ha 1 elemento
	*/
	int modulo = dim_array % dim_thread;
	int quoziente = getQuoziente(modulo, dim_array, dim_thread);


	printf("\n\n***** LOADING *****\n\n");

	clock_t start_exec_time_threads = clock(); 
 
	pthread_t threads[dim_thread]; // Creazione array di thread
	
	// ---------------- Creazione thread ed esecuzione
	for(int i = 0; i < dim_thread; i++){
		
		// Popolamento della struct che verrà passata come argomento ai vari thread
		// Inserisco le informazioni generali
		args[i].vet = vet;
		args[i].start = getStartIndex(i, quoziente);
		args[i].finish = getFinishIndex(i, quoziente, dim_thread, dim_array);
		
		pthread_create(&threads[i], NULL, myThread, &args[i]);
	}
	
	// ---------------- Detach dei thread
	for(int i = 0; i < dim_thread; i++) {
		status = pthread_detach(threads[i]);

		if(status != 0) 
			printf("Errore nel join con il thread%d\nStatus: %d", i, status);
	}

	double total_exec_time_threads = getTimeTaken(start_exec_time_threads);

	clock_t start_exec_time_main = clock();
	int m_sum = sumInMain(vet, dim_array);
	double time_exec_main = getTimeTaken(start_exec_time_main);

	// ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");

	printf("SUM: \t%d\n", m_sum);
	printf("Execution time %f sec \n", time_exec_main);

	printf("\nTHREADS\n");
	printf("SUM: \t%d\n", t_sum);
	printf("Execution time %f sec \n", total_exec_time_threads);
	

	// Distruzione del semaforo
	sem_destroy(&mutex);

	// Deallocazione memoria dinamica utilizzata
	free(vet);  
	free(args);

  return 0;
}