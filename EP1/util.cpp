#pragma once
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <cmath>

#define fi first
#define se second
#define ll long long
#define pll pair<ll, ll>
double eps = 1e-6;
using namespace std;

void prepare_paths(int argc, char* argv[], string &pathA, string &pathB, string &pathC, char &implementation) {
	if(argc >= 5) {
		implementation = argv[1][0];
		pathA = string(argv[2]);
		pathB = string(argv[3]);
		pathC = string(argv[4]);
	} else {
		printf("Usage: main <implementação> <caminho_matr_A> <caminho_matr_B> <caminho_matr_C>\n");
		exit(0);
	}
}

void zerar(double** v, ll x, ll y) {
	for(ll i=0;i<x;i++) {
		for(ll j=0;j<y;j++) {
			v[i][j] = 0.0;
		}
	}
}

double** init(ll x, ll y) {
	double **v;
	v = (double**) malloc(x*sizeof(double*));
  	if(v==NULL) {
        printf("init(%lld,%lld): Error! memory not allocated on v", x, y);
        exit(0);
    }
	for(ll i=0;i<x;i++) {
		v[i] = (double*) malloc(y*sizeof(double));
		if(v==NULL) {
       		printf("init(%lld,%lld): Error! memory not allocated on v[%lld]", x, y, i);
        	exit(0);
    	}
	}

	zerar(v,x,y);
	return v;
}


double** read(string path, pll &dim, bool transpose) {
	FILE* f = fopen(path.c_str(), "r");
	ll x,y;
	double val;
	double** mat;
	
	fscanf(f, "%lld %lld", &dim.fi, &dim.se);
	if(transpose) swap(dim.fi, dim.se);
	mat = init(dim.fi, dim.se);

	while(fscanf(f, "%lld %lld %lf", &x, &y, &val) != EOF) {
		x--;
		y--;
		if(transpose) mat[y][x] = val;
		else mat[x][y] = val;	
	}
	fclose(f);
	return mat;
}


void print(double** m, pll dim, bool transpose) {
	if(transpose) swap(dim.fi, dim.se);
	printf("\nPrinta matriz %lld x %lld\n", dim.fi, dim.se);
	for(ll i=0;i<dim.fi;i++) {
		for(ll j=0;j<dim.se;j++) {
			if(transpose) printf("%4.2f ", m[j][i]);
			else printf("%4.2f ", m[i][j]);

		}
		printf("\n");
	}
	printf("\n");
	
}

void write_stat(string path, double x) {
	FILE* f = fopen(path.c_str(), "a");
	fprintf(f, "%lf ", x);
	fclose(f);
}

void write(string path, double **v, pll dim) {
	FILE* f = fopen(path.c_str(), "w");
	fprintf(f, "%lld %lld\n", dim.fi, dim.se);
	for(ll i=0;i < dim.fi; i++) {
		for(ll j=0;j < dim.se; j++) {
			if(fabs(v[i][j]) >= eps) fprintf(f, "%lld %lld %lf\n",i+1,j+1, v[i][j]);
		}
	}
	fclose(f);
}

inline void free_mat(double **mat, pll dim) {
	for(ll i=0;i<dim.fi;i++) {
		free(mat[i]);
	}
	free(mat);
}

bool checker(double** gabarito, pll dimG, double** C, pll dimC) {
	if(dimG != dimC) return false;
	for(ll i=0;i < dimG.fi; i++) {
		for(ll j=0; j< dimG.se; j++) {
			if(fabs(gabarito[i][j]-C[i][j]) > eps) return false;
		}
	}
	return true;
}
