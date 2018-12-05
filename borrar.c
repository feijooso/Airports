#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "strutil.h"

#define CANTIDAD_DE_SPLITS_FORMATO 2
#define CANTIDAD_DE_SPLITS_FECHA 3
#define CANTIDAD_DE_SPLITS_TIEMPO 3
#define LONGITUD_ANIO 4
#define LONGITUD_FECHA 2
#define LONGITUD_TIEMPO 2

bool borrar_dos(char** uno, char** dos, bool res) {
    free_strv(uno);
    free_strv(dos);
    return res;
}

bool borrar_tres(char** uno, char** dos, char** tres, bool res) {
    free_strv(uno);
    free_strv(dos);
    free_strv(tres);
    return res;
}

bool parametro_tamanio_correcto(char* parametro[], size_t tamanio_correcto) {
    size_t contador = 0;
    while(parametro[contador] != NULL) {
        contador++;
    }
    if(contador != tamanio_correcto) return false;
    return true;
}

bool string_tamanio_correcto(char* string, size_t tamanio_correcto) {
    size_t contador = 0;
    while(string[contador] != '\0') {
        contador++;
    }
    if(contador != tamanio_correcto) return false;
    return true;
}

bool es_fecha_valida(char** formato_split) {
    if(formato_split == NULL) return false;
    if(!parametro_tamanio_correcto(formato_split, CANTIDAD_DE_SPLITS_FORMATO)) {
        free_strv(formato_split);
        return false;
    }
    //fecha
    char** fecha = split(formato_split[0], '-');
    if(fecha == NULL) {
        free_strv(formato_split);
        return false;
    }
    if(!parametro_tamanio_correcto(fecha, CANTIDAD_DE_SPLITS_FECHA)) {
        return borrar_dos(formato_split, fecha, false);
    }
    //año-mes-dia
    if(!string_tamanio_correcto(fecha[0], LONGITUD_ANIO)
     || !string_tamanio_correcto(fecha[1], LONGITUD_FECHA)
     || !string_tamanio_correcto(fecha[2], LONGITUD_FECHA) ) {
        return borrar_dos(formato_split, fecha, false);
    }
    //tiempo
    char** tiempo = split(formato_split[1], ':');
    if(tiempo == NULL) {
        return borrar_dos(formato_split, fecha, false);
    }
    if(!parametro_tamanio_correcto(tiempo, CANTIDAD_DE_SPLITS_TIEMPO)) {
        return borrar_tres(formato_split, fecha, tiempo, false);
    }
    //hora:mes:dia
    if(!string_tamanio_correcto(tiempo[0], LONGITUD_TIEMPO)
     || !string_tamanio_correcto(tiempo[1], LONGITUD_TIEMPO)
     || !string_tamanio_correcto(tiempo[2], LONGITUD_TIEMPO) ) {
        return borrar_tres(formato_split, fecha, tiempo, false);
    }
    return borrar_tres(formato_split, fecha, tiempo, true);
}

bool empezar_a_borrar(aerolinea_t* aerolinea, char* fecha_inicio, char* id_inicio, char* fecha_fin, char* id_fin) {
    abb_iter_t* iter = abb_iter_in_crear(aerolinea->abb, fecha_inicio);
    if(iter == NULL) return false;
    return true;
}

bool borrar(aerolinea_t* aerolinea, char* parametros[]) {
    char** formato_inicio = split(parametros[0], 'T');
    char** formato_fin = split(parametros[1], 'T');
    if(!es_fecha_valida(formato_inicio) || !es_fecha_valida(formato_fin)) 
        return false;    
    bool respuesta = empezar_a_borrar(aerolinea, formato_inicio[0], formato_inicio[1], formato_fin[0], formato_fin[1]);
    return borrar_dos(formato_inicio, formato_fin, respuesta);
}