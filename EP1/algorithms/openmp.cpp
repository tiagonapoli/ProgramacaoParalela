#pragma once
#include "../ep1.cpp"

double openmp1_mul(double **A, pll dimA, double **B, pll dimB, double **C, pll dimC) {
	//Parallel on B lines. Sequential dot product
	double t1,t2;
	t1 = omp_get_wtime();

	for(ll i=0;i<dimA.fi;i++) {
		#pragma omp parallel for
		for(ll j=0;j<dimB.fi;j++) {
			C[i][j] = dot_product(A[i], B[j], dimA.se);
		}
	}

	t2 = omp_get_wtime();
	return (t2-t1)*1e3;
}

double openmp2_mul(double **A, pll dimA, double **B, pll dimB, double **C, pll dimC) {
	//Parallel on A lines. Sequential dot product
	double t1,t2;
	t1 = omp_get_wtime();

	#pragma omp parallel for
	for(ll i=0;i<dimA.fi;i++) {
		for(ll j=0;j<dimB.fi;j++) {
			C[i][j] = dot_product(A[i], B[j], dimA.se);
		}
	}

	t2 = omp_get_wtime();
	return (t2-t1)*1e3;
}
