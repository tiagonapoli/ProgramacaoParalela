#include <stdio.h>
#include <helper_cuda.h>

__global__ void reduce(float *g_idata, float *g_odata) {
	//vetor com sizeof(float) * BLOCK_SIZE
	extern __shared__ float sdata[];
	
	int tid = threadIdx.x;
	int i = blockIdx.x * 2*blockDim.x + threadIdx.x;
	sdata[tid] = g_idata[i] + g_idata[i + blockDim.x];
	__syncthreads();

	//do reduction in shared memory
	for(int s=blockDim.x/2;s > 0; s /= 2) {
		if(tid < s) {
			sdata[tid] += sdata[tid+s]; 
		}
		__syncthreads();
	}

	if(tid == 0) g_odata[blockIdx.x] = sdata[0];
	

}

void gera(float *x, int n) {
	for(int i=0;i<n;i++) {
		x[i] = (float)i;
		//x[i] = rand()/(float)RAND_MAX;
	}
}

void check_block_sums(float *x, float *bsum, int blocks, int n) {
	printf("Blocks %d  n: %d   Total: %d\n", blocks, n, blocks*32*2);
	for(int i=0;i<blocks;i++) {
		float r = 0;
		for(int j=0;j<32*2;j++) {
			r += x[j + i*32*2];
		}
		printf("Gab: %f\nSum: %f\n\n", r, bsum[i]);
		if(fabs(bsum[i] - r) > 1e-5) {
			fprintf(stderr, "Erro no resultado!!");
			exit(0);
		}
	}

}

int main(int argc, char **argv) {
	
	int devID;
	cudaDeviceProp props;
	devID = findCudaDevice(argc, (const char**) argv);
	checkCudaErrors(cudaGetDeviceProperties(&props, devID));

	printf("Device %d - %s\n", devID, props.name);

	int n = 1 << 6;
	int threads = 32;
	int blocks = (n + 2 * threads - 1)/ (2 * threads);
	
	float *h_x, *h_block_sums, *d_x, *d_block_sums;
	h_x = (float*) calloc(n, sizeof(float));
	h_block_sums = (float*) calloc(blocks, sizeof(float));
	if(!h_x or !h_block_sums) {
		fprintf(stderr, "Deu ruim no calloc");
		exit(0);
	}

	checkCudaErrors(cudaMalloc(&d_x,n*sizeof(float)));
	checkCudaErrors(cudaMalloc(&d_block_sums,blocks*sizeof(float)));

	gera(h_x,n);
	checkCudaErrors(cudaMemcpy(d_x,h_x,n*sizeof(float),cudaMemcpyHostToDevice));

	reduce<<<blocks,threads,threads * sizeof(float)>>>(d_x,d_block_sums);
	checkCudaErrors(cudaGetLastError());
	
	checkCudaErrors(cudaMemcpy(h_block_sums,d_block_sums,blocks*sizeof(float),cudaMemcpyDeviceToHost));
	float res = 0;
	for(int i=0;i<blocks;i++) {
		res += h_block_sums[i];
	}

	check_block_sums(h_x,h_block_sums,blocks,n);

	checkCudaErrors(cudaFree(d_block_sums));
	checkCudaErrors(cudaFree(d_x));
	free(h_x);
	free(h_block_sums);

}
