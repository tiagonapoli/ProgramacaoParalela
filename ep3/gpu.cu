#include "utils.h"
#include <helper_cuda.h>
#include <cuda.h>
#include <curand_kernel.h>

const int NUM_THREADS = 32;

__device__ inline float f_device(int m, int k, float x) {
    return sin((2 * m + 1) * PI * x) * cos(2 * PI * k * x) / sin(PI * x);
}

__global__ void partial_sum(int n, int m, int k, float *sum_block, float *sum2_block) {
	//vetor com sizeof(int) * BLOCK_SIZE
    // mutex
    //__shared__ int *mutex_block = 0;
    //__shared__ float res_block = 0;
    // reduce
    __shared__ float sum[NUM_THREADS];
    __shared__ float sum2[NUM_THREADS];

	int tid = threadIdx.x;
    curandState state;
    /* Each thread gets same seed, a different sequence number, no offset. */
    curand_init(0, NUM_THREADS * blockIdx.x + threadIdx.x, 0, &state);	

    float sum_thread = 0.;
    float sum2_thread = 0.;
    float x, f_x;
    for (int i = 0; i < (n + NUM_THREADS - 1) / NUM_THREADS; i++) {
        x = curand_uniform() / 2.0;
        f_x = f_device(m, k, x);
        sum_thread += f_x;
        sum2_thread += f_x * f_x;
    }    

    // compute block answer using mutex
    /*while(atomicCAS(mutex_block,0,1) != 0);  //lock
    res_block += calc_res_device(n, sum_thread, sum2_thread);
    atomicExch(mutex_block, 0);  //unlock*/
	
    //compute block answer putting answer of each thread of the block in a 
    //array and then applying reduce at it
    sum[tid] = sum_thread;
    sum2[tid] = sum2_thread;
    __syncthreads();

    for (int i = NUM_THREADS / 2; i > 0; i = i / 2) {
        if (threadIdx.x >= i) return;
        sum[threadIdx.x] += sum[i + threadIdx.x];
        sum2[threadIdx.x] += sum2[i + threadIdx.x];
        __syncthreads();
    }

	if(tid == 0) {
        // atomic
		//block_res[blockIdx.x] = res_block;
        // reduce 
        sum_block[blockIdx.x] = sum[0];
        sum2_block[blockIdx.x] = sum2[0];
	}
}

pff gpu(ll n, int m, int k) {
	int num_blocks = (n + 2 * threads - 1)/ (2 * threads);
	
    float *d_sum_block;
    float *d_sum2_block;
    float *h_sum_block;
    float *h_sum2_block;
	checkCudaErrors(cudaMalloc(&d_sum_block, num_blocks * sizeof(float)));
	checkCudaErrors(cudaMalloc(&d_sum2_block, num_blocks * sizeof(float)));
    h_sum_block = (float *)calloc(num_blocks, sizeof(float));
    h_sum2_block = (float *)calloc(num_blocks, sizeof(float));

	checkCudaErrors(cudaMemcpy(d_sum_block, h_sum_block, num_blocks * sizeof(float),cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_sum2_block, h_sum2_block, num_blocks * sizeof(float),cudaMemcpyHostToDevice));
	partial_sum<<<num_blocks, NUM_THREADS>>>((n + num_blocks - 1) / num_blocks, m, k, d_sum_block, d_sum2_block);
	cudaDeviceSynchronize();
	
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaMemcpy(h_sum_block, d_sum_block, num_blocks*sizeof(float), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_sum2_block, d_sum2_block, num_blocks*sizeof(float), cudaMemcpyDeviceToHost));

    float sum = 0.;
    float sum2 = 0.;
    for (int i = 0; i < num_blocks; i++) {
        sum += h_sum_block[i];
        sum2 += h_sum2_block[i];
    }

	checkCudaErrors(cudaFree(d_sum_block));
	checkCudaErrors(cudaFree(d_sum2_block));
	free(h_sum_block);
    free(h_sum2_block);

    return calc_res(n, sum, sum2);
}

int main() {
    double porcentagem_acerto = testa_corretude(50, 32000000, 5e-3, gpu, true);
    printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}
