#include <stdio.h>
#include <helper_cuda.h>

__global__ void vadd(float* a, float *b, float *c, int n) {
	int id = blockIdx.x * blockDim.x + threadIdx.x;
	int pulo = gridDim.x * blockDim.x;
	//printf("%d %d\n", id, pulo);
	for(int i=id;i<n;i+=pulo) {
		//printf("%d\n", i);
		c[i] = a[i] + b[i];
	}
}

inline void gera(float *x, int n) {
	for(int i=0;i<n;i++) {
		x[i] = rand()/(float)(1 + rand());
	}
}

inline void check_result(float* a, float *b, float *c, int n) {
	printf("Checking result!...\n");
	for(int i=0;i<n;i++) {
		if(fabs(a[i] + b[i] - c[i]) > 1e-5) {
			fprintf(stderr, "Erro nos calculos...");
			exit(0);
		}
	}
	printf("OK!!\n");
}

int main(int argc, char** argv) {
	
	int devID;
	cudaDeviceProp props;

	devID = findCudaDevice(argc,(const char**) argv);
	checkCudaErrors(cudaGetDeviceProperties(&props, devID));

	printf("Device %d. Name %s\n", devID, props.name);

	int n = 1 << 20;
	printf("Size %d\n", n);
	float *h_x,*h_y,*h_z;
	h_x = (float*) malloc(n*sizeof(float));
	h_y = (float*) malloc(n*sizeof(float));
	h_z = (float*) malloc(n*sizeof(float));
	if(h_z == NULL or h_x == NULL or h_y == NULL) {
		fprintf(stderr, "Deu ruim no malloc");
		exit(0);
	}

	gera(h_x,n);
	gera(h_y,n);

	float *d_x, *d_y, *d_z;
	d_x = d_y = d_z = NULL;
	checkCudaErrors(cudaMalloc(&d_x, n*sizeof(float)));
	checkCudaErrors(cudaMalloc(&d_y, n*sizeof(float)));
	checkCudaErrors(cudaMalloc(&d_z, n*sizeof(float)));

	checkCudaErrors(cudaMemcpy(d_x,h_x,n*sizeof(float),cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(d_y,h_y,n*sizeof(float),cudaMemcpyHostToDevice));

	int threads = 64;
	int blocks = 128;
	vadd<<<blocks,threads>>>(d_x,d_y,d_z,n);
	checkCudaErrors(cudaGetLastError());
	checkCudaErrors(cudaMemcpy(h_z,d_z,n*sizeof(float),cudaMemcpyDeviceToHost));
	check_result(h_x,h_y,h_z,n);
	
	checkCudaErrors(cudaFree(d_x));
	checkCudaErrors(cudaFree(d_y));
	checkCudaErrors(cudaFree(d_z));
	free(h_x);
	free(h_y);
	free(h_z);

}
