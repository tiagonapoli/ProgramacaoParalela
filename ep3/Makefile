all: sequencial pthreads

UNAME_S := $(shell uname -s)

#ifeq ($(UNAME_S),Linux)
	CC=g++ -Wall -pthread 
#endif
#ifeq ($(UNAME_S),Darwin)
	#CC=g++-7 -pthread
#endif

sequencial: sequencial.o utils.o 
	$(CC) $^ -o $@

sequencial.o: sequencial.cpp utils.o
	$(CC) $^ -o $@

utils.o: utils.cpp utils.h
	$(CC) $^ -o $@

clean: 
	rm *.o