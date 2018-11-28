#define _XOPEN_SOURCE 700 // or any bigger number
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include "utils.c"
#include "agregar_archivo.c"
#include "constantes.c"
#include "hash.h"

void operar(char* input[], hash_t* hash) {
    size_t pos = 0;
    if(!iterar_hasta_palabra_valida(input, &pos)) {
        printf("error\n");
        return; 
    }
    if(remover_enter(input, &pos)) {
        printf("error\n");
        return;
    }
    if(strcmp(input[pos], "agregar_archivo") == 0) {
        agregar_archivo(hash, input, pos+1);
    } else if(strcmp(input[pos], "ver_tablero") == 0) {
        printf("ver_tablero\n");
    } else if(strcmp(input[pos], "info_vuelo") == 0) {
        printf("info_vuelo\n");
    } else if(strcmp(input[pos], "prioridad_vuelos") == 0) {
        printf("prioridad_vuelos\n");
    } else if(strcmp(input[pos], "borrar") == 0) {
        printf("borrar\n");
    } else {
        printf("error\n");
    }
}

int main() {
    char* linea = NULL;
    size_t leido = 0;
    size_t variable_inutil = 0;
    hash_t* hash = hash_crear(free_strv);
    if(hash == NULL) return 0;
    while((leido = getline(&linea, &variable_inutil, stdin)) != EOF) {
        char** input = split(linea, ' ');
        if(input == NULL) {
            linea = NULL;
            fprintf(stdout, "ERROR\n");
        } else {
            operar(input, hash);
        }
        free_strv(input);
    }
    free(linea);
    hash_destruir(hash);
    return 0;    
}