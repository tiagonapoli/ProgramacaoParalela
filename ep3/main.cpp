#include <stdio.h>
#include <stdlib.h>
#include "balanceado.h"
#include "gpu.h"
#include "utils.h"
#include "pthreads.h"
#include "sequencial.h"
#include <sys/sysinfo.h>

int main(int argc, char **argv) {
    ll n;
    int m,k;
    read(argc,argv,n,m,k);
    solve(n,m,k,balanceado,"Tempo com balanceamento de carga em segundos: ");
    solve(n,m,k,gpu,"Tempo na GPU com uma thread na CPU em segundos: ");
    char aux[100];
    sprintf(aux, "Tempo na CPU com %d threads em segundos: ", get_nprocs_conf());
    solve(n,m,k,pthreads_test, (const char*) aux);
    solve(n,m,k,sequencial,"Tempo sequencial em segundos: ");
}
