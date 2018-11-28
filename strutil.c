#include "strutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
 * de ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
 * cadenas que contiene son allocadas dinámicamente.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica del
 * arreglo devuelto. La función devuelve NULL si falló alguna llamada a
 * malloc(), o si ‘sep’ es '\0'.
 */
char** split(const char* str, char sep) {

	const size_t str_length = strlen(str);

	int elements_split_count = 2;
	for (int i=0; i < str_length; i++) {
		if(str[i] == sep) elements_split_count++;
	}

	char** element_splitted = malloc(sizeof(char*) * elements_split_count); 
	if(element_splitted == NULL) return NULL;

	int posicion_sep = 0;
	int split_posicion = 0;
	for(int i=0; i<=str_length; i++) {
		if(str[i] == sep || str[i] == '\0') {
			int size_split = i-posicion_sep;
			char* split = malloc(sizeof(char) * (size_split+1));	
			if(split == NULL) {
				for (int n=0; n<i; n++) {
					free(element_splitted[n]);
				}
				free(element_splitted);
				return NULL;	
			}

			strncpy(split, str+posicion_sep, size_split);
			split[size_split] = '\0';

			element_splitted[split_posicion] = split;
			posicion_sep = i+1;
			split_posicion++;
		}
	}

	element_splitted[elements_split_count-1] = NULL;

	return element_splitted;	
}

/*
 * Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL ‘strv’.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria.
 */
char* join(char** strv, char sep) {
	if(strv[0] == NULL) {
		char* vacio = malloc(sizeof(char));
		if(vacio) vacio[0] = '\0';
		return vacio;
	}

	size_t tamanio = 0;
	size_t strv_tam = 0;

	while (strv[strv_tam]) {
		tamanio += strlen(strv[strv_tam]);
		strv_tam++;
	}

	tamanio += strv_tam; 
	char* string = malloc(sizeof(char)*tamanio);
	if(string == NULL) return NULL;

	size_t pos = 0;
	for(size_t i=0; i<strv_tam; i++) {
		size_t str_tam = strlen(strv[i]);
		for(size_t n=0; n<str_tam; n++) {
			string[pos] = strv[i][n];
			pos++;
		}
		if(i < strv_tam-1) {
			string[pos] = sep;
			pos++;
		}
	}

	string[tamanio-1] = '\0';
	return string;
}

/*
 * Libera un arreglo dinámico de cadenas, y todas las cadenas que contiene.
 */
void free_strv(char* strv[]) {
	int posicion = 0;
	while(strv[posicion]) {
		free(strv[posicion]);
		posicion++;
	}
	free(strv);
}