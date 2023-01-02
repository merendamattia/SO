#include <stdio.h> 
#include <stdlib.h> // rand()
#include <unistd.h> // fflush()
#include <time.h> // clock(), clock_gettime()
#include <sys/wait.h> // wait()
#include <semaphore.h> // Semafori
#include <pthread.h> // Threads

#define MAX_LIMIT 500

/*
* Popolamento di un array con numeri interi random 
* Range: da -(MAX_LIMIT / 2) a +(MAX_LIMIT / 2)
*/
void populateArrayRandom(int num_righe, int dim_array, int* vet){
	srand(time(NULL));
	for(int j = 0; j < num_righe; j++)
		for(int i = 0; i < dim_array; i++)
			vet[i] = (rand() % MAX_LIMIT) - (MAX_LIMIT / 2);
}

void outputArray(int num_righe, int dim_array, int* vet){
	// printf("Output array:\n");
	for(int j = 0; j < num_righe; j++)
		for(int i = 0; i < dim_array; i++)
			printf("[%d] %d\n", i, vet[i]);
}

void outputArrayWithIndex(int vet[], int start, int finish){
	for(int i = start; i < finish; i++)
		if(vet[i] != 0)
			printf("%d) %d\n", i + 1, vet[i]);
}

/*
* Ritorna il tempo impiegato per l'esecuzione
* 'struct timespec start' è la struct contenente i secondi e i millisecondi in cui parte il "cronometro"
*/
double getTimeTaken(struct timespec start){
	struct timespec end;
	clock_gettime(CLOCK_REALTIME, &end);
	long total_time_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);

  	return (double)total_time_ns / 1000000000;
}

int sumInMain(int num_righe, int dim_array, int* vet){
	int sum_in_main = 0;
	
	for(int j = 0; j < num_righe; j++)
		for(int i = 0; i < dim_array; i++)
			sum_in_main += vet[i];
	
	return sum_in_main;
}

int getDimArray(){
	int dim_array;
	do {
		printf("Inserire la dimensione dell'array: ");
		scanf("%d", &dim_array);  
	} while(dim_array < 1);
	return dim_array;
}

int getNumRighe(){
	int num_righe;
	do {
		printf("Inserire il numero di array (almeno 2): ");
		scanf("%d", &num_righe);  
	} while(num_righe < 2);
	return num_righe;
}

int getNumThreads(){
	int dim_thread;
	do {
		printf("Inserire il numero di thread (almeno 5 e massimo 100): ");
		scanf("%d", &dim_thread);  
	} while(dim_thread < 5 || dim_thread > 100);
	return dim_thread;
}

int getNumProcess(){
	int dim_process;
	do {
		printf("Inserire il numero dei processi (almeno 5 e massimo 100): ");
		scanf("%d", &dim_process);  
	} while(dim_process < 5 || dim_process > 100);
	return dim_process;
}

int getNumArray(){
	int num_array;
	do {
		printf("Inserire il numero degli array (almeno 2): ");
		scanf("%d", &num_array);  
	} while(num_array < 1);
	return num_array;
}

int getQuoziente(int modulo, int dim_array, int dim_process){
	if(modulo == 0)
		return dim_array / dim_process;
	else 
		return dim_array / (dim_process - 1); // è più ottimizzato
}

int getStartIndex(int i, int quoziente){
	if(i == 0)
		return 0;
	else 
		return quoziente * i;
}

int getFinishIndex(int i, int quoziente, int dim_process, int dim_array){
	if(i == 0) 
		return quoziente;
	else if(i + 1 == dim_process) 
		return dim_array;
	else 
		return quoziente * (i + 1);		
}

// Menu switch
int getScelta(){
	int scelta;
	do {
		printf("\n\nMenu: \n");
		printf("1. Utilizzo dei soli Threads \n");
		printf("2. Utilizzo dei soli Processi \n");
		printf("3. Utilizzo sia Threads e Processi con un array\n");
		printf("4. Terminazione programma \n");
		printf("Scelta: ");
		scanf("%d", &scelta);  
	} while(scelta < 1 || scelta > 4);
	
	return scelta;
} 
// Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 