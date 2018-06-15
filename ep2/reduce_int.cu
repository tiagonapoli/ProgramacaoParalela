#include <stdio.h>
#include <helper_cuda.h>

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

inline int min(int a, int b) {
	return a > b ? a : b;
}

int*** read(char *path, int &n) {
	FILE* f = fopen(path, "r");
	
	fscanf(f, "%d", &n);
	
	int*** x;
	x = (int***) malloc(3*sizeof(int**));
	for(int i=0;i<3;i++) {
		x[i] = (int**) malloc(3*sizeo(int*));
		for(int j=0;j<3;j++) {
			x[i][j] = (int*) malloc(n * sizeof(int));
		}
	}

	for(int i=0;i<n;i++) {
		fscanf(f, "%*s");
		for(int j=0;j<3;j++) {
			for(int k=0;k<3;k++) {
				fscanf(f, "%d", &x[j][k][i]);
			}
		}
	}

	fclose(f);
	return x;
}

int solve(int **x, int n) {
	int res = x[0];
	for(int i=0;i<n;i++) {
		res = min(res, x[i]);
	}
	return res;
}

int solve_for_position(int n, int blocks, int *h_x, int *h_block_min, int *d_x, int *d_block_min) {
	
	checkCudaErrors(cudaMemcpy(d_x,h_x,n*sizeof(int),cudaMemcpyHostToDevice));

	reduce<<<blocks,threads,threads * sizeof(int)>>>(d_x,d_block_min);
	checkCudaErrors(cudaGetLastError());
	cudaDeviceSynchronize();

	checkCudaErrors(cudaMemcpy(h_block_min,d_block_min,blocks*sizeof(int),cudaMemcpyDeviceToHost));
	int res = 0;
	for(int i=0;i<blocks;i++) {
		res += h_block_min[i];
	}
	return res;
}

int main(int argc, char **argv) {
	
	int devID;
	cudaDeviceProp props;
	devID = findCudaDevice(argc, (const char**) argv);
	checkCudaErrors(cudaGetDeviceProperties(&props, devID));

	int n = 1 << 22;
	int threads = 32;
	int blocks = (n + 2 * threads - 1)/ (2 * threads);
	
	int *h_x, *h_block_min, *d_x, *d_block_min;
	h_x = (int*) calloc(n, sizeof(int));
	h_block_min = (int*) calloc(blocks, sizeof(int));
	if(!h_x or !h_block_min) {
		fprintf(stderr, "Deu ruim no calloc");
		exit(0);
	}
	checkCudaErrors(cudaMalloc(&d_x,n*sizeof(int)));
	checkCudaErrors(cudaMalloc(&d_block_min,blocks*sizeof(int)));



	checkCudaErrors(cudaFree(d_block_min));
	checkCudaErrors(cudaFree(d_x));
	free(h_x);
	free(h_block_min);

}
