CC=g++ -Wall -pthread 

main: main.o utils.o sequencial.o pthreads.o gpu.o 
	mpic++ -pthread -lcudart -L /usr/local/cuda/lib64/ $^ -o $@

main.o: main.cpp
	mpic++ -c $^ 

sequencial.o: ./implementacoes/sequencial.cpp
	$(CC) -c $^ 

pthreads.o: ./implementacoes/pthreads.cpp  ./headers/pthreads.h
	$(CC) -c pthreads.cpp 

utils.o: ./implementacoes/utils.cpp ./headers/utils.h
	$(CC) -c ./implementacoes/utils.cpp 

gpu: gpu.o utils.o
	nvcc -o gpu gpu.o utils.o

gpu.o: ./implementacoes/gpu.cu ./headers/gpu.h
	nvcc -I inc/ -c ./implementacoes/gpu.cu

clean:
	rm *.o main sequencial pthreads gpu
