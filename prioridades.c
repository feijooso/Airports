#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "strutil.h"
#include "constantes.c"
#include "hash.h"


int minimos(const void* a, const void* b){ 
	char** valoresa = split(a, ',');
	char** valoresb = split(b, ',');
    int pa = atoi(valoresa[PRIORIDAD]);
    int pb = atoi(valoresb[PRIORIDAD]);
	int res;
	if(pa == pb) {
		/*int ia = atoi(valoresa[ID]);
		int ib = atoi(valoresb[ID]);*/
		if(strcmp(valoresa[ID], valoresb[ID]) < 0) {
			res = -1;
		} else {
			res = 1;
		}
	} else if(pa < pb) {
		res = 1;
	} else {
		res = -1;
	}
	free_strv(valoresb);
    free_strv(valoresa);
	return res;
}

bool isNumeric(const char *str) {
    while(*str != '\0') {
        if(*str < '0' || *str > '9')
            return false;
        str++;
    }
    return true;
}

bool prioridad_vuelos(char* input[], aerolinea_t* vuelos) {
	if(!isNumeric(input[0])) return false;
	int n = atoi(input[0]);
	heap_t* heap_minimos = heap_crear(minimos);
	if(heap_minimos == NULL) return false;
	hash_iter_t* iter = hash_iter_crear(vuelos->hash);
	if(iter == NULL) {
		heap_destruir(heap_minimos, free);
		return false;
	}
	for (int i = 0; i < n; ++i) {
		if(!hash_iter_al_final(iter)){
			char** vuelo = hash_obtener(vuelos->hash, hash_iter_ver_actual(iter));
			char* info_joined = join(vuelo, ',');
			heap_encolar(heap_minimos, info_joined);
			hash_iter_avanzar(iter);
		}
	}

	while(!hash_iter_al_final(iter)){
		char** vuelo = hash_obtener(vuelos->hash, hash_iter_ver_actual(iter));
		char* info_joined = join(vuelo, ',');
		char* tope = heap_ver_max(heap_minimos); 
		char** tope_splited = split(tope, ',');
		int x = atoi(tope_splited[PRIORIDAD]);
		int y = atoi(vuelo[PRIORIDAD]);
		if (y > x){
			free(heap_desencolar(heap_minimos));
			heap_encolar(heap_minimos, info_joined);
		}
		else free(info_joined);
		free_strv(tope_splited);
		hash_iter_avanzar(iter);
	}

	int tam;
	if (n > heap_cantidad(heap_minimos)) tam = (int)heap_cantidad(heap_minimos);
	else tam = n;

    char* vector[tam];

    for (int j = 0; j < tam; j++) {
        vector[j] = heap_desencolar(heap_minimos);
    }

    for (int k = tam-1; k >= 0; k--) {

        char** splited = split(vector[k], ',');
        printf("%s ",splited[PRIORIDAD]);
        printf("%s ","-");
        printf("%s\n",splited[ID]);
        free_strv(splited);
        free(vector[k]);
    }

    hash_iter_destruir(iter);
    heap_destruir(heap_minimos, free);
	return true;

}