#include <stdio.h>
#include "mpi.h"
#include "omp.h"

double f( double x ) { 
     return 4.0 / ( 1.0 + x * x);
}

void printArray(int d[],int n, int r){
    int j;
    for(j=0;j<n;j++){
        printf("rank = %d -----> %d\n",r, d[j]);
    }
}

int main(void){
    int rank,tid,size,provided;
    MPI_Init_thread(NULL,NULL,MPI_THREAD_FUNNELED,&provided);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    double dizi[3200]={0}, yeniDizi[800]={0};
    int i;
    if(rank == 0){
        for(i=0;i<3200;i++){
            dizi[i]=(double)i/3200.0;
        }
    }

    double toplam = 0.0, sonuc = 0.0;

    MPI_Scatter(dizi,800,MPI_DOUBLE,yeniDizi,800,MPI_DOUBLE,0,MPI_COMM_WORLD);

    #pragma omp parallel shared(toplam) private(i)
    {
            #pragma omp for schedule(static) reduction(+:toplam)
                for(i = 0; i < 800; i++)
                    toplam += f( yeniDizi[i] );  
                
    }
    
    MPI_Reduce( &toplam, &sonuc, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );

    if( rank == 0 ){
        printf("Pi = %f\n", sonuc / 3200.0);
    }

    MPI_Finalize();
    return 0;
}
