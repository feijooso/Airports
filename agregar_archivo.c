#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"

#define max_cantidad_separaciones 2
#define formato_valido "csv"

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
        abb_nodo_t* abb_nodo = abb_guardar(aerolinea->abb, vuelo[ID], vuelo);
        if(abb_nodo == NULL) return false;
        if(!hash_guardar(aerolinea->hash, vuelo[ID], abb_nodo)) {
            abb_borrar(aerolinea->abb, vuelo[ID]);
            fclose(archivo);
            return false;
        }
    }
    free(linea);
    fclose(archivo);
    return true;
}

bool agregar_archivo(aerolinea_t* aerolinea, char* parametros[], size_t pos) {
    if(!es_formato_valido(parametros[0])) return false;
    FILE* archivo = fopen(parametros[0], "r");
    if(archivo == NULL) return false;
    return leer_archivo(aerolinea, archivo);
}