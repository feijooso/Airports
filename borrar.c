#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"

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
    pila_t* pila = pila_crear();
	while(!abb_iter_in_al_final(iter) && strcmp(clave_final,abb_iter_in_ver_actual(iter)) > 0){
		pila_apilar(pila,abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	while(!pila_esta_vacia(pila)){
	    char* fecha_id = pila_desapilar(pila);
        char** datos = split(fecha_id,'_');
        hash_borrar(vuelos->hash, datos[1]);
	    abb_borrar(vuelos->abb, fecha_id);

	    free_strv(datos);
	}
	free(vector_clavef);
	free(vector_clavei);
	free(clave_inicial);
	free(clave_final);
	pila_destruir(pila);
	abb_iter_in_destruir(iter);
	return true;
}