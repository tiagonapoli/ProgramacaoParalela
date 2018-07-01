CC=g++ -Wall -pthread 
H_PATH=headers/
IMP_PATH=./implementacoes

main: main.o utils.o sequencial.o pthreads.o gpu.o 
	mpic++ -pthread -lcudart -L /usr/local/cuda/lib64/ $^ -o $@

main.o: main.cpp
	mpic++ -I $(H_PATH) -c $^ 

sequencial.o: $(IMP_PATH)/sequencial.cpp
	$(CC) -I $(H_PATH) -c $^ 

pthreads.o: ./implementacoes/pthreads.cpp  ./headers/pthreads.h
	$(CC) -I $(H_PATH) -c pthreads.cpp 

utils.o: ./implementacoes/utils.cpp ./headers/utils.h
	$(CC) -I $(H_PATH) -c ./implementacoes/utils.cpp 

gpu: gpu.o utils.o
	nvcc -o gpu gpu.o utils.o

gpu.o: ./implementacoes/gpu.cu ./headers/gpu.h
	nvcc -I inc/ -c ./implementacoes/gpu.cu

clean:
	rm *.o main sequencial pthreads gpu
