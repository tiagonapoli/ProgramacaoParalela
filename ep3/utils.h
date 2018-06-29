#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<utility>
#include<ctime>
using namespace std;
typedef pair<float, float> pff;
typedef long long ll;
const float PI = acos(-1);

inline float f(int m, int k, float x) {
    return sin((2 * m + 1) * PI * x) * cos(2 * PI * k * x) / sin(PI * x);
}

inline float calcula_erro_resposta(float result, int m, int k) {
    if(abs(k) <= abs(m)) return fabs(1.0 - result);
    return fabs(result);
}

void testa_corretude(int testes, bool verbose, void (*func_to_test)(ll, int, int));

pff calc_res(ll n, float sum, float sum2);