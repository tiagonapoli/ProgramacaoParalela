#include "utils.h"


pff sequencial(ll n, int m, int k) {
    srand(time(NULL));
    float sum = 0.;
    float sum2 = 0.;
    float x, f_x;
    int aux;
    for (ll i = 0; i < n; i++) {
        aux = rand();
        while(aux == 0) aux = rand();
        x = (aux / (2.0 * (float)RAND_MAX));
        f_x = f(m, k, x);
        sum += f_x;
        sum2 += f_x * f_x;
    }

    return calc_res(n, sum, sum2);
}


void seq_tester() {
    int tests = 10;
    ll n = 8000000;
    float eps = 5e-3;
    printf("Testes: %d\nN: %lld\neps: %f\n\n", tests, n, eps);
    double porcentagem_acerto = testa_corretude(tests, n, eps, sequencial, true);
    printf("Acerto: %lf%%\n", porcentagem_acerto * 100.0);
}

