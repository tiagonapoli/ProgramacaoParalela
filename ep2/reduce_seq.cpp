#include <stdio.h>
#include <stdlib.h>
#include "utils.cpp"

int main(int argc, char **argv) {

	int n;
	int ***x = read(argc, argv, &n);

	int res[3][3];

	for(int i=0;i<3;i++) {
		for(int j=0;j<3;j++) {
            res[i][j] = x[i][j][0];
            for(int k=0;k<n;k++) {
                res[i][j] = min(res[i][j], x[i][j][k]);
            }
		}
	}

	print_matrix(res);
}


