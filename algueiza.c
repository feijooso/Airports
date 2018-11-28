#define _XOPEN_SOURCE 700 // or any bigger number
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strutil.h"
#include "utils.c"

#define EOF (-1)

void operar(char* input[]) {
    size_t pos = 0;
    iterar_hasta_palabra_valida(input, &pos);
    remover_enter(input, &pos);
    if(strcmp(input[pos], "agregar_archivo") == 0) {
        printf("agregar_archivo\n");
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
    while((leido = getline(&linea, &variable_inutil, stdin)) != EOF) {
        char** input = split(linea, ' ');
        if(input == NULL) {
            linea = NULL;
            fprintf(stdout, "ERROR\n");
        } else {
            operar(input);
        }
        free_strv(input);
    }
    free(linea);
    return 0;    
}