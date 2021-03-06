#include "utils.h"
#include <sys/sysinfo.h>

struct thread_data {
	int id, m, k;
    unsigned int seed;
    float sum, sum2;
    ll num_points;

    thread_data(){}

    thread_data(int id, int m, int k, ll num_points) {
        this->id = id;
        this->m = m;
        this->k = k;
        this->seed = rand();
        this->sum = 0;
        this->sum2 = 0;
        this->num_points = num_points;
	}
};

void *worker(void *thread_arg) {
	thread_data *td;
	td = (thread_data*) thread_arg;
    float x, f_x;
    int aux;

    for (ll i = 0; i < td->num_points; i++) {
        aux = rand_r(&td->seed);
        while(aux == 0) aux = rand_r(&td->seed);
        x = (aux / (2.0 * (float)RAND_MAX));
        f_x = f(td->m, td->k, x);
        td->sum += f_x;
        td->sum2 += f_x * f_x;
    }
	
    pthread_exit(NULL);
}

pff pthreads(ll n, int m, int k, const int num_threads, float* sum, float* sum2, ll *new_n) {
    *new_n = ((n + num_threads-1)/num_threads) * num_threads;
	n = *new_n;
	pthread_t threads[num_threads];
    thread_data td[num_threads];
    srand(time(NULL));
	for(int i = 0; i < num_threads; i++) {
		td[i] = thread_data(i, m, k, (n + num_threads - 1LL) / num_threads);
		int ret = pthread_create(&threads[i], NULL, worker, (void*)&td[i]);
		if(ret) {
			cout << "Error: unable to create pthread " << ret << endl;
			exit(-1);
		}
	}

	void *status;
	for(int i = 0; i < num_threads; i++) {
		int ret = pthread_join(threads[i], &status);
		if(ret) {
			cout << "Error: unable to join" << ret << endl;
			exit(-1);
		}
	}

    (*sum) = 0.; 
    (*sum2) = 0.;
    for (int i = 0; i < num_threads; i++) {
        (*sum) += td[i].sum;
        (*sum2) += td[i].sum2;
    }

    return calc_res(n, *sum, *sum2);
}

pff pthreads_test(ll n, int m, int k, float* sum, float* sum2, ll *new_n) {
    return pthreads(n,m,k, get_nprocs_conf(), sum, sum2, new_n); 
}


void pthreads_tester() {
    int tests = 10;
    ll n = 32000000;
    float eps = 5e-3;
    printf("Testes: %d\nN: %lld\neps: %f\n\n", tests, n, eps);
    double porcentagem_acerto = testa_corretude(tests, n, eps, pthreads_test, true);
    printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}
