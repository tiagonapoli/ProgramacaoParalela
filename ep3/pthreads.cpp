#include "utils.h"

struct thread_data {
	int id, seed;
    float sum, sum2;
    ll num_points;

    thread_data(){}

    thread_data(int id, ll num_points, float sum = 0., float sum2 = 0.) {
        this->id = id;
        this->seed = time(NULL);
        this->num_points = num_points;
        this->sum = sum;
        this->sum2 = sum2;
	}
};

void *worker(void *thread_arg) {
	thread_data *td;
	td = (thread_data*) thread_arg;
    float x, f_x;
    for (ll i = 0; i < td->num_points; i++) {
        x = (rand_r(&td->seed) / (2.0 * (float)RAND_MAX));
        f_x = f(m, k, x);
        td->sum += f_x;
        td->sum2 += f_x * f_x;
    }
	pthread_exit(NULL);
}

pff pthreads(int n, int m, int k, const int num_threads) {
    pthread_t threads[num_threads];
	for(int i = 0; i < num_threads; i++) {
		td[i] = thread_data(i, (n + num_threads - 1) / num_threads);
		ret = pthread_create(&threads[i], NULL, worker, (void*)&td[i]);
		if(ret) {
			cout << "Error: unable to create pthread " << ret << endl;
			exit(-1);
		}
	}

	void *status;
	for(int i = 0; i < num_threads; i++) {
		ret = pthread_join(threads[i], &status);
		if(ret) {
			cout << "Error: unable to join" << ret << endl;
			exit(-1);
		}
	}

    float sum = 0., sum2 = 0.;
    for (int i = 0; i < num_threads; i++) {
        sum += td[i].sum;
        sum2 += td[i].sum2;
    }

    return calc_res(sum, sum2);
}