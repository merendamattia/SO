#include "./utilities/utility.c"
#include "./algorithms/thread.c"
#include "./algorithms/process.c"   

void threads_vs_processes(){ 
	int num_righe = 1;
	int dim_array = getDimArray(); 
	printf("\nIl numero dei thread scelto sarà uguale al numero dei processi.\n");
	int dim_thread = getNumThreads(); 

	if(dim_thread > dim_array)
		dim_thread = dim_array;

	int *vet = (int *) malloc(dim_array * sizeof (int)); // Alloca spazio per l'array

	if (!vet) { 
		printf("Errore nell'allocazione della memoria :(\n");
		exit(EXIT_FAILURE);
	}    
	
	if(dim_array >= 100000000)
		printf("\n\n***** POPOLAMENTO ARRAY IN CORSO *****\n");

	struct timespec start_array_population_time, start_exec_time_main;
	
	clock_gettime(CLOCK_REALTIME, &start_array_population_time);
	populateArrayRandom(num_righe, dim_array, vet); 
	printf("Array population time %f sec\n", getTimeTaken(start_array_population_time));

	printf("\n\n***** LOADING *****\n\n");     
     
	clock_gettime(CLOCK_REALTIME, &start_exec_time_main);
	int main_sum = sumInMain(num_righe, dim_array, vet);
	double total_exec_time_main = getTimeTaken(start_exec_time_main);

	// ---------------- Risultati finali
	printf("\n***** RESULTS *****\n");
	printf("MAIN\n");
   
	printf("SUM: \t%d\n", main_sum);
	printf("Exec time %f sec \n", total_exec_time_main);  
 
	t_more_array_main(num_righe, dim_array, dim_thread, vet);
	p_more_array_main(num_righe, dim_array, dim_thread, vet);

	free(vet); 
}

int main(){    
	printf("Merenda Saverio Mattia [MAT: 330503]\n");
	printf("Versione MacOs\n");

	int scelta;

	do{
		scelta = getScelta();
  
		switch(scelta){
			case 1: threads(); 
				break; 
			case 2: processes();
				break;
			case 3: threads_vs_processes();
				break;
		}
	
	} while(scelta != 4);

	printf("\nCiaooooo!!\n");
	
	return 0;
}
// Realizzato da [Merenda Saverio Mattia](https://www.merendamattia.com) [MAT: 330503] 