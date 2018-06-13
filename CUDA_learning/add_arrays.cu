#include <stdio.h>
#include <math.h>

__global__ void add(int n, float* x, float* y) {
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	int pulo = gridDim.x * blockDim.x;
	for(int i=tid;i < n; i += pulo) {
		y[i] = x[i] + y[i];
	}
}


int main() {
    int n = 1 << 25;
	
    float *x,*y;
    cudaMallocManaged(&x, n*sizeof(float));
    cudaMallocManaged(&y, n*sizeof(float));


    for(int i=0;i<n;i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

	int block_size = 128;
	int num_blocks = 4096;
    add<<<num_blocks, block_size>>>(n,x,y);

    cudaDeviceSynchronize();

    float error = 0.0f;
    for(int i=0;i<n;i++) {
        error = fmax(error, fabs(y[i]-3.0f));
    }
    printf("Max error: %f\n", error);

    cudaFree(x);
    cudaFree(y);

    return 0;
}
