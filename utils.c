#include <string.h>
#include <stdio.h>
#include <stdbool.h>

bool iterar_hasta_palabra_valida(char* input[], size_t* pos) {
    if(input[*pos] == NULL) return false;
    while(*input[*pos] == '\0' || *input[*pos] == '\n') {
        *pos += 1;
        if(input[*pos] == NULL) return false;
    }
    return true;
}

bool remover_enter(char* input[], size_t* pos) {
    size_t longitud = strlen(input[*pos]);
    if(longitud > 0 && input[*pos][longitud-1] == '\n') {
        input[*pos][longitud-1] = '\0';
        return true;    
    }
    return false;
}

bool cantidad_parametros_correctos(char* input[], size_t pos, size_t cantidad_parametros, char* parametros[]) {
    size_t parametros_contados = 0;
    while(iterar_hasta_palabra_valida(input, &pos)) {
        if(parametros_contados >= cantidad_parametros) return false;
        remover_enter(input, &pos);
        parametros[parametros_contados] = input[pos];
        parametros_contados++;
        pos++;
    }
    if(parametros_contados != cantidad_parametros) return false;
    return true;
}