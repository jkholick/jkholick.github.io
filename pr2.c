#include <stdio.h>
#include <omp.h>

int main() {
	int num_tasks, num_threads;
	// Get user input
	printf("Enter the number of tasks: ");
	scanf("%d", &num_tasks);
	
	printf("Enter the number of threads: ");
	scanf("%d", &num_threads);
	
	printf("--------------------------------------\n");
	// Set number of threads
	omp_set_num_threads(num_threads);
	// Parallel for loop with static scheduling and chunk size 2
	#pragma omp parallel for schedule(static, 2)
	for (int i = 0; i < num_tasks; i++) {
		int tid = omp_get_thread_num();
		// Critical section to prevent mixed output
		#pragma omp critical
		{
			printf("Thread %d executes iteration %d\n", tid, i);
		}
	}
	return 0;
}