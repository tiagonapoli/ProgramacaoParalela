generate: generate.cpp
	g++ -o generate generate.cpp

reduce_seq: reduce_seq.cpp utils.cpp
	g++ -o reduce_seq reduce_seq.cpp

main: reduce.cu utils.cpp
	nvcc -I inc/ -o main reduce.cu

clean:
	rm main reduce_seq generate in test *.out 

test: test.cpp generate main reduce_seq 
	g++ -o test test.cpp
	./test
