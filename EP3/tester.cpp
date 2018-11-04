#include <mpi.h>
#include <string.h>
#include "balanceado.h"
#include "gpu.h"
#include "pthreads.h"
#include "sequencial.h"

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


    if(my_rank == 0) printf("Flags:\n-s: Sequential tester\n-g: GPU tester\n-b: Balancing tester\n-p: Pthreads tester\n\n");
    for(int i=0;i<argc;i++) {
        if(my_rank == 1 and strlen(argv[i]) >= 2 and argv[i][1] != 'b') continue;
        if(strlen(argv[i]) >= 2) {
            switch (argv[i][1]) {
                case 's':
                    printf("*************** SEQUENCIAL *******************\n");
                    seq_tester();
                    printf("********************************************\n\n");   
                    break;
                case 'g':
                    printf("*************** GPU *******************\n");
                    gpu_tester();
                    printf("********************************************\n\n");  
                    break;
                case 'b':
                    if(my_rank == 0) printf("*************** BALANCING *******************\n");
                    balanceado_tester();
                    if(my_rank == 0) printf("********************************************\n\n"); 
                    break;
                case 'p':
                    printf("*************** PTHREADS *******************\n");
                    pthreads_tester();
                    printf("********************************************\n\n");
                    break;		
			}
        }
    }

	MPI_Finalize();
}
