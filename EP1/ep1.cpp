#pragma once
#include <cstdio>
#include <cstdlib>
#include <utility>
#include <pthread.h>
#include <string>
#include <iostream>
#include <cstring>

int THREAD_NUM = 4;
#define fi first
#define se second
#define ll long long
#define pll pair<ll, ll>
using namespace std;

#include "omp.h"
#include "util.cpp"
#include "./algorithms/sequential.cpp"
#include "./algorithms/openmp.cpp"
#include "./algorithms/pthread.cpp"

bool PRINT_FLAG = false;
bool TESTING = false;
bool CORRIGE = false;

double run(double (*func)(double**,pll,double**,pll,double**,pll),
		   double **A, pll dimA, double **B, pll dimB, double **C, pll dimC,
		   double **gabarito, pll dimG, 
		   string pathC, const string func_name) {

	omp_set_num_threads(THREAD_NUM);
	double ret = (*func)(A,dimA,B,dimB,C,dimC);
	string s = "./out/" + string(func_name) + "_" + to_string(dimA.fi) + "_t" + to_string(THREAD_NUM) + ".out";
	write(pathC, C, dimC);
	if(TESTING) write_stat(s, ret);
	if(PRINT_FLAG) cout << func_name << ": " << ret << " ms\n";
	if(CORRIGE) {
		if(checker(gabarito,dimG,C,dimC) == false) {
			cout << "Resultados errados para " << func_name << "\n";
			exit(0);
		} else if(PRINT_FLAG) cout << "Check " << func_name << " !" << "\n";
	}
	return ret;
}

void go(int argc, char* argv[]) {
	
	string pathA, pathB, pathC;
	char implementation;
	double **A, **B, **C, **gabarito;
	pll dimA, dimB, dimC, dimG;

	prepare_paths(argc, argv, pathA, pathB, pathC, implementation);
	if(!TESTING) { 
		cout << "Implementation: " << implementation << "\n";  
		cout << "pathA: " << pathA << "\n";
		cout << "pathB: " << pathB << "\n";
		cout << "pathC: " << pathC << "\n";
	}

	A = read(pathA, dimA, 0);
	B = read(pathB, dimB, 1);

	dimG = dimC = {dimA.fi, dimB.fi};
	gabarito = init(dimG.fi, dimG.se);
	C = init(dimC.fi, dimC.se);

	if(CORRIGE or implementation == 's') {
		run(sequential_mul,A,dimA,B,dimB,gabarito,dimG,gabarito,dimG,pathC,"seq");
	} 

/*	if(implementation == 'A' or implementation == 'O') {
		run(openmp1_mul,A,dimA,B,dimB,C,dimC,gabarito,dimG,pathC,"mp1");
	} 
*/
	if(implementation == 'A' or implementation == 'o') {
		run(openmp2_mul,A,dimA,B,dimB,C,dimC,gabarito,dimG,pathC,"mp2");
	} 

	if(implementation == 'A' or implementation == 'p') {
		run(pthread_mul,A,dimA,B,dimB,C,dimC,gabarito,dimG,pathC,"pth");
	} 
	
	free_mat(A, dimA);
	free_mat(B, dimB);
	free_mat(C, dimC);
	free_mat(gabarito, dimG);
}


bool check_for_test(char* argv[]) {
	if(strcmp("--test", argv[5]) == 0) {
		TESTING = true;
		return 1;
	}
	return 0;
}

int main (int argc, char* argv[]) {
	PRINT_FLAG = true;
	int aux;
	if(argc >= 6 and check_for_test(argv)) {
		int n, d , quadrada, mod;
		printf("Quantos testes?\n");
		scanf("%d", &n);

		printf("Matrizes quadradas?\n");
		scanf("%d", &quadrada);
		quadrada = (quadrada >= 1);
		
		if(quadrada == 1) {
			printf("Dimensao das matrizes quadradas?\n");
			scanf("%d", &d);
		} else d = -1;
		
		printf("Corrigir os resultados usando o resultado sequencial?\n");
		scanf("%d", &aux);
		CORRIGE = (aux >= 1);

		printf("Quantas threads?\n");
		scanf("%d", &THREAD_NUM);
		
		system("make generate");
		string s = "./generate " + to_string(d) + " 0";
		
		for(int i=0;i<n;i++) {
			if(i % 5 == 0) {
				PRINT_FLAG= true; 
				printf("\n-------TESTE %d--------\n", i);
				s.pop_back(), s.push_back('1');
				system(s.c_str(	));
				s.pop_back(), s.push_back('0');
			} else system(s.c_str());

			go(argc, argv);
			PRINT_FLAG = false;
		}
	} else go(argc, argv);

	return 0;
}



