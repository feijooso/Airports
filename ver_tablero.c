#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "strutil.h"
#include "constantes.c"
#include "cola.h"

bool ver_tablero(char* parametros[], aerolinea_t* vuelos){
	int n = atoi(parametros[0]);
	char* modo = parametros[1];
	char** vector_clavei = calloc(3, sizeof(char*));
	vector_clavei[0] = parametros[2];
	vector_clavei[1] = "0000";
	char* clave_inicial = join(vector_clavei, '_');
	char** vector_clavef = calloc(3,sizeof(char*));
	vector_clavef[0] = parametros[3];
	vector_clavef[1] = "9999";
	char* clave_final = join(vector_clavef, '_');
	abb_iter_t* iter = abb_iter_in_crear(vuelos->abb, clave_inicial);
	char* clave = abb_iter_in_ver_actual(iter);
	if (abb_iter_in_al_final(iter)) {
		abb_iter_in_destruir(iter);
		return true;
	}

	if(strcmp(modo, "asc") == 0){
		cola_t* cola = cola_crear();
		bool seguir = true;
		while (seguir && strcmp(clave_final, clave) > 0){
			cola_encolar(cola,clave);
	        abb_iter_in_avanzar(iter);
			clave = abb_iter_in_ver_actual(iter);
			if (clave == NULL) seguir = false;

		}

		int i = 0;
		while (!cola_esta_vacia(cola) && (i < n)){
			char** clave = split(cola_desencolar(cola), '_');
			printf("%s",clave[0]);
			printf("%s"," - ");
			printf("%s\n",clave[1]);
			free_strv(clave);
			i++;
		}
		cola_destruir(cola,NULL);
	}

	if(strcmp(modo, "desc") == 0){
		pila_t* pila = pila_crear();
		bool seguir = true;
		while (seguir && strcmp(clave_final, clave) > 0){
			pila_apilar(pila,clave);
	        abb_iter_in_avanzar(iter);
			clave = abb_iter_in_ver_actual(iter);
			if (clave == NULL) seguir = false;

		}

		int i = 0;
		while (!pila_esta_vacia(pila) && (i < n)){
			char** clave = split(pila_desapilar(pila), '_');
			printf("%s",clave[0]);
			printf("%s"," - ");
			printf("%s\n",clave[1]);
			free_strv(clave);
			i++;
		}
		pila_destruir(pila);
	}
	
	abb_iter_in_destruir(iter);
	free(vector_clavef);
	free(clave_final);
	free(clave_inicial);
	free(vector_clavei);
	return true;
}
