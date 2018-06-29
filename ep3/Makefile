all: sequencial pthreads

CC=g++ -Wall -pthread 

sequencial: sequencial.o utils.o utils.h
	$(CC) $^ -o $@

utils.o: utils.cpp
	$(CC) -c $^ 

clean: 
	rm *.o 