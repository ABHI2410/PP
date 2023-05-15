#include <stdio.h>
#include <math.h>
#include <mpi.h>

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

int main(int argc, char** argv) {
    int rank, size;
    int n = 1000; // Number of intervals (adjust for desired accuracy)
    double a = 0.0; // Lower limit of integration
    double b = 1.0; // Upper limit of integration
    double h;
    double local_result, global_result;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Number of intervals: %d\n", n);
        printf("Number of processes: %d\n", size);
    }

    // Start the timer
    start_time = MPI_Wtime();

    // Compute local result
    h = (b - a) / (n * size);
    local_result = f(a + rank * h * n) + f(a + (rank + 1) * h * n);

    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        local_result += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
    }

    local_result = local_result * h / 3.0;

    // Reduce local results to obtain the global result
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Stop the timer
    end_time = MPI_Wtime();

    // Print the result in process 0
    if (rank == 0) {
        printf("%.15lf\n", global_result);
        printf("%.6lf \n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}