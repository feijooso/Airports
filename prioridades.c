#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


int minimos(char** a, char** b){ //recibe una tupla (id y priori)

	if(a[1] == b[1]) return 0; //compara prioridades
	if(a[1] < b[1]) return 1;
	return -1;
}

void prioridad_vuelos(char* input[], aerolinea_t* vuelos){
	heap_t* heap_minimos = heap_crear(minimos);
	hash_iter_t* iter = hash_iter_crear(vuelos->hash);

	for (int i = 0; i < input[0]; ++i) {
		if(!hash_iter_al_final(iter)){
			char* id = hash_iter_ver_actual(iter);
			void* vuelo = hash_obtener(hash, id);
			char* info_vuelo[] = {vuelo[ID], vuelo[PRIORIDAD]}; 
			heap_encolar(heap_minimos, info_vuelo);
			hash_iter_avanzar(iter);
		}
	}

	while(!hash_iter_al_final(iter)){
		char* id = hash_iter_ver_actual(iter);
		void* vuelo = hash_obtener(hash, id);
		char* info_vuelo[] = {vuelo[ID], vuelo[PRIORIDAD]};
		char* tope[] = heap_ver_tope(heap_minimos) 
		if (info_vuelo[1] > tope[1]){
			heap_desencolar(heap_minimos);
			heap_encolar(heap_minimos,info_vuelo);
		}
	}

	for (int i = 0; i < n; ++i)
	{
		char* imprimir = heap_desencolar(heap_minimos);
		printf("%s\n",imprimir[0]);
		printf("%s\n",'-');
		printf("%s\n",imprimir[1]);
	}

}