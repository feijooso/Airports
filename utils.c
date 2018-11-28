#include <string.h>
#include <stdio.h>

void iterar_hasta_palabra_valida(char* input[], size_t* pos) {
    while(*input[*pos] == '\0' || *input[*pos] == '\n') {
        *pos += 1;
    }
}

void remover_enter(char* input[], size_t* pos) {
    size_t longitud = strlen(input[*pos]);
    if(longitud > 0 && input[*pos][longitud-1] == '\n') {
        input[*pos][longitud-1] = '\0';
    }
}