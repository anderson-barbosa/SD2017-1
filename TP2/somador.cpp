#include <iostream>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <atomic>
#define VETSIZE 10000

using namespace std;

int num_threads;
int soma = 0;
signed char nums[VETSIZE];

atomic_flag lock = ATOMIC_FLAG_INIT;

void acquire() {
	while (lock.test_and_set());
}

void release() {
	lock.clear();
}

void somador(int tid) {
	int slocal = 0;
	int inicio = tid*(VETSIZE/num_threads);
	int fim = min((tid+1)*(VETSIZE/num_threads), VETSIZE);
	for (int i=inicio; i<fim; i++) {
		slocal+=nums[i];
	}
	acquire();
	soma+=slocal;
	release();
}

int main(int argc, char *argv[]) {
	sscanf(argv[1], "%d", &num_threads);
	thread t[num_threads];
	for (int i=0; i<VETSIZE; i++) {
		srand(clock());
		signed char n = (rand()%201) - 100;
		nums[i]=n;
	}
	int tinicial = clock();
	for (int i = 0; i < num_threads; ++i) {
	    t[i] = thread(somador, i);
	}

	for (int i = 0; i < num_threads; ++i) {
	   t[i].join();
	}
	int tfinal = clock();
	cout << "Resultado da soma: "<< soma << endl;
	cout << "Tempo para " << num_threads << " threads: " << (float)(tfinal-tinicial)/CLOCKS_PER_SEC << " segundos" << endl;
	return 0;
}