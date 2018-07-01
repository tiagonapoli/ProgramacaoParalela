#include <string.h>
#include "balanceado.h"
//#include "gpu.h"
#include "pthreads.h"
#include "sequencial.h"

int main(int argc, char **argv) {
    printf("Flags:\n-s: Sequential tester\n-g: GPU tester\n-b: Balancing tester\n-p: Pthreads tester\n\n");
    for(int i=0;i<argc;i++) {
        if(strlen(argv[i]) >= 2) {
            switch (argv[i][1]) {
                case 's':
                    printf("*************** SEQUENCIAL *******************\n");
                    seq_tester();
                    printf("********************************************\n\n");   
                    break;
                case 'g':
                    printf("*************** GPU *******************\n");
//                    gpu_tester();
                    printf("********************************************\n\n");  
                    break;
                case 'b':
                    printf("*************** BALANCING *******************\n");
                    balanceado_tester();
                    printf("********************************************\n\n"); 
                    break;
                case 'p':
                    printf("*************** PTHREADS *******************\n");
                    pthreads_tester();
                    printf("********************************************\n\n");
                    break;		
			}
        }
    } 
}
