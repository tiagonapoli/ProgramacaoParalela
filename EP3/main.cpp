#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "balanceado.h"
#include "gpu.h"
#include "utils.h"
#include "pthreads.h"
#include "sequencial.h"
#include <sys/sysinfo.h>

int main(int argc, char **argv) {

    int my_rank, world_size;

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

	//Desconsidera a primeira execucao na gpu, ela geralmente eh mais lenta
	if(my_rank == 0) solve(1e7,1,1,gpu,"Tempo na GPU com uma thread na CPU em segundos: ", false);

    ll n;
    int m,k;
    

    if(my_rank == 0) read(argc,argv,n,m,k);
    solve(n,m,k,balanceado,"Tempo com balanceamento de carga em segundos: ", (my_rank == 0));
    if(my_rank == 0) {
        solve(n,m,k,gpu,"Tempo na GPU com uma thread na CPU em segundos: ", true);
        char aux[100];
        sprintf(aux, "Tempo na CPU com %d threads em segundos: ", get_nprocs_conf());
        solve(n,m,k,pthreads_test, (const char*) aux, true);
        solve(n,m,k,sequencial,"Tempo sequencial em segundos: ", true);
    }
   	MPI_Finalize();
}
