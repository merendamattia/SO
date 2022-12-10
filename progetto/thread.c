#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "utility.c"
 
// Variabili globali
pthread_mutex_t mutex;
int sum = 0;

struct arg_struct {
  int *vet;
  int tid;
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

		pthread_mutex_lock(&mutex); // entra nella zona critica
		//printf("+++\tAccesso zona critica tid%d\n", pthread_self());
		
		sum += internal_sum;

		//printf("---\tRilascio zona critica tid: %d\n", pthread_self());
		pthread_mutex_unlock(&mutex); // esce dalla zona critica

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
	int dim_array, dim_thread;
	pthread_mutex_init (&mutex, NULL); // Il mutex è iniziamente libero
	
	dim_array = getDimArray(); // Input dimensione array

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	struct arg_struct *args = malloc(dim_array * sizeof (struct arg_struct)); // Alloca spazio per la struct

	if (!vet || !args) { 
		printf("Errore nell'allocazione della memoria :(\n");
		return -1;
		// Termino il programma se non posso allocare memoria
	}

	populateArrayRandom(vet, dim_array); // Popolamento array con valori casuali che vanno da - MAX_LIMIT/2 A + MAX_LIMIT/2
	//outputArray(vet, dim_array);

	dim_thread = getNumThreads(); // Input numero thread

	clock_t total_time_taken_included_creation = clock(); // Start cronometro
 
	pthread_t threads[dim_thread]; // Creazione array di thread
	void *status = 0; // Variabile di stato dei threads

	/* Usando questa formula avremo l'ultimo thread con meno numeri (se modulo = 0)
	* Dim array: 21
	* Num thread: 6
	* Crea 6 array 
	* 5 array hanno 4 elementi
	* 1 array ha 1 elemento
	*/
	int modulo = dim_array % dim_thread;
	int quoziente;

	// printf("\nQuoziente: %d", dim_array / dim_thread);
	
	if(modulo == 0)
		quoziente = dim_array / dim_thread;
	else 
		quoziente = dim_array / (dim_thread - 1); // è più ottimizzato

	// printf("\nQuoziente scelto: %d\n", quoziente);
	// printf("Modulo (resto): %d\n", modulo);

	// if(modulo == 0)
	// 	printf("\n%d thread con %d elementi da sommare\n", dim_thread, quoziente);
	// else {
	// 	printf("\n%d thread con %d elementi da sommare\n", dim_thread - 1, quoziente);
	// 	printf("1 thread con %d elementi da sommare\n", dim_array % (dim_thread - 1));
	// }
	
	// ---------------- Creazione thread ed esecuzione
	for(int i = 0; i < dim_thread; i++){
		
		// Popolamento della struct che verrà passata come argomento ai vari thread
		// Inserisco le informazioni generali
		args[i].vet = vet;
		args[i].tid = (int) &threads[i];

		if(i == 0) {
			args[i].start = 0;
			args[i].finish = quoziente;
		} else if(i + 1 == dim_thread) {
			args[i].start = quoziente * i;
			args[i].finish = dim_array;
		} else {
			args[i].start = quoziente * i;
			args[i].finish = quoziente * (i + 1);
		}
	
		pthread_create(&threads[i], NULL, myThread, &args[i]);

		//sleep(1); // senza questo non funziona, TODO: da capire il perchè
	}
	
	clock_t total_time_taken_thread = clock();
	
	// ---------------- Join dei thread
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads[i], &status);

		if(status == 0) 
			printf("Errore nel join con il thread%d\n", i);
	}

	double time_exec_thread = getTimeTaken(total_time_taken_thread);

	// ---------------- Output finali
	printf("\nThreads exec time %f sec \n", time_exec_thread);
	printf("Threads exec (with creation) time %f sec \n", getTimeTaken(total_time_taken_included_creation));

	pthread_mutex_lock(&mutex);
	printf("\nSum in thread: \t%d\n", sum);
	pthread_mutex_unlock(&mutex);

	clock_t total_time_taken_in_main = clock();
	printf("Sum in main: \t%d\n", sumInMain(vet, dim_array));
	printf("\t-> with exec time %f sec \n", getTimeTaken(total_time_taken_in_main));

	// Distruzione del mutex
	pthread_mutex_destroy(&mutex); 
	// Deallocazione memoria dinamica utilizzata
	free(vet);  
	free(args);

  return 0;
}