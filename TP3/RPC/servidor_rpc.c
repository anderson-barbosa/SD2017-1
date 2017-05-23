//servidor_rpc.c
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include "config.h"

static xmlrpc_value * rpc_count_greater (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_int32 n;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_int32 ret;
	int result = 0;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_int(envP, element, &n);
	int i;
	for (i = 1; i <= vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}
	xmlrpc_DECREF(element);

	for (i = 0; i < vec_size; ++i){
		if (vec[i]>n) {
			result+=1;
		}
	}
	ret = (xmlrpc_int32) result;
	return xmlrpc_build_value(envP, "i", ret);
}

static xmlrpc_value * rpc_log (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_int32 base;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_value * ret;
	double l;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_int(envP, element, &base);
	int i = 0;
	for (i = 1; i <=vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}

	ret = xmlrpc_array_new(envP);
	i=0;
	for (i = 0; i < vec_size; ++i){
		l = (xmlrpc_double)(log(vec[i])/log(base));
		element = xmlrpc_double_new(envP, l);
		xmlrpc_array_append_item(envP, ret, element);

	}
	xmlrpc_DECREF(element);
	
	return ret;
}

static xmlrpc_value * rpc_nth_root (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_int32 n;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_value * ret;
	double l;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_int(envP, element, &n);
	int i = 0;
	for (i = 1; i <=vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}

	ret = xmlrpc_array_new(envP);
	double exp = 1.0/n;
	i=0;
	for (i = 0; i < vec_size; ++i){
		l = (xmlrpc_double)(pow(vec[i],exp));
		element = xmlrpc_double_new(envP, l);
		xmlrpc_array_append_item(envP, ret, element);

	}
	xmlrpc_DECREF(element);
	
	return ret;
}

static xmlrpc_value * rpc_count_multiples (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_int32 n;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_int32 ret;
	int result = 0;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_int(envP, element, &n);
	int i;
	for (i = 1; i <= vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}
	xmlrpc_DECREF(element);

	for (i = 0; i < vec_size; ++i){
		if (vec[i]%n==0) {
			result+=1;
		}
	}
	ret = (xmlrpc_int32) result;
	return xmlrpc_build_value(envP, "i", ret);
}

static xmlrpc_value * rpc_count_powers (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_int32 n;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_int32 ret;
	int result = 0;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_int(envP, element, &n);
	int i;
	for (i = 1; i <= vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}
	xmlrpc_DECREF(element);
	
	int let;
	double test;
	int intTest;
	
	for (i = 0; i < vec_size; ++i){
		let = (int)vec[i];
		test = log(let)/log(n);
		intTest = (int)test;
		if (test-intTest==0) {
			result+=1;
		}
	}
	ret = (xmlrpc_int32) result;
	return xmlrpc_build_value(envP, "i", ret);
}

static xmlrpc_value * rpc_mult (xmlrpc_env * const envP, xmlrpc_value * const paramArrayP, void * const serverInfo, void * const channelInfo) {
	xmlrpc_double n;
	int vec_size = xmlrpc_array_size(envP, paramArrayP)-1;
	xmlrpc_int32 vec[vec_size];
	xmlrpc_value * element;
	xmlrpc_value * ret;
	double l;

	xmlrpc_array_read_item(envP, paramArrayP, 0, &element);
	xmlrpc_read_double(envP, element, &n);
	int i = 0;
	for (i = 1; i <=vec_size; ++i) {
		xmlrpc_array_read_item(envP, paramArrayP, i, &element);
		xmlrpc_read_int(envP, element, &vec[i-1]);
	}

	ret = xmlrpc_array_new(envP);
	i=0;
	for (i = 0; i < vec_size; ++i){
		l = (double)(vec[i]*n);
		element = xmlrpc_double_new(envP, l);
		xmlrpc_array_append_item(envP, ret, element);

	}
	xmlrpc_DECREF(element);
	
	return ret;
}

int main(int argc, char **argv) {
    struct xmlrpc_method_info3 const methodInfo1 = {
        /* .methodName     = */ "rpc.count.greater",
        /* .methodFunction = */ &rpc_count_greater,
    };

    struct xmlrpc_method_info3 const methodInfo2 = {
       /* .methodName     = */ "rpc.log",
       /* .methodFunction = */ &rpc_log,
   	};
   	struct xmlrpc_method_info3 const methodInfo3 = {
       /* .methodName     = */ "rpc.nth.root",
       /* .methodFunction = */ &rpc_nth_root,
   	};
   	struct xmlrpc_method_info3 const methodInfo4 = {
       /* .methodName     = */ "rpc.mult",
       /* .methodFunction = */ &rpc_mult,
   	};
	struct xmlrpc_method_info3 const methodInfo5 = {
       /* .methodName     = */ "rpc.count.multiples",
       /* .methodFunction = */ &rpc_count_multiples,
   	};
	struct xmlrpc_method_info3 const methodInfo6 = {
       /* .methodName     = */ "rpc.count.powers",
       /* .methodFunction = */ &rpc_count_powers,
   	};
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    if (argc-1 != 1) {
        fprintf(stderr, "You must specify 1 argument:  The TCP port "
                "number on which the server will accept connections "
                "for RPCs (8080 is a common choice).  "
                "You specified %d arguments.\n",  argc-1);
        exit(1);
    }
    
    xmlrpc_env_init(&env);

    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred) {
        printf("xmlrpc_registry_new() failed.  %s\n", env.fault_string);
        exit(1);
    }

    xmlrpc_registry_add_method3(&env, registryP, &methodInfo1);
   	xmlrpc_registry_add_method3(&env, registryP, &methodInfo2);
   	xmlrpc_registry_add_method3(&env, registryP, &methodInfo3);
   	xmlrpc_registry_add_method3(&env, registryP, &methodInfo4);
	xmlrpc_registry_add_method3(&env, registryP, &methodInfo5);
   	xmlrpc_registry_add_method3(&env, registryP, &methodInfo6);


    if (env.fault_occurred) {
        printf("xmlrpc_registry_add_method3() failed.  %s\n",
               env.fault_string);
        exit(1);
    }

    serverparm.config_file_name = NULL;
    serverparm.registryP        = registryP;
    serverparm.port_number      = atoi(argv[1]);
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

    printf("Running XML-RPC server...\n");

    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if (env.fault_occurred) {
        printf("xmlrpc_server_abyss() failed.  %s\n", env.fault_string);
        exit(1);
    }

    return 0;
}
