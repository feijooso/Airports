#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"

#define max_cantidad_separaciones 2
#define formato_valido "csv"
#define cantidad_parametros 1

bool es_formato_valido(char* nombre_archivo) {
    char** archivo_valido = split(nombre_archivo, '.');
    if(archivo_valido == NULL) return false;
    size_t cant_separaciones = 0;
    while(archivo_valido[cant_separaciones] != NULL) {
        cant_separaciones++;
    }
    if(cant_separaciones != max_cantidad_separaciones || strcmp(archivo_valido[1], formato_valido) != 0) {
        free_strv(archivo_valido);
        return false;
    }
    free_strv(archivo_valido);
    return true;
}

bool leer_archivo(aerolinea_t* aerolinea, FILE* archivo) {
    char* linea = NULL;
    size_t variable_inutil = 0;
    size_t leido = 0;
    while((leido = getline(&linea, &variable_inutil, archivo)) != EOF) {
        char** vuelo = split(linea, ',');
        size_t posicion_ultimo_dato = CANCELADO;
        remover_enter(vuelo, &posicion_ultimo_dato);
        if(!hash_guardar(aerolinea->hash, vuelo[ID], vuelo)) {
            free_strv(vuelo);
            fclose(archivo);
            return false;
        }
    }
    free(linea);
    fclose(archivo);
    printf("OK\n");
    return true;
}

bool agregar_archivo(aerolinea_t* aerolinea, char* input[], size_t pos) {
    char* parametros[cantidad_parametros];
    if(!cantidad_parametros_correctos(input, pos, cantidad_parametros, parametros)) {
        fprintf(stderr, "error\n");
        return false;
    }
    if(!es_formato_valido(parametros[0])) {
        fprintf(stderr, "archivo invalido\n");
        return false;
    }
    FILE* archivo = fopen(parametros[0], "r");
    if(archivo == NULL) {
        fprintf(stderr, "No se pudo leer el archivo indicado\n");
		return false;
    }

    return leer_archivo(aerolinea, archivo);
}