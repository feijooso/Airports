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

	while(!abb_iter_in_al_final(iter) && strcmp(clave_final,abb_iter_in_ver_actual(iter)) > 0){
		char** datos = split(abb_iter_in_ver_actual(iter),'_');
		hash_borrar(vuelos->hash, datos[1]);
		abb_iter_borrar(iter);
	}
	abb_iter_in_destruir(iter);
	return true;
}