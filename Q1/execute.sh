eval "mpicc -o prime prime.c -lm"
for ((i=1; i<=32; i++));
do
	eval "mpirun -np $i ./prime 1000000"
done

