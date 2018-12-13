#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"
#include "cola.h"

bool borrar(aerolinea_t* vuelos, char* parametros[]){
	if (strcmp(parametros[0], parametros[1]) > 0) return false;
	char** vector_clavei = calloc(3, sizeof(char*));
	vector_clavei[0] = parametros[0];
	vector_clavei[1] = "0000";
	char* clave_inicial = join(vector_clavei, '_');
	char** vector_clavef = calloc(3,sizeof(char*));
	vector_clavef[0] = parametros[1];
	vector_clavef[1] = "9999";
	char* clave_final = join(vector_clavef, '_');
	abb_iter_t* iter = abb_iter_in_crear(vuelos->abb, clave_inicial);
	if(iter == NULL) return false;
	if(abb_iter_in_al_final(iter)) return true;
    cola_t* cola = cola_crear();
	while(!abb_iter_in_al_final(iter) && strcmp(clave_final,abb_iter_in_ver_actual(iter)) > 0){
		cola_encolar(cola,abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	while(!cola_esta_vacia(cola)){
	    char* fecha_id = cola_desencolar(cola);
        char** datos = split(fecha_id,'_');
        char** vuelo = (char**)hash_obtener(vuelos->hash, datos[1]);
        if (vuelo != NULL){
			for (int i = 0; i < 9; i++) {
				printf("%s ", vuelo[i]);
			}
			printf("%s", vuelo[9]);
			printf("\n");
			hash_borrar(vuelos->hash, datos[1]);
		}       
        
	    abb_borrar(vuelos->abb, fecha_id);

	    free_strv(datos);
	}
	free(vector_clavef);
	free(vector_clavei);
	free(clave_inicial);
	free(clave_final);
	cola_destruir(cola, NULL);
	abb_iter_in_destruir(iter);
	return true;
}