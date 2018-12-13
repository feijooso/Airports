#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"

#define max_cantidad_separaciones 2
#define formato_valido "csv"

char* generar_clave_fecha_id(char* fecha, char* id) {
    size_t fecha_largo = strlen(fecha);
    size_t id_largo = strlen(id);
    size_t respuesta_largo = fecha_largo + id_largo + 2; //fecha_id\0;
    char* respuesta = malloc(sizeof(char) * respuesta_largo);
    if(respuesta == NULL) return NULL;
    size_t i = 0;
    for(; i<fecha_largo; i++) {
        respuesta[i] = fecha[i];
    }
    respuesta[i] = '_';
    for(size_t n=0; n<id_largo; n++) {
        i++;
        respuesta[i] = id[n];
    }
    respuesta[i+1] = '\0';
    return respuesta;
}

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
        if(vuelo == NULL) return false;
        size_t posicion_ultimo_dato = CANCELADO;
        remover_enter(vuelo, &posicion_ultimo_dato);
        char* abb_clave = generar_clave_fecha_id(vuelo[FECHA], vuelo[ID]);
        if(abb_clave == NULL) return false;

        if (!hash_pertenece(aerolinea->hash,vuelo[ID])){
            if(!abb_guardar(aerolinea->abb, abb_clave, vuelo)) {
                free(abb_clave);
                free_strv(vuelo);
                return false;
            }
            if(!hash_guardar(aerolinea->hash, vuelo[ID], vuelo)) {
                abb_borrar(aerolinea->abb, abb_clave);
                free(abb_clave);
                free_strv(vuelo);
                fclose(archivo);
                return false;
            }
        }
        else {
            char** datos = hash_obtener(aerolinea->hash,vuelo[ID]);
            char* abb_clave_vieja = generar_clave_fecha_id(datos[FECHA], vuelo[ID]);
            abb_borrar(aerolinea->abb, abb_clave_vieja);
            abb_guardar(aerolinea->abb, abb_clave, vuelo);
            free(abb_clave_vieja);
            hash_guardar(aerolinea->hash, vuelo[ID], vuelo);
        }
        free(abb_clave);
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