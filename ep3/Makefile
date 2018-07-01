all: sequencial pthreads gpu main

CC=g++ -Wall -pthread 

main: main.o utils.o sequencial.o pthreads.o 
	mpic++ -pthread $^ -o $@

main.o: main.cpp
	$(CC) -c $^ 

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

gpu: gpu.cu utils.o
	nvcc -I inc/ -o gpu gpu.cu utils.o

clean:
	rm *.o main sequencial pthreads gpu
