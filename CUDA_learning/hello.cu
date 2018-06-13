#include <stdio.h>

__global__ void olar() {
    printf("Ola!\n");
}

int main() {

    olar<<1,1>>();
        
}