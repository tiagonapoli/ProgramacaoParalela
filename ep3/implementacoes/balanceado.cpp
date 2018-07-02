#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"
#include "utils.h"
#include "pthreads.h"

const int SUM_TAG = 1;
const int SUM2_TAG = 2;

pff balanceado(ll n, int m, int k, float *sum, float *sum2) {
    
    float sum_0, sum_1, sum2_0, sum2_1;
    int my_rank, world_size;
    ll gpu_n, cpu_n;

    gpu_n = 0.90 * n;
    cpu_n = n - gpu_n;
    sum_0 = sum_1 = sum2_0 = sum2_1 = 0.0;

    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Find out rank, size
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming 2 processes for this task
    if (world_size != 2) {
        fprintf(stderr, "O numero de computadores deve ser 2\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if(my_rank == 0) {
        //MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
        //MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
        MPI_Send(&gpu_n, 1, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD);
        
		printf("FAZER PTHREADS %d\n", my_rank);
        pthreads_test(cpu_n, m, k, &sum_0, &sum2_0);
        
		MPI_Recv(&sum_1, 1, MPI_FLOAT, 1, SUM_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&sum2_1, 1, MPI_FLOAT, 1, SUM2_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        
		printf("Rank0: %lf %lf\n", sum_1, sum2_1);  
        *sum = sum_0 + sum_1;
        *sum2 = sum2_0 + sum2_1;
    } else if(my_rank == 1) {
        MPI_Recv(&gpu_n, 1, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
		printf("FAZER GPU %d\n", my_rank);
        gpu(gpu_n, m, k, &sum_1, &sum2_1);
        printf("Rank1: %lf %lf\n", sum_1, sum2_1);
        
		MPI_Send(&sum_1, 1, MPI_FLOAT, 0, SUM_TAG, MPI_COMM_WORLD);
        MPI_Send(&sum2_1, 1, MPI_FLOAT, 0, SUM2_TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
	return calc_res(n, *sum, *sum2);
}

void balanceado_tester() {
    int tests = 1;
    ll n = 32000000;
    float eps = 5e-3;
    printf("Testes: %d\nN: %lld\neps: %f\n\n", tests, n, eps);
    double porcentagem_acerto = testa_corretude(tests, n, eps, balanceado, true);
    printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}
