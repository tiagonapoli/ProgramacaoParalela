#pragma once
#include <stdlib.h>
#include <stdio.h>

inline int min_seq(int a, int b) {
	return a < b ? a : b;
}

int next_pot(int x) {
	if(x == 0) return 0;
    while(__builtin_popcount(x) > 1) {
        x += x & (-x);
    }
	while(x < 64) x *= 2;
    return x;
}


int*** read(int argc, char** argv, int *n) {

    if(argc < 2) {
        printf("Usage: ./a.out path_to_matrices_archive\n");
        exit(0);    
    }

	FILE* f = fopen(argv[1], "r");
	
	fscanf(f, "%d", n);
	
	int*** x;
	x = (int***) malloc(3*sizeof(int**));
	if(!x) {
        fprintf(stderr, "erro no malloc em %s linha %d\n", __FILE__, __LINE__);
        exit(0);
    }
    for(int i=0;i<3;i++) {
		x[i] = (int**) malloc(3*sizeof(int*));
        if(!x) {
            fprintf(stderr, "erro no malloc em %s linha %d\n", __FILE__, __LINE__);
            exit(0);
        }
		for(int j=0;j<3;j++) {
			x[i][j] = (int*) malloc(next_pot(*n) * sizeof(int));
		}
	}

	for(int i=0;i<*n;i++) {
		fscanf(f, "%*s");
		for(int j=0;j<3;j++) {
			for(int k=0;k<3;k++) {
				fscanf(f, "%d", &x[j][k][i]);
			}
		}
	}

    int sz = next_pot(*n);
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            for(int k=*n;k<sz;k++) {
                x[i][j][k] = x[i][j][0];
            }
        }
    }

    *n = sz;
	fclose(f);
	return x;
}

void print_matrix(int x[3][3]) {
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++) {
            printf("%d ", x[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void print_all_matrices(int *** x, int n) {
    for(int i=0;i<n;i++) {
        for(int j=0;j<3;j++) {
            for(int k=0;k<3;k++) {
                printf("%d ", x[j][k][i]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}

