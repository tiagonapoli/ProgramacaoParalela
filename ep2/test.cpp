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
    while(fscanf(f, "%c", &ret[pos])) {
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
        exit(0);
    }
    return true;
}


int main() {
    
    int n, lim;
    
    printf("Quantas matrizes?\n");
    scanf("%d", &n);
    printf("Qual o limite pra o maior numero?\n");
    scanf("%d", &lim);

    int testes;
    printf("Quantos testes?\n");
    scanf("%d", &testes);

    char ordem[40];
    sprintf(ordem, "./generate %d %d", n, lim);

    for(int i=0;i<testes;i++) {
        system(ordem);
        system("./reduce_seq in > gabarito.out");
        system("./main int > cuda_res.out");
        check();
    }

}

