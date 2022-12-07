#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_LIMIT 1000
pthread_mutex_t mutex;
int sum = 0;
double total_time_taken = 0;

double getTimeTaken(clock_t t){
	t = clock() - t;
  	return ( (double)t ) / CLOCKS_PER_SEC;
}

int something_worked(void) {
    /* thread operation might fail, so here's a silly example */
	void *p = malloc(100);
	free(p);
	return p ? 1 : 0;
}

void *myThread(int *arg) {
	clock_t t = clock();

	if (something_worked()) {
		
		printf("Hello world! I'm thread%d\n", ((int)arg + 1));
		fflush(stdout);

		sum++;
		
		pthread_mutex_lock(&mutex); // entra nella zona critica
		
		printf("+++\tAccesso zona critica td%d\n", ((int)arg + 1));

		float time_taken = getTimeTaken(t);
		printf("Thread%d took %f sec to execute \n", ((int)arg + 1), time_taken);
		fflush(stdout);
		total_time_taken += time_taken;

		printf("---\tRilascio zona critica td%d\n", ((int)arg + 1));

		pthread_mutex_unlock(&mutex); // esce dalla zona critica

		pthread_exit(1);
	} 
	else pthread_exit(0);
}

void populateArrayRandom(int vet[], int dim_array){
	srand(time(NULL));
	for(int i = 0; i < dim_array; i++)
		vet[i] = rand() % MAX_LIMIT;
}

void outputArray(int vet[], int dim_array){
	for(int i = 0; i < dim_array; i++)
		printf("%d\n", vet[i]);
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

	// ---------------- Popolamento array con valori casuali che vanno da 0 a MAX_LIMIT
	populateArrayRandom(vet, dim_array); 
	outputArray(vet, dim_array);

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

	// ---------------- Creazione thread ed esecuzione
	for(int i = 0; i < dim_thread; i++){
		
		pthread_create(&threads[i], NULL, myThread, i);
	}
	
	// ---------------- Join dei thread
	for(int i = 0; i < dim_thread; i++) {
		pthread_join(threads[i], &status);

		if(status == 0) printf("Error thread%d", i);
	}

	printf("Sum: %d\n", sum);

	printf("Threads exec time %f sec \n", total_time_taken);
	printf("Threads exec (with creation) time %f sec \n", getTimeTaken(total_time_taken_included_creation));

	pthread_mutex_destroy(&mutex); // distruzione del mutex
	free(vet);  // Abbiamo finito di lavorare con l'array, e liberiamo il puntatore associato
	vet = NULL; // Il puntatore non può essere più usato fino ad un riassegnamento effettuato con malloc. */
	
  	return 0;
}
