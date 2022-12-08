#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "utility.c"

#define MAX_LIMIT 1000
pthread_mutex_t mutex;
int sum = 0;
double total_time_taken = 0;
int global_quoziente = 0;

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

//void *myThread(int arg[]) {
void *myThread(void *arguments) {
	clock_t t = clock();

	struct arg_struct *args = arguments;
	
	if (isWorking()) {

		int internal_sum = 0;
		for(int i = args->start; i < args->finish; i++)
			internal_sum += args->vet[i];

		pthread_mutex_lock(&mutex); // entra nella zona critica
		//printf("+++\tAccesso zona critica tid%d\n", pthread_self());
		
		sum += internal_sum;

		float time_taken = getTimeTaken(t);

		printf("\ntid_%d", pthread_self());
		printf("\n\texec time %f sec", time_taken);
		printf("\n\tinternal_sum: %d", internal_sum);
		printf("\n\tsub-array_length: %d", args->finish - args->start);
		printf("\n");
		//printf("indirizzo array:%d\n", &arg);
		fflush(stdout);

		total_time_taken += time_taken;

		//printf("---\tRilascio zona critica tid: %d\n", pthread_self());
		pthread_mutex_unlock(&mutex); // esce dalla zona critica

		// printf("tid%d contenuto array\n", pthread_self());
		// outputArrayWithIndex(args->vet, args->start, args->finish);
		// printf("\n");

		pthread_exit(1);
	} 
	else pthread_exit(0);
	
}

int main() {
	int dim_array, dim_thread;
	pthread_mutex_init (&mutex, NULL); // il mutex è iniziamente libero
	
	// ---------------- Input dimensione array
	do {
		printf("Inserire la dimensione dell'array: ");
		scanf("%d", &dim_array);  
	} while(dim_array < 1);

	/* Alloca spazio per un array con dim_array elementi di tipo int. */
	int *vet = (int *) malloc(dim_array * sizeof (int));
	if (!vet) {
		printf("La memoria non puo' essere allocata :(\n");
		return -1;
	}

	struct arg_struct *args = malloc(dim_array * sizeof (struct arg_struct));
	if (!args) {
		printf("La memoria non puo' essere allocata :(\n");
		return -1;
	}

	// ---------------- Popolamento array con valori casuali che vanno da 1 a MAX_LIMIT + 1
	populateArrayRandom(vet, dim_array); 
	// printf("Output array:\n");
	// outputArray(vet, dim_array);

	// ---------------- Input numero thread
	do {
		printf("Inserire il numero di thread (almeno 5 e massimo 10): ");
		scanf("%d", &dim_thread);  
	} while(dim_thread < 5 || dim_thread > 10);

	// ---------------- Start cronometro
	clock_t total_time_taken_included_creation = clock();

	// ---------------- Creazione array di thread
	pthread_t threads[dim_thread]; 
	void *status = 0; 

	/* Usando questa formula avremo l'ultimo thread con meno numeri
	* Dim array: 21
	* Num thread: 6
	* Crea 6 array 
	* 5 array hanno 4 elementi
	* 1 array ha 1 elemento
	*/
	int modulo = dim_array % dim_thread;
	int quoziente;
	
	if(modulo == 0)
		quoziente = dim_array / dim_thread;
	else 
		quoziente = dim_array / (dim_thread - 1);

	printf("\nQuoziente: %d", dim_array / dim_thread);
	printf("\nQuoziente scelto: %d\n", quoziente);
	printf("Modulo (resto): %d\n", modulo);

	if(modulo == 0)
		printf("\n%d thread con %d elementi da sommare\n", dim_thread, quoziente);
	else {
		printf("\n%d thread con %d elementi da sommare\n", dim_thread - 1, quoziente);
		printf("1 thread con %d elementi da sommare\n", quoziente - modulo);
	}
	
	global_quoziente = quoziente;

	// ---------------- Creazione thread ed esecuzione
	for(int i = 0; i < dim_thread; i++){
		args->vet = vet;
		args->tid = (int) &threads[i];

		if(i == 0) {
			args->start = 0;
			args->finish = quoziente;
		} else if(i + 1 == dim_thread) {
			args->start = quoziente * i;
			args->finish = dim_array;
		} else {
			args->start = quoziente * i;
			args->finish = quoziente * (i + 1);
		}
	
		pthread_create(&threads[i], NULL, myThread, (void *)args);

		sleep(1); // senza questo non funziona, TODO: da sistemare
	}
	
	// ---------------- Join dei thread
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads[i], &status);

		if(status == 0) 
			printf("Error thread%d", i);
	}

	printf("\nThreads exec time %f sec \n", total_time_taken);
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