#include <string.h>
#include "balanceado.h"
#include "gpu.h"
#include "pthreads.h"
#include "sequencial.h"

int main(int argc, char **argv) {
    for(int i=0;i<argc;i++) {
        if(strlen(argv[i]) >= 2) {
            switch argv[i][1]:
                case 's':
                    seq_tester();
                    break;
                case 'g':
                    gpu_tester();
                    break;
                case 'b':
                    balanceado_tester();
                    break;
                case 'p':
                    pthreads_tester();
                    break;
        }
    } 
}