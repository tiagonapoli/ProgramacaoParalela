#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "utils.cpp"

int main(int argc, char **argv) {

	int n;
	int ***x = read(argc, argv, &n);

	int res[3][3];

	clock_t t1 = clock();
	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
            res[i][j] = x[i][j][0];
            for(int k=0;k<n;k++) {
                res[i][j] = min_seq(res[i][j], x[i][j][k]);
            }
		}
	}
	clock_t t2 = clock();
	
	FILE* f = fopen("time_seq.out", "w");
	fprintf(f, "%.0lf ", (double)(t2-t1)*1000000/CLOCKS_PER_SEC);

	print_matrix(res);
}


