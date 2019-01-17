#include <stdio.h>
#include "mpi.h"
#include "omp.h"

int main (void) {

int rank, tid, size, provided;

MPI_Init_thread(NULL, NULL, MPI_THREAD_FUNNELED, &provided);

MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);

#pragma omp parallel private(tid) 
{
     tid = omp_get_thread_num();
     printf("Hello from thread %d of process %d\n", tid, rank);
}

MPI_Finalize();

return 0;

}
