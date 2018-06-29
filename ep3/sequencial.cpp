#include "utils.h"


pff sequencial(ll n, int m, int k) {
    srand(time(NULL));
    float sum = 0.;
    float sum2 = 0.;
    float x, f_x;
    for (ll i = 0; i < n; i++) {
        x = (rand() / (2.0 * (float)RAND_MAX));
        f_x = f(m, k, x);
        sum += f_x;
        sum2 += f_x * f_x;
    }

    return calc_res(n, sum, sum2);
}


int main() {


}