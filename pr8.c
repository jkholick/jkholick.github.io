#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int rank, size;
	int data[8];
	int recv_data;
	int gathered_data[8];
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	

	if (size > 8) {
		if (rank == 0) {
			printf("This demo supports up to 8 processes only.\n");
		}
		MPI_Finalize();
		return 0;
	}
	
	if (rank == 0) {
		for (int i = 0; i < size; i++) {
			data[i] = i * 10;
		}
		printf("Root process has data: ");
		for (int i = 0; i < size; i++) {
			printf("%d ", data[i]);
		}
		printf("\n");
	}

	MPI_Scatter(data, 1, MPI_INT, &recv_data, 1, MPI_INT, 0, MPI_COMM_WORLD);

	printf("Process %d received %d\n", rank, recv_data);

	recv_data *= 2;

	MPI_Gather(&recv_data, 1, MPI_INT, gathered_data, 1, MPI_INT, 0,MPI_COMM_WORLD);

	if (rank == 0) {
		printf("Root process gathered: ");
		for (int i = 0; i < size; i++) {
			printf("%d ", gathered_data[i]);
		}
		printf("\n");
	}
	
	MPI_Finalize();
	return 0;
}