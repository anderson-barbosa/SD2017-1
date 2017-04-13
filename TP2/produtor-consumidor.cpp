#include <iostream>
#include <thread>
#include <semaphore.h>
#include <ctime>
#include <cstdlib>
#define N 2
#define NMAX 10000
using namespace std;

int buffer[N];
int np;
int nc;

sem_t mutex;
sem_t empty;
sem_t full;

bool imprimir = true;
int num_proc = 0;

void produtor(int tid) {
	while(1) {
		srand(clock());
		int n = (rand()%(10000000))+1;
		sem_wait(&empty);
		sem_wait(&mutex);
		for (int i=0; i<N; i++) {
			if (buffer[i]==0) {
				buffer[i]=n;
				break;
			}
		}
		if (num_proc>=NMAX) {
			sem_post(&mutex);
			sem_post(&full);
			sem_post(&empty);
			break;
		} else {
			sem_post(&mutex);
			sem_post(&full);
		}
	}
}

void consumidor(int tid) {
	while(1) {
		int n;
		bool eprimo = true;
		sem_wait(&full);
		sem_wait(&mutex);
		for (int j=0; j<N; j++) {
			if (buffer[j]>0) {
				n=buffer[j];
				buffer[j]=0;
				break;
			}
		}
		num_proc+=1;
		if (num_proc>NMAX) {
			sem_post(&mutex);
			sem_post(&empty);
			sem_post(&full);
			break;
		} else {
			sem_post(&mutex);
			sem_post(&empty);
		}
		for (int j=2; j<n; j++) {
			if (n%j==0) {
				eprimo=false;
				break;
			}
		}
		if (imprimir) {
			cout << n;
			if (eprimo){
				cout << " é primo.";
			} else {
				cout << " não é primo.";
			}
			cout << endl;
		}
	}
}

int main(int argc, char *argv[]) {
	sscanf(argv[1], "%d", &np);
	sscanf(argv[2], "%d", &nc);
	int aux;
	if (argc==4) {
		sscanf(argv[3], "%d", &aux);
	}
	imprimir=aux;
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, N);
	sem_init(&full, 0, 0);
	thread p[np];
	thread c[nc];
	for (int i=0; i<N; i++) {
		buffer[i]=0;
	}
	int tinicial = time(NULL);
	
	for (int i = 0; i < np; ++i) {
		p[i] = thread(produtor, i);
	}
	for (int i = 0; i < nc; ++i) {
		c[i] = thread(consumidor, i);
	}	

	for (int i = 0; i < np; ++i) {
	    p[i].join();
	}
	for (int i = 0; i < nc; ++i) {
	    c[i].join();
	}
	int tfinal = time(NULL);
	cout << "Tempo: " << (tfinal-tinicial) << " segundos" << endl;
	return 0;
}
