#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>

void generateMatrix(int N, float **matrix) {
    int i, j;
    srand(time(NULL));
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i <= j)
                matrix[i][j] = 2.0 + (float)rand() / (RAND_MAX * 2.0 - 1.0);
            else
                matrix[i][j] = 0.0;
        }
    }
}

void generateVector(int N, float *vector) {
    int i;
    for (i = 0; i < N; i++) {
        vector[i] = (float)(i + 1);
    }
}

int main(int argc, char* argv[]) {
    int i, j, N;
    float **A, *X;
    float *local_X;
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        if (rank == 0) printf("Usage: %s <matrix_size>\n", argv[0]);
        MPI_Finalize();
        return 0;
    }

    N = strtol(argv[1], NULL, 10);
    int block_size = N / size;
    int remainder = N % size;
    int start_row = rank * block_size;
    int end_row = start_row + block_size;

    if (rank == size - 1) {
        end_row += remainder;
    }

    A = (float**)malloc(N * sizeof(float*));
    for (i = 0; i < N; i++) {
        A[i] = (float*)malloc(N * sizeof(float));
    }

    X = (float*)malloc(N * sizeof(float));
    local_X = (float*)malloc((end_row - start_row) * sizeof(float));

    if (rank == 0) {
        generateMatrix(N, A);
        generateVector(N, X);
    }
    double start_time = MPI_Wtime();
    // Broadcast matrix A
    for (i = 0; i < N; i++) {
        MPI_Bcast(A[i], N, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    // Scatter vector X
    MPI_Scatter(X, block_size, MPI_FLOAT, local_X, block_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Forward elimination
    for (i = start_row + 1; i < end_row; i++) {
        for (j = 0; j < i; j++) {
            local_X[i - start_row] -= A[i][j] * local_X[j - start_row];
        }
    }

    // Gather partial results from all processes
    MPI_Gather(local_X, block_size, MPI_FLOAT, X, block_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Back substitution
        for (i = N - 1; i >= 0; i--) {
            for (j = i + 1; j < N; j++) {
                X[i] -= A[i][j] * X[j];
                X[i] /= A[i][i];
            }
            // // Print the solution
            // printf("Solution:\n");
            // for (i = 0; i < N; i++) {
            //     printf("X[%d] = %f\n", i, X[i]);
            // }
        }

        // Clean up
        free(X);
        free(local_X);
        for (i = 0; i < N; i++) {
            free(A[i]);
        }
        free(A);
        start_time = MPI_Wtime() - start_time;
        printf("%lf",start_time*1000000);
    }
        MPI_Finalize();
        return 0;
}

