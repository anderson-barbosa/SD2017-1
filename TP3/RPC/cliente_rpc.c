//cliente_rpc.c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/client.h>
#include "config.h"

#define VECSIZE 10000
#define MOD 32768
#define NAME "Xmlrpc-c Test Client"
#define VERSION "1.0"

sem_t mutex;

int num_threads;
int result = 0;
double vec[VECSIZE];
int func;
xmlrpc_env env;
const char * const serverUrl = "http://localhost:8080/RPC2";
char * methodName;

static void dieIfFaultOccurred (xmlrpc_env * const envP) {
    if (envP->fault_occurred) {
        fprintf(stderr, "ERROR: %s (%d)\n", envP->fault_string, envP->fault_code);
        exit(1);
    }
}


typedef struct {
	int tid;
	double f_arg;
} thread_arg;

void * thread_func1(void * arg) {
	xmlrpc_value * number;
	xmlrpc_value * paramArrayP;
	xmlrpc_value * resultP;
	xmlrpc_int32 partialResult;
	paramArrayP = xmlrpc_array_new(&env);

	thread_arg * argument = (thread_arg *) arg;
	int tid = argument->tid;
	double num = argument->f_arg;
	int iindex = tid*(VECSIZE/num_threads);
	int findex = (tid < num_threads-1) ? (tid+1)*(VECSIZE/num_threads) : VECSIZE;
	
	number = xmlrpc_int_new(&env, (int)num);
	xmlrpc_array_append_item(&env, paramArrayP, number);
	int j;
    for (j=iindex; j<findex; ++j) {
    	number = xmlrpc_int_new(&env, (int)vec[j]);
    	xmlrpc_array_append_item(&env, paramArrayP, number);
    }
    xmlrpc_DECREF(number);
    resultP = xmlrpc_client_call_params(&env, serverUrl, methodName, paramArrayP);
    xmlrpc_DECREF(paramArrayP);
    xmlrpc_read_int(&env, resultP, &partialResult);
    xmlrpc_DECREF(resultP);
    sem_wait(&mutex);
    result+=partialResult;
    sem_post(&mutex);
}

void * thread_func2(void * arg) {
	xmlrpc_value * number;
	xmlrpc_value * element;
	xmlrpc_value * paramArrayP;
	xmlrpc_value * resultP;
	xmlrpc_double partialMean;
	paramArrayP = xmlrpc_array_new(&env);
	
	thread_arg * argument = (thread_arg *) arg;
	int tid = argument->tid;
	double num = argument->f_arg;
	int iindex = tid*(VECSIZE/num_threads);
	int findex = (tid < num_threads-1) ? (tid+1)*(VECSIZE/num_threads) : VECSIZE;

	if (func==4) {
		number = xmlrpc_double_new(&env, num);
	} else {
		number = xmlrpc_int_new(&env, (int)num);
	}

	xmlrpc_array_append_item(&env, paramArrayP, number);
	int j;
    for (j=iindex; j<findex; ++j) {
    	number = xmlrpc_int_new(&env, (int)vec[j]);
    	xmlrpc_array_append_item(&env, paramArrayP, number);
    }
    xmlrpc_DECREF(number);
    resultP = xmlrpc_client_call_params(&env, serverUrl, methodName, paramArrayP);
    xmlrpc_DECREF(paramArrayP);
    printf("%d %f\n", tid, vec[iindex]);
    for (j = iindex; j < findex; ++j) {
    	xmlrpc_array_read_item(&env, resultP, j-iindex, &element);
		xmlrpc_read_double(&env, element, &vec[j]);
    }
    printf("%d %f\n", tid, vec[iindex]);
}

int main(int argc, char **argv) {
	sscanf(argv[1], "%d", &num_threads);
	sscanf(argv[2], "%d", &func);
	double arg = strtod(argv[3], NULL);

	switch(func) {
		case 1:
			methodName = "rpc.count.greater";
			break;
		case 2:
			methodName = "rpc.log";
			break;
		case 3:
			methodName = "rpc.nth.root";
			break;
		case 4:
			methodName = "rpc.mult";
			break;
		case 5:
			methodName = "rpc.count.multiples";
			break;
		case 6:
			methodName = "rpc.count.powers";
			break;
	}

    pthread_t threads[num_threads];
    thread_arg args[num_threads];
    int indexes[num_threads];
    sem_init(&mutex, 0, 1);

    xmlrpc_env_init(&env);

    srand(time(NULL));
    int i;
    for (i=0; i<VECSIZE; ++i) {
    	vec[i] = rand() % MOD;
    }

    xmlrpc_client_init2(&env, XMLRPC_CLIENT_NO_FLAGS, NAME, VERSION, NULL, 0);
    dieIfFaultOccurred(&env);

    switch(func) {
    	case 1:
		case 5:
		case 6:
    	{
    		for (i = 0; i < num_threads; ++i) {
		    	args[i].tid = i;
		    	args[i].f_arg = arg;
		    	pthread_create(&threads[i], NULL, thread_func1, &args[i]);
		    }
		    break;
		}
		case 2:
		case 3:
		case 4:
		{
			for (i = 0; i < num_threads; ++i) {
		    	args[i].tid = i;
		    	args[i].f_arg = arg;
		    	pthread_create(&threads[i], NULL, thread_func2, &args[i]);
    		}
    		break;
		}
    }

    for (i = 0; i < num_threads; ++i) {
    	pthread_join(threads[i], NULL);
    }

	if (func==1 || func>4) {
		printf("O resultado e %d.\n", result);
	}


    xmlrpc_env_clean(&env);

    xmlrpc_client_cleanup();

	return 0;
}
