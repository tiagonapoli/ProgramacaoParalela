-----Flags no makefile-----
No makefile ha a flag CUDA_LIB64_PATH que deve ser setada segundo a localizacao da pasta lib64
do CUDA no seu computador. Na rede linux temos o seguinte:
CUDA_LIB64_PATH=/usr/local/cuda/lib64



-----Uso do make----
make main -> Cria o programa main, pedido pelo enunciado
make tester -> Cria o programa tester, que serve para fazer testes simples de corretude e tempos para execucao.



-----Execucao da main-----
A execucao da main deve ser feita com mpirun, usando dois computadores. Os testes na rede linux foram feitos usando:
mpirun -n 2 main N k m 
(Essa execução não usa de fato 2 computadores, mas simula-os com 2 threads, a fim de testar a implementação MP



-----Execucao da tester-----
A execucao da tester deve ser feita com mpirun, usando dois computadores. Os testes na rede linux foram feitos usando:
mpirun -n 2 tester 

Podem ser passadas as flags:
-s: Testar a implementação sequencial
-p: Testar a implementação com pthreads
-g: Testar a implementação para GPU
-b: Testar a implementação com balanceamento
