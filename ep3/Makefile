all: sequencial pthreads

CC=g++ -Wall -pthread 

sequencial: sequencial.o utils.o utils.h
	$(CC) $^ -o $@

sequencial.o: sequencial.cpp
	$(CC) -c $^ 

utils.o: utils.cpp
	$(CC) -c $^ 

clean: 
	rm *.o 