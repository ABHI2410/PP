#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int n, my_rank, p;
    double *A, *B, *X, *A_local, *B_local, *X_local;
    int i, j, k;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {

        if (argc != 2) {
            if (p == 0) printf("Please supply a matrix size.\n");
            MPI_Finalize();
            exit(1);
        }

        n = atoi(argv[1]);

        A = (double *) malloc(n * n * sizeof(double));
        B = (double *) malloc(n * sizeof(double));
        X = (double *) malloc(n * sizeof(double));

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {  
                A[i * n + j] = ((double) rand() / (double) RAND_MAX);
            }
            X[i] = i+1;
            for (j = 0; j < n; j++) {
                B[i] += A[i * n + j] * (double) X[i+1];
                printf("%lf\n",B[i]);
            }
        }
        for (i =0; i<n;i++){
            for (j = 0 ; j< n;j++){
                printf("%lf * %lf = %lf\n",A[i],X[i],B[i]);
            }
        }
    }
    

    // MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // int block_size = n / p;

    // A_local = (double *) malloc(n * block_size * sizeof(double));
    // B_local = (double *) malloc(block_size * sizeof(double));
    // X_local = (double *) malloc(block_size * sizeof(double));

    // MPI_Scatter(A, n * block_size, MPI_DOUBLE, A_local, n * block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    // MPI_Scatter(B, block_size, MPI_DOUBLE, B_local, block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // for (k = n - 1; k >= 0; k--) {
    //     if (k % p == my_rank) {
    //         X_local[k / p] = B_local[k / p] / A_local[k * block_size + k / p];
    //         for (i = 0; i < k; i++) {
    //             B_local[i] -= A_local[i * block_size + k / p] * X_local[k / p];
    //         }
    //     }
    // }

    // MPI_Gather(X_local, block_size, MPI_DOUBLE, X, block_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // if (my_rank == 0) {
    //     printf("Solution:\n");
    //     for (i = 0; i < n; i++) {
    //         printf("%f ", X[i]);
    //     }
    //     printf("\n");

    //     free(A);
    //     free(B);
    //     free(X);
    // }

    // free(A_local);
    // free(B_local);
    // free(X_local);

    MPI_Finalize();

    return 0;
}
