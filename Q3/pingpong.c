#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {

    int rank, size, partner_rank, i, data_size, MIN_DATA_SIZE,MAX_DATA_SIZE, INCREMENT;
    double start_time, end_time, total_time, latency, communication_time, temp;
    char *send_buf, *recv_buf;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        printf("Error: this program requires exactly 2 processes\n");
        MPI_Finalize();
        exit(1);
    }

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

    send_buf = (char *) malloc(MAX_DATA_SIZE * sizeof(char));
    recv_buf = (char *) malloc(MAX_DATA_SIZE * sizeof(char));

    // FOR 512 BYTES OF DATA WITH 32 BYTES INCREMENT
    if (rank == 0) {
        printf("Data Size (bytes)\tTemp (s)\tStart Time (s)\tEnd Time (s)\tLatency (us)\tComm. Time (us)\n");
    }

    for (data_size = MIN_DATA_SIZE; data_size <= MAX_DATA_SIZE; data_size += INCREMENT) {

        temp = MPI_Wtime();
        if (rank == 0) {
            partner_rank = 1;
        } else {
            partner_rank = 0;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        start_time = MPI_Wtime();

        // Ping-pong communication
        for (i = 0; i < 1000; i++) {
            if (rank == 0) {
                MPI_Send(send_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
                MPI_Recv(recv_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(recv_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(send_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
            }
        }

        end_time = MPI_Wtime();
        latency = start_time - temp;
        total_time = end_time - start_time - latency;
        communication_time = total_time /(2 * 1000) ;
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            printf("%d\t%f\t%f\t%f\t%f\t%f\n", data_size, temp, start_time, end_time, latency*1000000, communication_time*1000000);
        }

    }
    free(send_buf);
    free(recv_buf);

    // send_buf = (char *) malloc(MAX_LARGE_DATA_SIZE * sizeof(char));
    // recv_buf = (char *) malloc(MAX_LARGE_DATA_SIZE * sizeof(char));
    // // FOR 128K BYTES OF DATA WITH 1K BYTES INCREMENT
    // if (rank == 0) {
    //     printf("Data Size (bytes)\tTemp (s)\tStart Time (s)\tEnd Time (s)\tLatency (us)\tComm. Time (us)\n");
    // }

    // for (data_size = 0; data_size <= MAX_LARGE_DATA_SIZE; data_size += 1024) {

    //     temp = MPI_Wtime();
    //     if (rank == 0) {
    //         partner_rank = 1;
    //     } else {
    //         partner_rank = 0;
    //     }

    //     MPI_Barrier(MPI_COMM_WORLD);
    //     start_time = MPI_Wtime();

    //     // Ping-pong communication
    //     for (i = 0; i < 1000; i++) {
    //         if (rank == 0) {
    //             MPI_Send(send_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
    //             MPI_Recv(recv_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //         } else {
    //             MPI_Recv(recv_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    //             MPI_Send(send_buf, data_size, MPI_CHAR, partner_rank, 0, MPI_COMM_WORLD);
    //         }
    //     }

    //     end_time = MPI_Wtime();
    //     latency = start_time - temp;
    //     total_time = end_time - start_time - latency;
    //     communication_time = total_time /(2 * 1000) ;
    //     MPI_Barrier(MPI_COMM_WORLD);

    //     if (rank == 0) {
    //         printf("%d\t%f\t%f\t%f\t%f\t%f\n", data_size, temp, start_time, end_time, latency*1000000, communication_time*1000000);
    //     }

    // }

    // free(send_buf);
    // free(recv_buf);
    MPI_Finalize();

    return 0;
}
