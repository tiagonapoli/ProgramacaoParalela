#include "utils.h"

//Cronometro - Functions
void cronometro::set_initial_time() {
    t1 = chrono::high_resolution_clock::now();
}

void cronometro::set_final_time() {
    t2 = chrono::high_resolution_clock::now();
}

double cronometro::get_ms_past() {
    time_span = chrono::duration_cast<chrono::duration<double>> (t2 - t1);
    return 1000.0*time_span.count();
}

double cronometro::get_s_past() {
    time_span = chrono::duration_cast<chrono::duration<double>> (t2 - t1);
    return time_span.count();
}

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

void solve(ll n, int m, int k, pff (*func_to_test)(ll, int, int, float*, float*, ll*), const char* print_string, bool verbose) {
    struct cronometro cron;
	float sum, sum2;
	ll dummy;
    double minus_error, plus_error;
    cron.set_initial_time();
    pff ans = (*func_to_test)(n, m, k, &sum, &sum2, &dummy);
    cron.set_final_time();
    minus_error = calcula_erro_resposta(ans.second, m, k);
    plus_error = calcula_erro_resposta(ans.first, m, k);
    if(verbose) {
     	//printf("N calculado: %lld\n", dummy);
		printf("%s %lf\n", print_string,cron.get_s_past());
        printf("Erro no calculo com a soma: %lf\n", plus_error);
        printf("Erro no calculo com a subtracao: %lf\n\n", minus_error);
    }
}

double testa_corretude(int testes, ll iter, float eps, pff (*func_to_test)(ll, int, int, float*, float*, ll*), bool verbose=false) {
    int qtd_bom, qtd_ruim, k, m;
    float minus_error, plus_error;
    pff ans;
    qtd_bom = qtd_ruim = 0;
    srand(time(NULL));
    
    struct cronometro cron;

	ll dummy;
    float sum, sum2;
    for(int i=0;i<testes;i++) {
        k = random_operation_sign() * random_in_range(0,100);
        m = random_operation_sign() * random_in_range(0,100); 
        
        cron.set_initial_time();
        pff ans = (*func_to_test)(iter, m, k, &sum, &sum2, &dummy);
        cron.set_final_time();

        minus_error = calcula_erro_resposta(ans.second, m, k);
        plus_error = calcula_erro_resposta(ans.first, m, k);
        if(minus_error > eps or plus_error > eps) {
            qtd_ruim++;
        } else qtd_bom++;
        if(verbose) printf("==== Teste %d ====\nm: %d\nk: %d\nResultPlus : %9lf [%9lf]\nResultMinus: %9lf [%9lf]\nTempo: %lf ms\n\n",i,m,k,ans.first, plus_error, ans.second, minus_error, cron.get_ms_past());
    }

    if(verbose) printf("==Fim==\nErrados: %d\nCertos: %d\n", qtd_ruim, qtd_bom);
    return ((double)qtd_bom) / (double)(qtd_bom + qtd_ruim);
}

void read(int argc, char **argv, ll &n, int &m, int &k) {
    if(argc < 4) {
        printf("Usage: ./main N k M\n");
        exit(0);
    }
	
    sscanf(argv[1], "%lld", &n);
	sscanf(argv[2], "%d", &k);
    sscanf(argv[3], "%d", &m);
}
