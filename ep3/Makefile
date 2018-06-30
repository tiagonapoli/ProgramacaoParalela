all: sequencial pthreads gpu

CC=g++ -Wall -pthread 

sequencial: sequencial.o utils.o
	$(CC) $^ -o $@

sequencial.o: sequencial.cpp
	$(CC) -c $^ 

pthreads: pthreads.o utils.o
	$(CC) $^ -o $@

pthreads.o: pthreads.cpp
	$(CC) -c $^ 

utils.o: utils.cpp utils.h
	$(CC) -c utils.cpp 

gpu: gpu.cu utils.o
	nvcc -I inc/ -o gpu gpu.cu

clean:
	rm *.o main sequencial pthreads gpu