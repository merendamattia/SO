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

void outputArrayWithIndex(int vet[], int start, int finish){
	for(int i = start; i < finish; i++)
		if(vet[i] != 0)
			printf("%d) %d\n", i + 1, vet[i]);
}

double getTimeTaken(clock_t t){
	t = clock() - t;
  	return ( (double)t ) / CLOCKS_PER_SEC;
}

int arrayLength(int vet[], int dim_array){
	int c = 0;
	for(int i = 0; i < dim_array; i++)
		if(vet[i] != 0)
			c++;
	return c;
}

int sumInMain(int vet[], int dim_array){
	int sum_in_main = 0;
	for(int i = 0; i < dim_array; i++)
		sum_in_main += vet[i];
	return sum_in_main;
}