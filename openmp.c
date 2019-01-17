#include <stdio.h>
#include "mpi.h"
#include "omp.h"
#define TAG 777

void printArray(int d[], int n, int r) {
     int j;
     for (j = 0; j < n; j++)
          printf("rank = %d --> %d\n", r, d[j]);
}

int main (void) {

int rank, tid, size, provided, toplam;
MPI_Status status;

MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &provided);

MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

int dizi[20] = { 0 };
int yeniDizi[5] = { 0 };
int i;

if ( rank == 0 ) {
     for (i = 0; i < 20; i++)
          dizi[i] = i + 1;
}

#pragma omp parallel private(tid, i)
{
     tid = omp_get_thread_num();
     if (rank == 0)     
          MPI_Send(&dizi[5*tid], 5, MPI_INT, 1, TAG + tid, MPI_COMM_WORLD);
     else
          MPI_Recv(yeniDizi, 5, MPI_INT, 0, TAG + tid, MPI_COMM_WORLD, &status);
     if ( rank == 1 )
          for (i = 0; i < 5; i++)
               printf("P %d,   T %d --> yeniDizi[%d] = %d\n", rank, tid, i, yeniDizi[i]);
}

MPI_Finalize();

return 0;

}






