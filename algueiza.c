#define _XOPEN_SOURCE 700 // or any bigger number
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include "algueiza.h"
#include "agregar_archivo.c"
#include "info_vuelos.c"
#include "prioridades.c"
#include "borrar.c"
#include "ver_tablero.c"

aerolinea_t* aerolinea_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato_abb, hash_destruir_dato_t destruir_dato_hash) {
    aerolinea_t* aerolinea = malloc(sizeof(aerolinea_t));
    if(aerolinea == NULL) return NULL;
    aerolinea->hash = hash_crear(destruir_dato_hash);
    if(aerolinea->hash == NULL) {
        free(aerolinea);
        return NULL;
    }
    aerolinea->abb = abb_crear(cmp, destruir_dato_abb);
    if(aerolinea->abb == NULL) {
        hash_destruir(aerolinea->hash);
        free(aerolinea);
        return NULL;
    }
    return aerolinea;
}

void aerolinea_destruir(aerolinea_t* aerolinea) {
    hash_destruir(aerolinea->hash);
    abb_destruir(aerolinea->abb);
    free(aerolinea);
}

void operar(char* input[], aerolinea_t* aerolinea) {
    size_t pos = 0;
    if(!iterar_hasta_palabra_valida(input, &pos)) {
        printf("ERROR\n");
        return; 
    }
    if(remover_enter(input, &pos)) {
        printf("ERROR\n");
        return;
    }
    if(strcmp(input[pos], "agregar_archivo") == 0) {
        pos++;
        char* parametros[CANTIDAD_PARAMETROS_AGREGAR_ARCHIVO];
        if(cantidad_parametros_correctos(input, pos, CANTIDAD_PARAMETROS_AGREGAR_ARCHIVO, parametros) 
         && agregar_archivo(aerolinea, parametros, pos+1)) {
            printf("OK\n");         
        } else {
            printf("ERROR\n");
        }

    } else if(strcmp(input[pos], "ver_tablero") == 0) {
        pos++;
        char* parametros[CANTIDAD_PARAMETROS_VER_TABLERO];
        if(cantidad_parametros_correctos(input, pos, CANTIDAD_PARAMETROS_VER_TABLERO, parametros)
           && ver_tablero(parametros,aerolinea)) {
            printf("OK\n");
        } else {
            printf("ERROR\n");
        }

    } else if(strcmp(input[pos], "info_vuelo") == 0) {
        pos++;
        char* parametros[CANTIDAD_PARAMETROS_INFO_VUELO];
        if(cantidad_parametros_correctos(input, pos, CANTIDAD_PARAMETROS_INFO_VUELO, parametros)
           && info_vuelo(parametros,aerolinea)) {
            printf("OK\n");
        } else {
            printf("ERROR\n");
        }

    } else if(strcmp(input[pos], "prioridad_vuelos") == 0) {
        pos++;
        char* parametros[CANTIDAD_PARAMETROS_PRIORIDADES];
        if(cantidad_parametros_correctos(input, pos, CANTIDAD_PARAMETROS_PRIORIDADES, parametros)
           && prioridad_vuelos(parametros,aerolinea)) {
            printf("OK\n");
        } else {
            printf("ERROR\n");
        }
    } else if(strcmp(input[pos], "borrar") == 0) {
        pos++;
        char* parametros[CANTIDAD_PARAMETROS_BORRAR];
        if(cantidad_parametros_correctos(input, pos, CANTIDAD_PARAMETROS_BORRAR, parametros)
         && borrar(aerolinea, parametros)) {
            printf("OK\n");
        } else {
            printf("ERROR\n");
        }

    } else {
        printf("ERROR\n");
    }
}

int main() {
    char* linea = NULL;
    size_t leido = 0;
    size_t variable_inutil = 0;
    aerolinea_t* aerolinea = aerolinea_crear(strcmp, NULL, free_strv);
    if(aerolinea == NULL) return 0;
    while((leido = getline(&linea, &variable_inutil, stdin)) != EOF) {
        char** input = split(linea, ' ');
        if(input == NULL) {
            linea = NULL;
            fprintf(stdout, "ERROR\n");
        } else {
            operar(input, aerolinea);
        }
        free_strv(input);
    }
    free(linea);
    aerolinea_destruir(aerolinea);
    return 0;    
}