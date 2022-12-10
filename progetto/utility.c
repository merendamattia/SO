#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_LIMIT 1000

void populateArrayRandom(int vet[], int dim_array){
	srand(time(NULL));
	for(int i = 0; i < dim_array; i++)
		vet[i] = (rand() % MAX_LIMIT + 1) - MAX_LIMIT/2;
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



