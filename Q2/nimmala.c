#include "mpi.h"
#include <math.h>
#include<stdio.h>
#include<stdlib.h>


int main(int argc,char *argv[])
{
    int done = 100, n, myid, no_process, i;
    double PI_digital = 3.141592653589793238462643;
    double mpi, pi, h, sum, x,begin=0.0,end=0.0;
	
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&no_process);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    if (argc != 2){
        if (!myid) printf("Command Line: %s <m> \n", argv[0]);
        MPI_Finalize();
        exit(1);
    }
    n = atoi(argv[1]);
    
    while (done != 0) {
        begin = MPI_Wtime();
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0) break;
    
        h   = 1.0 / (double) n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += no_process) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x*x);
        }
        mpi = h * sum;
        
        MPI_Reduce(&mpi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0,MPI_COMM_WORLD);
            end = MPI_Wtime();
        if (myid == 0){
            printf("%f\t%.16lf\n",(end-begin)* 1000000,pi);
        }
        done--;
    }
    MPI_Finalize();
    return 0;
}