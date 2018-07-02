#include "gpu.h"
#include <helper_cuda.h>
#include <cuda.h>
#include <curand_kernel.h>
typedef float __precision;

__constant__ __precision d_PI = 3.14159265359;
__device__ inline __precision f_device(int m, int k, __precision x) {
    return sin((2 * m + 1) * d_PI * x) * cos(2 * d_PI * k * x) / sin(d_PI * x);
}

__global__ void partial_sum(ll n, int m, int k, __precision *sum_block, __precision *sum2_block) {
    
	__shared__ __precision sum[GPU_THREADS];
    __shared__ __precision sum2[GPU_THREADS];

	int tid = threadIdx.x;
	//printf("%d -> %d responsible: %lld\n", threadIdx.x + blockIdx.x * blockDim.x, blockIdx.x, (n+GPU_THREADS-1LL)/GPU_THREADS);
    curandState state;
    curand_init((unsigned long long) clock() + 279LL*(threadIdx.x + blockIdx.x * blockDim.x) , 0, 0, &state);	

    __precision sum_thread = 0.;
    __precision sum2_thread = 0.;
    __precision x, f_x;
    for (ll i = 0; i < (n + GPU_THREADS - 1LL) / GPU_THREADS; i++) {
        x = curand_uniform(&state) / 2.0;
        f_x = f_device(m, k, x);
        sum_thread += f_x;
        sum2_thread += f_x * f_x;
    }    

    //compute block answer putting answer of each thread of the block in a 
    //array and then applying reduce at it
    sum[tid] = sum_thread;
    sum2[tid] = sum2_thread;
    __syncthreads();

    for (int i = GPU_THREADS / 2; i > 0; i = i / 2) {
        if (threadIdx.x >= i) return;
        sum[threadIdx.x] += sum[i + threadIdx.x];
        sum2[threadIdx.x] += sum2[i + threadIdx.x];
        __syncthreads();
    }

	if(tid == 0) {
        sum_block[blockIdx.x] = sum[0];
        sum2_block[blockIdx.x] = sum2[0];
	}
}

pff gpu(ll n, int m, int k, float* sum, float* sum2, ll *new_n) {
	int num_blocks = 1;
	for(ll i=2;i<=1024;i++) {
		if(n / (i * GPU_THREADS) >= 100) num_blocks = i;
	}

	ll total_threads = GPU_THREADS * num_blocks;
	*new_n = ((n + total_threads -1)/total_threads) * total_threads;
	n = *new_n;	

    __precision *d_sum_block;
    __precision *d_sum2_block;
    __precision *h_sum_block;
    __precision *h_sum2_block;
	checkCudaErrors(cudaMalloc(&d_sum_block, num_blocks * sizeof(__precision)));
	checkCudaErrors(cudaMalloc(&d_sum2_block, num_blocks * sizeof(__precision)));
    h_sum_block = (__precision *)calloc(num_blocks, sizeof(__precision));
    h_sum2_block = (__precision *)calloc(num_blocks, sizeof(__precision));

	checkCudaErrors(cudaMemcpy(d_sum_block, h_sum_block, num_blocks * sizeof(__precision),cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_sum2_block, h_sum2_block, num_blocks * sizeof(__precision),cudaMemcpyHostToDevice));
	partial_sum<<<num_blocks, GPU_THREADS>>>(n / num_blocks, m, k, d_sum_block, d_sum2_block);
	cudaDeviceSynchronize();
	
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaMemcpy(h_sum_block, d_sum_block, num_blocks*sizeof(__precision), cudaMemcpyDeviceToHost));
	checkCudaErrors(cudaMemcpy(h_sum2_block, d_sum2_block, num_blocks*sizeof(__precision), cudaMemcpyDeviceToHost));

	double aux1, aux2;
	aux1 = aux2 = 0;
    for (int i = 0; i < num_blocks; i++) {
		aux1 += h_sum_block[i];
		aux2 += h_sum2_block[i];
    }
	(*sum) = (float)aux1;
	(*sum2) = (float)aux2;


	checkCudaErrors(cudaFree(d_sum_block));
	checkCudaErrors(cudaFree(d_sum2_block));
	free(h_sum_block);
    free(h_sum2_block);

    return calc_res(n, *sum, *sum2);
}

void gpu_tester() {
	int tests = 10;
	ll n = 8192000000;
	float eps = 5e-3;
	printf("Testes: %d\nN: %lld\neps: %f\n\n", tests, n, eps);
	double porcentagem_acerto = testa_corretude(tests, n, eps, gpu, true);
	printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}

