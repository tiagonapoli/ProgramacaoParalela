FLAGS_NVCC = -I inc/ -I headers/  
#FLAGS = -Wall -pthread -I inc/ -I headers/ -lcudart -L /usr/local/cuda/lib64 
FLAGS = -Wall -pthread -I inc/ -I headers/  


H_PATH=headers
IMP_PATH=implementacoes

tester: tester.cpp pthreads.o utils.o sequencial.o balanceado.o gpu.o
	mpic++ $(FLAGS) $^ -o $@

main: main.cpp utils.o sequencial.o pthreads.o gpu.o 
	mpic++ $(FLAGS) $^ -o $@


balanceado.o: $(IMP_PATH)/balanceado.cpp $(H_PATH)/balanceado.h
	mpic++ $(FLAGS) -c $<

sequencial.o: $(IMP_PATH)/sequencial.cpp $(H_PATH)/sequencial.h
	g++ $(FLAGS) -c $< 

gpu.o: $(IMP_PATH)/gpu.cu $(H_PATH)/gpu.h
	nvcc $(FLAGS_NVCC) -c $<

pthreads.o: $(IMP_PATH)/pthreads.cpp  $(H_PATH)/pthreads.h
	g++ $(FLAGS) -c $< 

utils.o: $(IMP_PATH)/utils.cpp $(H_PATH)/utils.h
	g++ $(FLAGS) -c $<

clean:
	rm *.o main tester
