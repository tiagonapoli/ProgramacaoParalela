#include <cstdio>
#include <cstdlib>
#include <utility>
#include <time.h>

int print_flag = 0;
#define fi first
#define se second
#define ll long long
#define pll pair<ll, ll>
using namespace std;

void generate(const char *path, pll dim) {
	FILE* f = fopen(path, "w");
	fprintf(f, "%lld %lld\n", dim.fi, dim.se);
    ll qtd = (dim.fi * dim.se)*8/10;
    ll x,y;
    double val;
    if(print_flag) printf("%lld x %lld: qtd %lld\n", dim.fi, dim.se, qtd);
    while(qtd--) {
        x = 1 + rand() % dim.fi;
        y = 1 + rand() % dim.se;
        val = rand() % 100/(1.0 + rand() % 100);
        fprintf(f, "%lld %lld %lf\n", x,y,val);
    }
	fclose(f);
}

int main (int argc, char* argv[]) {
	pll dimA, dimB;

    srand(time(NULL));

    int sz = 500 + rand() % 1000;
    if(argc >= 3) {
        sscanf(argv[1], "%d", &sz);
        sscanf(argv[2], "%d", &print_flag);
    }

    dimA.fi = sz;
    dimA.se = sz;
    dimB.fi = dimA.se;
    dimB.se = sz;

    if(sz == -1) {
        dimA.fi = 1 + rand() % 800;
        dimA.se = 1 + rand() % 800;
        dimB.fi = dimA.se;
        dimB.se = 1 + rand() % 800;
    }

    generate("./input/a.in", dimA);
    generate("./input/b.in", dimB);

    if(print_flag) printf("GERADOS OS INPUTS\n");

	return 0;

}
