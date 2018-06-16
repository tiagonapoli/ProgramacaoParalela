#include <stdio.h>
#include <stdlib.h>
#include <helper_cuda.h>
#include "utils.cpp"

__device__ inline int min(int a, int b) {
	if(a > b) {
		return a;
	}
	return b;
}

__global__ void reduce(int *in, int *out) {
	//vetor com sizeof(int) * BLOCK_SIZE
	extern __shared__ int sdata[];
	
	int tid = threadIdx.x;
	int i = blockIdx.x * 2*blockDim.x + threadIdx.x;
	sdata[tid] = in[i] + in[i + blockDim.x];
	__syncthreads();

	//do reduction in shared memory
	for(int s=blockDim.x/2;s > 0; s /= 2) {
		if(tid < s) {
			sdata[tid] = min(sdata[tid], sdata[tid+s]); 
		}
		__syncthreads();
	}

	if(tid == 0) {
		out[blockIdx.x] = sdata[0];
	}
}

int solve_for_position(int n, int blocks, int *h_x, int *h_block_min, int *d_x, int *d_block_min) {
	checkCudaErrors(cudaMemcpy(d_x,h_x,n*sizeof(int),cudaMemcpyHostToDevice));
	reduce<<<blocks,threads,threads * sizeof(int)>>>(d_x,d_block_min);
	checkCudaErrors(cudaGetLastError());
	cudaDeviceSynchronize();
	checkCudaErrors(cudaMemcpy(h_block_min,d_block_min,blocks*sizeof(int),cudaMemcpyDeviceToHost));
	int res = h_block_min[0];
	for(int i=0;i<blocks;i++) {
		res = min(res, h_block_min[i]);
	}
	return res;
}

int main(int argc, char **argv) {
	
	int devID;
	cudaDeviceProp props;
	devID = findCudaDevice(argc, (const char**) argv);
	checkCudaErrors(cudaGetDeviceProperties(&props, devID));

	int n;
	int ***h_x = read(argc, argv, &n);
	
	int threads = 32;
	int blocks = (n + 2 * threads - 1)/ (2 * threads);
	
	int *h_block_min, *d_x, *d_block_min;
	h_block_min = (int*) calloc(blocks, sizeof(int));
	if(!h_x or !h_block_min) {
		fprintf(stderr, "Deu ruim no calloc");
		exit(0);
	}

	checkCudaErrors(cudaMalloc(&d_x,n*sizeof(int)));
	checkCudaErrors(cudaMalloc(&d_block_min,blocks*sizeof(int)));

	int res[3][3];

	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
			res[i][j] = solve_for_position(n,blocks,h_x[i][j],h_block_min,d_x,d_block_min);
		}
	}

	print_matrix(res);

	checkCudaErrors(cudaFree(d_block_min));
	checkCudaErrors(cudaFree(d_x));
	free(h_x);
	free(h_block_min);

}
