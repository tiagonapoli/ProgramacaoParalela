main: ep1.cpp util.cpp ./algorithms/pthread.cpp ./algorithms/sequential.cpp ./algorithms/openmp.cpp
	g++ -pthread -fopenmp -o main ep1.cpp

generate: generate.cpp
	g++ -o generate generate.cpp

clean:
	rm main generate

testall: main
	./main A ./input/a.in ./input/b.in ./out/c.out --test

testmp1: main
	./main O ./input/a.in ./input/b.in ./out/c.out --test

testmp2: main
	./main o ./input/a.in ./input/b.in ./out/c.out --test

testpthread: main
	./main p ./input/a.in ./input/b.in ./out/c.out --test