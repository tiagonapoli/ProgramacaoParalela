#include <stdio.h>
#include <math.h>

void add(int n, float* x, float* y) {
    for(int i=0;i<n;i++) {
        y[i] = x[i] + y[i];
    }
}

int main() {
    int n = 1 << 20;

    float *x = new float[n];
    float *y = new float[n];

    for(int i=0;i<n;i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    add(n,x,y);

    float error = 0.0f;
    for(int i=0;i<n;i++) {
        error = fmax(error, fabs(y[i]-3.0f));
    }
    printf("Max error: %f\n", error);

    delete [] x;
    delete [] y;   

    return 0;
}