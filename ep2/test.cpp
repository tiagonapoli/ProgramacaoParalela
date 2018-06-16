#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool comp(const char* cuda, const char* seq) {
    int a, b;
    a = strlen(cuda);
    b = strlen(seq);

    if(a != b) return false;
    for(int i=0;i<a;i++) {
        if(cuda[i] != seq[i]) return false;
    }
    return true;
}

char* read(const char* path) {
    FILE* f = fopen(path, "r");
    char* ret = (char*) malloc(1000*sizeof(char));
    if(!ret) {
        fprintf(stderr, "Erro em %s -> linha %d\n", __FILE__, __LINE__);
        exit(0);
    }
    int pos = 0;
    while(fscanf(f, "%c", &ret[pos]) != EOF) {
        pos++;
    }
    ret[pos] = '\0';
    fclose(f);
    return ret;
}

bool check() {
    char *a, *b;
    a = read("gabarito.out");
    b = read("cuda_res.out");
    if(comp(b,a) == false) {
		fprintf(stderr, "Resultados diferentes!!!!!\n");
    	printf("Gabarito:\n[%s]\n", a);
    	printf("Cuda:\n[%s]\n", b);
		exit(0);
    }
    return true;
}

long long read_time(const char* p) {
	FILE *f = fopen(p, "r");
	long long x;
	fscanf(f, "%lld", &x);
	fclose(f);
	return x;
}

int main() {
    
    int n=1;

	int gerar;
	printf("Gerar aleatorio?\n");
	scanf("%d", &gerar);
	
	int testes = 1;
	if(gerar != 0) {
		printf("Quantas matrizes?\n");
		scanf("%d", &n);
		printf("Quantos testes?\n");
		scanf("%d", &testes);

	}
    char ordem[40];
	char stats1[50];
	char stats2[50];
	sprintf(ordem, "./generate %d", n);
	sprintf(stats1, "cat time_cuda.out >> ./stats/%d_matrices_cuda.out", n);
	sprintf(stats2, "cat time_seq.out >> ./stats/%d_matrices_seq.out", n);

	bool print_flag = 0;
    for(int i=0;i<testes;i++) {
		if(i % 10 == 0) print_flag = 1;
		if(print_flag) printf("=== Teste %d ===\n",i);
		if(gerar != 0) {
			if(print_flag) printf("Gerando...\n");
			system(ordem);
		}
		if(print_flag) printf("Sequencial...\n");
        system("./reduce_seq in > gabarito.out");
		if(print_flag) printf("Cuda...\n");
        system("./main in > cuda_res.out");
        if(print_flag) printf("Checking...\n");
		bool r = check();
		if(print_flag) printf("%s\n", r ? "OK!" : "Error"); 
		if(print_flag) printf("\n");

		if(print_flag) printf("Time CUDA: %.3lf ms\n", read_time("time_cuda.out")/1000.0);
		if(print_flag) printf("Time SEQ:  %.3lf ms\n", read_time("time_seq.out")/1000.0);
		print_flag = 0;

		system(stats1);
		system(stats2);
    }

}

