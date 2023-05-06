#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int i, j, k;
    double start_time, end_time, total_time, local_time;
    double timer_overhead, max_comm_time;
    int runs = 1000;
    int iterations = 100;

    // for small data size transfer
    for (k = 0; k <= 512; k += 32) {
        int msg_size = k;

        double communication_time[runs];
        for (i = 0; i < runs; i++) {
            MPI_Barrier(MPI_COMM_WORLD);
            total_time = 0.0;

            for (j = 0; j < iterations; j++) {
                char* sendbuf = (char*) malloc(msg_size);
                char* recvbuf = (char*) malloc(msg_size);
                MPI_Barrier(MPI_COMM_WORLD);

                double tmp = MPI_Wtime();
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
            printf("Msg size: %d bytes, Latency: %lf,Avg. Comm. time: %lf us\n", msg_size, timer_overhead * 1000000,(avg_time / 1000)*1000000);
        }
    }

    // For large data size transfer
    for (k = 0; k <= 128000; k += 1024) {
        int msg_size = k;

        double communication_time[runs];
        for (i = 0; i < runs; i++) {
            MPI_Barrier(MPI_COMM_WORLD);
            total_time = 0.0;

            for (j = 0; j < iterations; j++) {
                char* sendbuf = (char*) malloc(msg_size);
                char* recvbuf = (char*) malloc(msg_size);
                MPI_Barrier(MPI_COMM_WORLD);

                double tmp = MPI_Wtime();
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
            printf("Msg size: %d bytes, Latency: %lf,Avg. Comm. time: %lf us\n", msg_size, timer_overhead * 1000000,(avg_time / 100)*1000000);
        }
    }

    MPI_Finalize();
    return 0;
}
