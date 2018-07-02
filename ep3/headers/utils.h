#pragma once
#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<cmath>
#include<utility>
#include<ctime>
#include <chrono>
using namespace std;
typedef pair<float, float> pff;
typedef long long ll;
const float PI = acos(-1);

struct cronometro {
    chrono::high_resolution_clock::time_point t1;
    chrono::high_resolution_clock::time_point t2;
    chrono::duration<double> time_span;

    void set_initial_time();
    void set_final_time();
    double get_ms_past();
    double get_s_past();
};

inline float f(int m, int k, float x) {
    return sin((2 * m + 1) * PI * x) * cos(2 * PI * k * x) / sin(PI * x);
}

inline float calcula_erro_resposta(float result, int m, int k) {
    if(abs(k) <= abs(m)) return (m >= 0) ? fabs(1.0 - result) : fabs(-1.0 - result);
    return fabs(result);
}

double testa_corretude(int testes, ll iter, float eps, pff (*func_to_test)(ll, int, int, float*, float*), bool verbose);

pff calc_res(ll n, float sum, float sum2);

void solve(ll n, int m, int k, pff (*func_to_test)(ll, int, int, float*, float*), const char* print_string, bool verbose);

void read(int argc, char **argv, ll &n, int &m, int &k);

ll choose_optimum_distribution(ll n, int m, int k);
