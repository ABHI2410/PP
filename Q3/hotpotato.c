#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int i, j, k, MIN_DATA_SIZE,MAX_DATA_SIZE,INCREMENT;
    double start_time, end_time, total_time, local_time;
    double timer_overhead, max_comm_time;
    int runs = 1000;
    int iterations = 100;
    // Check for the command line argument.
    if (argc != 4) {
        if (rank == 0) printf("INvalid Aruguments\n");
        MPI_Finalize();
        exit(1);
    }
    // Change the data type of the argumment from string to integer
    MIN_DATA_SIZE = atoi(argv[1]);
    MAX_DATA_SIZE = atoi(argv[2]);
    INCREMENT = atoi(argv[3]);
    if (rank == 0) {
        printf("Data Size (bytes)\tLatency (us)\tComm. Time (us)\n");
    }
    // for small data size transfer
    for (k = MIN_DATA_SIZE; k <= MAX_DATA_SIZE; k += INCREMENT) {
        int msg_size = k;
        double communication_time[runs];
        for (i = 0; i < runs; i++) {
            MPI_Barrier(MPI_COMM_WORLD);
            total_time = 0.0;

            for (j = 0; j < iterations; j++) {
                double tmp = MPI_Wtime();
                char* sendbuf = (char*) malloc(msg_size);
                char* recvbuf = (char*) malloc(msg_size);
                MPI_Barrier(MPI_COMM_WORLD);
                start_time = MPI_Wtime();
                MPI_Allreduce(sendbuf, recvbuf, msg_size, MPI_BYTE, MPI_SUM, MPI_COMM_WORLD);
                end_time = MPI_Wtime();
                free(sendbuf);
                free(recvbuf);

                timer_overhead = start_time - tmp;
                local_time = end_time - start_time - timer_overhead;
                total_time += local_time;
            }

            MPI_Barrier(MPI_COMM_WORLD);
            MPI_Allreduce(&total_time, &max_comm_time, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
            communication_time[i] = max_comm_time / iterations;
        }

        if (rank == 0) {
            double avg_time = 0.0;
            for (i = 0; i < runs; i++) {
                avg_time += communication_time[i];
            }
            avg_time /= runs;
            printf("%d\t%lf\t%lf\n", msg_size, timer_overhead * 1000000,(avg_time / 1000)*1000000);
        }
    }

    MPI_Finalize();
    return 0;
}
