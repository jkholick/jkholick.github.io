#include <stdio.h>
#include <omp.h>

int fib(int n) {
	if (n < 2)
		return n;
	int x, y;
	
	#pragma omp task shared(x)
	x = fib(n - 1);
	
	#pragma omp task shared(y)
	y = fib(n - 2);
	
	#pragma omp taskwait
	return x + y;
}

int main() {
	int n;
	printf("Enter the number of Fibonacci terms: ");
	scanf("%d", &n);
	printf("Fibonacci Series:\n");
	
	#pragma omp parallel
	{
		#pragma omp single
		{
			for (int i = 0; i < n; i++) {
				#pragma omp task firstprivate(i)
				{
					int result = fib(i);
					#pragma omp critical
					printf("Fib(%d) = %d\n", i, result);
				}
			}
		}
	}
	return 0;
}