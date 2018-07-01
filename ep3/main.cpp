#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "gpu.h"
#include "utils.h"
#include "pthreads.h"
#include "sequencial.h"
#include <sys/sysinfo.h>


int main(int argc, char **argv) {

    ll n;
    int m,k;
    read(argc,argv,n,m,k);

    solve(n,m,k,gpu,"Tempo na GPU com uma thread na CPU em segundos: ");
    char aux[100];
    sprintf(aux, "Tempo na CPU com %d threads em segundos: ", get_nprocs_conf());
    solve(n,m,k,pthreads_test, (const char*) aux);
    solve(n,m,k,sequencial,"Tempo sequencial em segundos: ");

    ll num_points_to_gpu = choose_optimum_distribution(n, m, k);
    printf("best distribution %d\n", num_points_to_gpu);
    // choosing the best distribution of points between cpu and gpu
   
    /*
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Find out rank, size
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We are assuming at least 2 processes for this task
    if (world_size != 2) {
        fprintf(stderr, "World size must be two for %s\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int ping_pong_count = 0;
    int partner_rank = (world_rank + 1) % 2;
    while (ping_pong_count < PING_PONG_LIMIT) {
        if (world_rank == ping_pong_count % 2) {
            // Increment the ping pong count before you send it
            ping_pong_count++;
            MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
            printf("%d sent and incremented ping_pong_count %d to %d\n",
                   world_rank, ping_pong_count, partner_rank);
        } else {
            MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            printf("%d received ping_pong_count %d from %d\n",
                   world_rank, ping_pong_count, partner_rank);
        }
    }
    MPI_Finalize();
    */
}
