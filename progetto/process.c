#include "utility.c"

void makeSum(int vet[], int start, int finish){
	int sum = 0;
	for(int i = start; i < finish; i++)
		sum += vet[i];

	printf("\tpid: %d, internal_sum: %d, sub-array_length: %d\n", getpid(), sum, finish - start);
	// printf("pid: %d\n", getpid());
	// printf("\tinternal_sum: %d\n", sum);
	// printf("\n\texec time %f sec", time_taken);
	// printf("\tsub-array_length: %d", finish - start);
	// printf("\n");
	fflush(stdout);
	
	exit(sum);
}

int main() {
	
	int stat;

	int dim_array = getDimArray(); // Input dimensione array
	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array
	populateArrayRandom(vet, dim_array); // Popolamento array con valori casuali che vanno da 1 a MAX_LIMIT + 1
	outputArray(vet, dim_array);

	int dim_process = getNumProcess(); // Input numero thread
	pid_t pid[5];

	int modulo = dim_array % dim_process;
	int quoziente;

	printf("\nQuoziente: %d", dim_array / dim_process);
	
	if(modulo == 0)
		quoziente = dim_array / dim_process;
	else 
		quoziente = dim_array / (dim_process - 1); // è più ottimizzato

	printf("\nQuoziente scelto: %d\n", quoziente);
	printf("Modulo (resto): %d\n", modulo);

	if(modulo == 0)
		printf("\n%d processi con %d elementi da sommare\n", dim_process, quoziente);
	else {
		printf("\n%d processi con %d elementi da sommare\n", dim_process - 1, quoziente);
		printf("1 processi con %d elementi da sommare\n", dim_array % (dim_process - 1));
	}
	printf("\n");

	for(int i = 0; i < dim_process; i++) { // loop will run n times (n=5)
		
		int start, finish;
		if(i == 0) {
			start = 0;
			finish = quoziente;
		} else if(i + 1 == dim_process) {
			start = quoziente * i;
			finish = dim_array;
		} else {
			start = quoziente * i;
			finish = quoziente * (i + 1);
		}

		if((pid[i] = fork()) == 0) {
			printf("[son] pid %d from [parent] pid %d\n", getpid(), getppid());
			sleep(1);
			makeSum(vet, start, finish);
		}
	}

	int final_sum = 0;

	for(int i = 0; i < dim_process; i++) { // loop will run n times (n=5)
		
		pid_t cpid = waitpid(pid[i], &stat, 0);
        
        if (WIFEXITED(stat)) {
            //printf("Child %d terminated with status: %d\n", cpid, WEXITSTATUS(stat));
            final_sum += WEXITSTATUS(stat);
        }
	}

	printf("final_sum: %d\n", final_sum);
	return 0;
}
