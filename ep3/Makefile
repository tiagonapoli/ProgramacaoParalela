all: sequencial pthreads gpu main

CC=g++ -Wall -pthread 

main: main.o utils.o sequencial.o pthreads.o gpu.o 
	mpic++ -pthread -lcudart -L /usr/local/cuda/lib64/ $^ -o $@

main.o: main.cpp
	mpic++ -c $^ 

#sequencial: sequencial.o utils.o
#	$(CC) $^ -o $@

sequencial.o: sequencial.cpp
	$(CC) -c $^ 

#pthreads: pthreads.o utils.o
#	$(CC) $^ -o $@

pthreads.o: pthreads.cpp pthreads.h
	$(CC) -c pthreads.cpp 

utils.o: utils.cpp utils.h
	$(CC) -c utils.cpp 

gpu: gpu.o utils.o
	nvcc -o gpu gpu.o utils.o

gpu.o: gpu.cu gpu.h
	nvcc -I inc/ -c gpu.cu

clean:
	rm *.o main sequencial pthreads gpu
