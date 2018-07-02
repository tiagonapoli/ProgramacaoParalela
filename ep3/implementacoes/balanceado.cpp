#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"
#include "utils.h"
#include "pthreads.h"

const int SUM_TAG = 1;
const int SUM2_TAG = 2;
const int GPU_N_TAG = 3;
const int N_TAG = 4;
const int M_TAG = 5;
const int K_TAG = 6;

pff balanceado(ll n, int m, int k, float *sum, float *sum2, ll *new_n) {
    
    float sum_0, sum_1, sum2_0, sum2_1;
    int my_rank, world_size;
    ll gpu_n, cpu_n;

	cpu_n = 0.002 * n;
	gpu_n = n - cpu_n;
    sum_0 = sum_1 = sum2_0 = sum2_1 = 0.0;

    // Find out rank, size
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    if(my_rank == 0) {
//MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)
//MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)
        MPI_Send(&gpu_n, 1, MPI_LONG_LONG, 1, GPU_N_TAG, MPI_COMM_WORLD);
        MPI_Send(&n, 1, MPI_LONG_LONG, 1, N_TAG, MPI_COMM_WORLD);
        MPI_Send(&m, 1, MPI_INT, 1, M_TAG, MPI_COMM_WORLD);
        MPI_Send(&k, 1, MPI_INT, 1, K_TAG, MPI_COMM_WORLD);
        
//		struct cronometro cron;
//		cron.set_initial_time();
		pthreads_test(cpu_n, m, k, &sum_0, &sum2_0, &cpu_n);
//        cron.set_final_time();
//		printf("PTHREADS %lf\n", cron.get_ms_past());


		MPI_Recv(&sum_1, 1, MPI_FLOAT, 1, SUM_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&sum2_1, 1, MPI_FLOAT, 1, SUM2_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        MPI_Recv(&gpu_n, 1, MPI_LONG_LONG, 1, GPU_N_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        
        *sum = sum_0 + sum_1;
        *sum2 = sum2_0 + sum2_1;
		*new_n = gpu_n + cpu_n;
    } else if(my_rank == 1) {
		MPI_Recv(&gpu_n, 1, MPI_LONG_LONG, 0, GPU_N_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&n, 1, MPI_LONG_LONG, 0, N_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&m, 1, MPI_INT, 0, M_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&k, 1, MPI_INT, 0, K_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//		struct cronometro cron;
//		cron.set_initial_time();
		gpu(gpu_n, m, k, &sum_1, &sum2_1, &gpu_n);
//        cron.set_final_time();
//		printf("GPU %lf\n", cron.get_ms_past());
        
		MPI_Send(&sum_1, 1, MPI_FLOAT, 0, SUM_TAG, MPI_COMM_WORLD);
        MPI_Send(&sum2_1, 1, MPI_FLOAT, 0, SUM2_TAG, MPI_COMM_WORLD);
		MPI_Send(&gpu_n, 1, MPI_LONG_LONG, 0, GPU_N_TAG, MPI_COMM_WORLD);
    }

	return calc_res(*new_n, *sum, *sum2);
}

void balanceado_tester() {
    int my_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	bool verbose = false;
	if(my_rank == 0) verbose = true;
	int tests = 10;
    ll n = 8192000000;
    float eps = 5e-3;
    if(verbose) printf("Testes: %d\nN: %lld\neps: %f\n\n", tests, n, eps);
    double porcentagem_acerto = testa_corretude(tests, n, eps, balanceado, verbose);
    if(verbose) printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}
