#include "utils.h"

inline float f(int m, int k, float x) {
    return sin((2 * m + 1) * PI * x) * cos(2 * PI * k * x) / sin(PI * x);
}

pff calc_res(float sum, float sum2) {
    float sqrt_delta = sqrt(sum2 - (sum * sum) / (float)n);
    float res_plus = (sum + sqrt_delta) / (float)n;
    float res_minus = (sum - sqrt_delta) / (float)n;
    return pff(res_plus, res_minus);
}