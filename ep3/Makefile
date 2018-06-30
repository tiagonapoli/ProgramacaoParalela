all: sequencial pthreads

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

clean: 
	rm *.o 

clean_all:
	rm *.o sequencial pthreads