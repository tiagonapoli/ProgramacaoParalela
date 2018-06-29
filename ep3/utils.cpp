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

double testa_corretude(int testes, ll iter, float eps, pff (*func_to_test)(ll, int, int), bool verbose=false) {
    int qtd_bom, qtd_ruim, k, m;
    float minus_error, plus_error;
    pff ans;
    qtd_bom = qtd_ruim = 0;
    srand(time(NULL));
    
    for(int i=0;i<testes;i++) {
        k = /*random_operation_sign() **/ random_in_range(0,100);
        m = /*random_operation_sign() **/ random_in_range(0,100); 
        pff ans = (*func_to_test)(iter, m, k);
        minus_error = calcula_erro_resposta(ans.second, m, k);
        plus_error = calcula_erro_resposta(ans.first, m, k);
        if(minus_error > eps or plus_error > eps) {
            qtd_ruim++;
        } else qtd_bom++;
        if(verbose) printf("==== Teste %d ====\nm: %d\nk: %d\nResultPlus : %9lf [%9lf]\nResultMinus: %9lf [%9lf]\n\n",i,m,k,ans.first, plus_error, ans.second, minus_error);
    }

    printf("==Fim==\nErrados: %d\nCertos: %d\n", qtd_ruim, qtd_bom);
    return ((double)qtd_bom) / (double)(qtd_bom + qtd_ruim);
}
