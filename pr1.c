#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void merge(int arr[], int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;
	
	int *L = (int *)malloc(n1 * sizeof(int));
	int *R = (int *)malloc(n2 * sizeof(int));
	
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1 + j];
	
	i = 0;
	j = 0;
	k = left;
	
	while (i < n1 && j < n2)
		arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
	while (i < n1)
		arr[k++] = L[i++];
	while (j < n2)
		arr[k++] = R[j++];
	
	free(L);
	free(R);
}

void serialMergeSort(int arr[], int left, int right) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		
		serialMergeSort(arr, left, mid);
		serialMergeSort(arr, mid + 1, right);
		
		merge(arr, left, mid, right);
	}
}

void parallelMergeSort(int arr[], int left, int right, int depth) {
	if (left < right) {
		int mid = left + (right - left) / 2;
		if (depth <= 4) {
			#pragma omp parallel sections
			{
				#pragma omp section
				parallelMergeSort(arr, left, mid, depth + 1);
				#pragma omp section
				parallelMergeSort(arr, mid + 1, right, depth + 1);
			}
		} else {
			serialMergeSort(arr, left, mid);
			serialMergeSort(arr, mid + 1, right);
		}
		merge(arr, left, mid, right);
	}
}

int main() {
	int n;
	printf("Enter the number of elements in the array: ");
	scanf("%d", &n);
	
	int *arr_seq = (int *)malloc(n * sizeof(int));
	int *arr_par = (int *)malloc(n * sizeof(int));
	
	if (!arr_seq || !arr_par) {
		printf("Memory allocation failed.\n");
		return -1;
	}

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		int val = rand() % 100000;
		arr_seq[i] = val;
		arr_par[i] = val;
	}
	
	double start, end;
	
	start = omp_get_wtime();
	serialMergeSort(arr_seq, 0, n - 1);
	end = omp_get_wtime();
	double time_serial = end - start;
	
	start = omp_get_wtime();
	parallelMergeSort(arr_par, 0, n - 1, 0);
	end = omp_get_wtime();
	double time_parallel = end - start;
	
	printf("\nSerial Merge Sort Time : %.6f seconds\n", time_serial);
	printf("Parallel Merge Sort Time : %.6f seconds\n", time_parallel);
	printf("Speedup : %.2f×\n", time_serial / time_parallel);
	
	for (int i = 0; i < n; i++) {
		if (arr_seq[i] != arr_par[i]) {
			printf("Mismatch at index %d!\n", i);
			break;
		}
	}
	
	free(arr_seq);
	free(arr_par);
	return 0;
}