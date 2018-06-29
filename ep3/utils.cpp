#include "utils.h"

pff calc_res(ll n, float sum, float sum2) {
    float sqrt_delta = sqrt(sum2 - (sum * sum) / (float)n);
    float res_plus = (sum + sqrt_delta) / (float)n;
    float res_minus = (sum - sqrt_delta) / (float)n;
    return pff(res_plus, res_minus);
}

inline int random_operation_sign() {
    if(rand() % 2) return 1;
    return -1;
}

inline int random_in_range(int a, int b) {
    //random in interval [a,b]
    if(a == b) return a;
    return a + rand() % (b-a+1);
}
/*
void testa_corretude(int testes, bool verbose, void (*func_to_test)(ll, int, int)) {
    int qtd_bom, qtd_ruim, k, m;
    float minus_error, plus_error;
    pff ans;
    qtd_bom = qtd_ruim = 0;
    srand(time(NULL));
    
    for(int i=0;i<testes;i++) {
        k = random_operation_sign() * random_in_range(0,100);
        m = random_operation_sign() * random_in_range(0,100); 
        pff ans = (*func_to_test)(1000000, m, k);
        minus_error = calcula_erro_resposta(ans.first, m, k);
        plus_error = calcula_erro_resposta(ans.second, m, k);
        if(verbose) printf("==== Teste %d ====\nPlus: %lf\nMinus %lf\n\n",i, plus_error, minus_error);
    }
}
*/