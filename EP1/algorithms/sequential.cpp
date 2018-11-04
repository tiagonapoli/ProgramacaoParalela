#pragma once
#include "../ep1.cpp"

inline double dot_product(double *x, double *y, ll sz) {
	double res = 0.0;
	for(ll i=0;i<sz;i++) {
		res += x[i] * y[i];
	}
	return res;
}

double sequential_mul(double **A, pll dimA, double **B, pll dimB, double **C, pll dimC) {
	double t1,t2;
	t1 = omp_get_wtime();
	for(ll i=0;i<dimA.fi;i++) {
		for(ll j=0;j<dimB.fi;j++) {
			C[i][j] = dot_product(A[i], B[j], dimA.se);
		}
	}

	t2 = omp_get_wtime();
	return (t2-t1)*1e3;
}
