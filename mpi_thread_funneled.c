#include <stdio.h>
#include "mpi.h"
#include "omp.h"

void printArray(int d[], int n, int r) {
     int j;
     for (j = 0; j < n; j++)
          printf("rank = %d --> %d\n", r, d[j]);
}

int main (void) {

int rank, tid, size, provided, toplam;
MPI_Status status;

MPI_Init_thread(NULL, NULL, MPI_THREAD_FUNNELED, &provided);

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int dizi[20] = { 0 };
int i;

if ( rank == 0 ) {
     for (i = 0; i < 20; i++)
          dizi[i] = i + 1;
}

printArray(dizi, 20, rank);

#pragma omp parallel
{
     if (rank == 0) {
          #pragma omp master
               MPI_Send(dizi, 20, MPI_INT, 1, 777, MPI_COMM_WORLD);
          #pragma omp barrier
     } else {
          #pragma omp master
               MPI_Recv(dizi, 20, MPI_INT, 0, 777, MPI_COMM_WORLD, &status);
          #pragma omp barrier
     }
}

// puts(""); puts("");
// printArray(dizi, 20, rank);

if ( rank == 1 ) {
     #pragma omp parallel private(i) shared(toplam)
     {
          #pragma omp for schedule(static) reduction(+:toplam)
               for (i = 0; i < 20; i++)
                    toplam += dizi[i];
     }

     printf("Toplam = %d\n", toplam);
}

MPI_Finalize();

return 0;

}
