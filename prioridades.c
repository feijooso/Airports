#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"
#include "strutil.h"


int minimos(const void* a, const void* b){ //recibe una tupla (id y priori)
	char** valoresa = split(a, ',');
	char** valoresb = split(b, ',');
	if(valoresa[PRIORIDAD] == valoresb[PRIORIDAD]) return 0; //compara prioridades
	if(valoresa[PRIORIDAD] < valoresb[PRIORIDAD]) return 1;
	return -1;
}

bool prioridad_vuelos(char* input[], aerolinea_t* vuelos){
	heap_t* heap_minimos = heap_crear(minimos);
	hash_iter_t* iter = hash_iter_crear(vuelos->hash);
	int n = atoi(input[0]);

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
		if (vuelo[PRIORIDAD] > x){
			heap_desencolar(heap_minimos);
			heap_encolar(heap_minimos, info_joined);
		}
		hash_iter_avanzar(iter);
	}

	for (int i = 0; i < n; i++)
	{
		char* imprimir = heap_desencolar(heap_minimos);
		char** splited = split(imprimir, ',');
		printf("%s ",splited[PRIORIDAD]);
		printf("%s ","-");
        printf("%s\n",splited[ID]);
	}
	return true;

}