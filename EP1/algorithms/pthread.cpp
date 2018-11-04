#pragma once
#include "../ep1.cpp"

struct thread_data {
	int thread_id;
	double **A, **B, **C;
	pll dimA, dimB, dimC;
	ll l_ini, l_fim;

    thread_data(){}

	thread_data(int id,double **A, pll dimA, double **B, pll dimB, double **C, pll dimC) {
		this->thread_id = id;
		this->A = A;
		this->B = B;
		this->C = C;
		this->dimA = dimA;
		this->dimB = dimB;
		this->dimC = dimC;
		this->l_ini = (id * dimA.fi)/THREAD_NUM;
		this->l_fim = ((id+1) * dimA.fi)/THREAD_NUM;
	}
};

void *worker(void *thread_arg) {
	thread_data *args;
	args = (thread_data*) thread_arg;
    //printf("ID %d\n[%lld->%lld[\n\n",args->thread_id, args->l_ini, args->l_fim);
	for(ll i=args->l_ini; i<args->l_fim; i++) {
		for(ll j=0; j<args->dimB.fi; j++) {
		//	printf("[%d] %d,%d\n", args->thread_id, i, j);
			args->C[i][j] = dot_product(args->A[i], args->B[j], args->dimA.se);
		}
	}
	pthread_exit(NULL);
}

double pthread_mul(double **A, pll dimA, double **B, pll dimB, double **C, pll dimC) {
    pthread_t threads[THREAD_NUM];
	thread_data td[THREAD_NUM];
	int ret;
    double t1,t2;
    t1 = omp_get_wtime();
	for(int i=0;i<THREAD_NUM;i++) {
		td[i] = thread_data(i,A,dimA,B,dimB,C,dimC);
		ret = pthread_create(&threads[i], NULL, worker, (void*)&td[i]);
		if(ret) {
			cout << "Error: unable to create pthread " << ret << endl;
			exit(-1);
		}
	}

	void *status;
	for(int i=0;i<THREAD_NUM;i++) {
		ret = pthread_join(threads[i], &status);
		if(ret) {
			cout << "Error: unable to join" << ret << endl;
			exit(-1);
		}
	}

    t2 = omp_get_wtime();
	return (t2-t1)*1e3;
}



