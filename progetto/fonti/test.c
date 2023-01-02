#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define MAX_LIMIT 1000

void populateArrayRandom(int vet[], int dim_array){
	srand(time(NULL));
	for(int i = 0; i < dim_array; i++)
		vet[i] = rand() % MAX_LIMIT + 1;
}

void outputArray(int vet[], int dim_array){
	for(int i = 0; i < dim_array; i++)
		if(vet[i] != 0)
			printf("%d) %d\n", i + 1, vet[i]);
}

int main(){
	int dim_array, dim_thread;

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
	
	// ---------------- Input numero thread
	do {
		printf("Inserire il numero di thread (almeno 5 e massimo 10): ");
		scanf("%d", &dim_thread);  
	} while(dim_thread < 5 || dim_thread > 10);

	// ---------------- Popolamento array con valori casuali che vanno da 0 a MAX_LIMIT
	populateArrayRandom(vet, dim_array); 
	outputArray(vet, dim_array);

	/* Usando questa formula avremo l'ultimo thread con meno numeri
	* Dim array: 21
	* Num thread: 6
	* Crea 6 array 
	* 5 array hanno 4 elementi
	* 1 array ha 1 elemento
	*/
	int quoziente = dim_array / (dim_thread - 1);
	printf("\nQuoziente: %d\n", quoziente);

	int modulo = dim_array % dim_thread;
	printf("\nModulo (resto): %d\n", modulo);

	//int new_array[quoziente + modulo];
	int *new_array = (int *)malloc(quoziente * sizeof (int));

	for(int i = 0; i < dim_thread; i++){
		for(int j = 0; j < quoziente; j++){
			
			new_array[j] = vet[quoziente * i + j];

			if(i + 1 != dim_thread && j >= quoziente)
				new_array[j] = 0;
		}
	
		
		printf("\n");
		outputArray(new_array, quoziente);
		printf("\n");
	}

	/* Usando questa formula avremo l'ultimo thread con piu numeri
	* Dim array: 21
	* Num thread: 6
	* Crea 6 array 
	* 5 array hanno 3 elementi
	* 1 array ha 6 elemento
	
	int quoziente = dim_array / dim_thread;
	printf("\nQuoziente: %d\n", quoziente);

	int modulo = dim_array % dim_thread;
	printf("\nModulo (resto): %d\n", modulo);

	//int new_array[quoziente + modulo];
	int *new_array = (int *)malloc(quoziente + modulo * sizeof (int));

	for(int i = 0; i < dim_thread; i++){
		for(int j = 0; j < quoziente + modulo; j++){
			
			new_array[j] = vet[quoziente * i + j];

			if(i + 1 != dim_thread && j >= quoziente)
				new_array[j] = 0;
		}
	
		
		printf("\n");
		outputArray(new_array, quoziente + modulo);
		printf("\n");
	}
	*/


	// Deallocazioni della memoria
	free(vet); 
	vet = NULL;
	free(new_array); 
	new_array = NULL;
	return 0;
}