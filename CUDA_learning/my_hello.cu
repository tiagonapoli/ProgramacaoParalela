#include <stdio.h>
#include <helper_cuda.h>

__global__ void printa() {
	printf("%d, %d\n", blockIdx.y*gridDim.x + blockIdx.x, \
					   threadIdx.z*blockDim.x*blockDim.y + threadIdx.y*blockDim.x + threadIdx.x);
}

int main(int argc, char** argv) {
	int devID;
	cudaDeviceProp props;

	devID = findCudaDevice(argc, (const char**) argv);

	checkCudaErrors(cudaGetDevice(&devID));
	checkCudaErrors(cudaGetDeviceProperties(&props, devID));

	printf("%d -> %s\n", devID, props.name);

	dim3 dimGrid(2,2);
	dim3 dimBlock(2,2,2);
	printa<<<dimGrid, dimBlock>>>();
	cudaDeviceSynchronize();

}
