#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int is_prime(int num) {
	if (num <= 1) return 0;
	if (num == 2) return 1;
	if (num % 2 == 0) return 0;
	
	for (int i = 3; i * i <= num; i += 2)
		if (num % i == 0)
			return 0;
	
	return 1;
}

int main() {
	int n;
	printf("Enter the value of n: ");
	scanf("%d", &n);
	
	int *primes_serial = (int *)malloc(n * sizeof(int));
	int *primes_parallel = (int *)malloc(n * sizeof(int));
	
	double start_serial = omp_get_wtime();
	for (int i = 1; i <= n; i++) {
		if (is_prime(i)) {
			primes_serial[i - 1] = 1;
		} else {
			primes_serial[i - 1] = 0;
		}
	}
	
	double end_serial = omp_get_wtime();
	double time_serial = end_serial - start_serial;
	
	double start_parallel = omp_get_wtime();
	#pragma omp parallel for
	for (int i = 1; i <= n; i++) {
		if (is_prime(i)) {
			primes_parallel[i - 1] = 1;
		} else {
			primes_parallel[i - 1] = 0;
		}
	}
	
	double end_parallel = omp_get_wtime();
	double time_parallel = end_parallel - start_parallel;
	
	printf("\nPrime numbers from 1 to %d:\n", n);
	for (int i = 1; i <= n; i++) {
		if (primes_parallel[i - 1])
			printf("%d ", i);
	}
	
	printf("\n\nSerial Execution Time : %.6f seconds", time_serial);
	printf("\nParallel Execution Time : %.6f seconds", time_parallel);
	printf("\nSpeedup : %.2f×\n", time_serial / time_parallel);
	
	free(primes_serial);
	free(primes_parallel);
	
	return 0;
}