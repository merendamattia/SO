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


int isWorking(void) {
    /* thread operation might fail, so here's a silly example */
	void *tmp = malloc(5);
	free(tmp);
	return tmp ? 1 : 0;
}

void *myThread(int arg[]) {
	clock_t t = clock();

	if (isWorking()) {

		int internal_sum = 0;
		for(int i = 0; i < global_quoziente; i++)
			internal_sum += arg[i];

		pthread_mutex_lock(&mutex); // entra nella zona critica
		//printf("+++\tAccesso zona critica tid%d\n", pthread_self());
		
		sum += internal_sum;

		float time_taken = getTimeTaken(t);

		printf("\ntid_%d exec time %f sec, internal_sum:%d, arrayLength:%d \n", pthread_self(), time_taken, internal_sum, arrayLength(arg, global_quoziente));
		//printf("indirizzo array:%d\n", &arg);
		fflush(stdout);

		total_time_taken += time_taken;

		//printf("---\tRilascio zona critica tid: %d\n", pthread_self());
		pthread_mutex_unlock(&mutex); // esce dalla zona critica

		// printf("tid%d contenuto array\n", pthread_self());
		// outputArray(arg, global_quoziente);
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

	/* Alloca spazio per un array con 10 elementi di tipo int. */
	int *vet = (int *)malloc(dim_array * sizeof (int));
	if (!vet) {
		/* La memoria non può essere allocata, il programma dovrebbe gestire l'errore in modo appropriato. */
		printf("La memoria non puo' essere allocata :(\n");
		return -1;
	}
	    
	//int vet[dim_array];

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

	printf("\nQuoziente: %d\n", dim_array / dim_thread);

	global_quoziente = quoziente;

	printf("Modulo (resto): %d\n\n", modulo);

	// ---------------- Creazione thread ed esecuzione
	for(int i = 0; i < dim_thread; i++){

		int *new_array = (int *)malloc(quoziente * sizeof (int));
		
		for(int j = 0; j < quoziente; j++){
			
			new_array[j] = vet[quoziente * i + j];

			if(i + 1 != dim_thread && j >= quoziente)
				new_array[j] = 0;
		}
	
		pthread_create(&threads[i], NULL, myThread, new_array);

		sleep(1); // senza questo non funziona, TODO: da sistemare
		free(new_array); 
		new_array = NULL;
	}
	
	// ---------------- Join dei thread
	
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads[i], &status);

		if(status == 0) printf("Error thread%d", i);
	}

	printf("\nThreads exec time %f sec \n", total_time_taken);
	printf("Threads exec (with creation) time %f sec \n", getTimeTaken(total_time_taken_included_creation));

	pthread_mutex_lock(&mutex);
	printf("\nSum in thread: %d\n", sum);
	pthread_mutex_unlock(&mutex);

	clock_t total_time_taken_in_main = clock();
	printf("Sum in main: %d\n", sumInMain(vet, dim_array));
	printf("Sum in main exec time %f sec \n", getTimeTaken(total_time_taken_in_main));

	pthread_mutex_destroy(&mutex); // distruzione del mutex
	free(vet);  // Abbiamo finito di lavorare con l'array, e liberiamo il puntatore associato
	vet = NULL; // Il puntatore non può essere più usato fino ad un riassegnamento effettuato con malloc. */
	// free(new_array); 
	// new_array = NULL;
  	return 0;
}


/*
TODO
creare un array di struct, con campi:
thread id, 
puntatore ad array (vet), 
int che tiene conto del numero di thread cosi da poter lavorare su un unico array condiviso invece di creare n sotto-array, 
ed eventualmente altri campi che possono servire 

