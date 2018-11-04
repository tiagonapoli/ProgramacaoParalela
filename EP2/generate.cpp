#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

void write(const char* path, int ***x, int n) {
    FILE *f = fopen(path, "w");
    fprintf(f, "%d\n***\n", n);
    for(int i=0;i<n;i++) {
        for(int j=0;j<3;j++) {
            for(int k=0;k<3;k++) {
                fprintf(f, "%d ", x[i][j][k]);
            }
            fprintf(f, "\n");
        }
        fprintf(f, "***\n");
    }
    fclose(f);
}

void generate(int n) {
    int ***x;
    
    x = (int***) malloc(n * sizeof(int**));
    if(!x) {
        fprintf(stderr, "erro em generate line %d\n", __LINE__);
        exit(0);
    }
    for(int i=0;i<n;i++) {
        x[i] = (int**) malloc(3 * sizeof(int*));
        if(!x[i]) {
            fprintf(stderr, "erro em generate line %d\n", __LINE__);
            exit(0);
        }
        for(int j=0;j<3;j++) {
            x[i][j] = (int*) malloc(3 * sizeof(int));
            if(!x[i][j]) {
                fprintf(stderr, "erro em generate line %d\n", __LINE__);
                exit(0);
            }
        }
    }

    srand(time(NULL));
    for(int i=0;i<n;i++) {
        for(int j=0;j<3;j++) {
            for(int k=0;k<3;k++) {
                x[i][j][k] = rand();
            }
        }
    }

    write("in", x, n);
    for(int i=0;i<n;i++) {
        for(int j=0;j<3;j++) {
            free(x[i][j]);
        }
        free(x[i]);
    }
    free(x);
}

int main(int argc, char** argv) {

    int n;

    if(argc < 2) {
        printf("Usage: ./generate qtd_de_matrizes\n");
        exit(0);
    }
    
    sscanf(argv[1], "%d", &n);
    generate(n);
}
