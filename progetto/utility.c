#include <stdio.h> // Input-Output
#include <stdlib.h> // rand()
#include <unistd.h> // fflush()
#include <time.h> // clock()
#include <sys/wait.h> // wait()
#include <semaphore.h> // Semafori

#define MAX_LIMIT 1000

/*
* Popolamento di un array con numeri interi random 
* Range: da -(MAX_LIMIT / 2) a +(MAX_LIMIT / 2)
*/
void populateArrayRandom(int vet[], int dim_array){
	srand(time(NULL));
	for(int i = 0; i < dim_array; i++)
		vet[i] = (rand() % MAX_LIMIT) - (MAX_LIMIT / 2);
}

void outputArray(int vet[], int dim_array){
	// printf("Output array:\n");
	for(int i = 0; i < dim_array; i++)
		printf("%d) %d\n", i + 1, vet[i]);
}

void outputArrayWithIndex(int vet[], int start, int finish){
	for(int i = start; i < finish; i++)
		if(vet[i] != 0)
			printf("%d) %d\n", i + 1, vet[i]);
}

/*
* Ritorna il tempo impiegato per l'esecuzione
* 'clock_t t' è la variabile in cui parte il "cronometro"
*/
double getTimeTaken(clock_t t){
	t = clock() - t;
  	return ( (double)t ) / CLOCKS_PER_SEC;
}

int sumInMain(int vet[], int dim_array){
	int sum_in_main = 0;
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

int getNumThreads(){
	int dim_thread;
	do {
		printf("Inserire il numero di thread (almeno 5 e massimo 10): ");
		scanf("%d", &dim_thread);  
	} while(dim_thread < 5 || dim_thread > 10);
	return dim_thread;
}

int getNumProcess(){
	int dim_process;
	do {
		printf("Inserire il numero dei processi (almeno 5 e massimo 10): ");
		scanf("%d", &dim_process);  
	} while(dim_process < 5 || dim_process > 10);
	return dim_process;
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


