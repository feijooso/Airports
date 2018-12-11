#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include "abb.h"
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pila.h"

typedef struct abb_nodo {
    struct abb_nodo* izq;
    struct abb_nodo* der;
    char* clave;
    void* dato;
} abb_nodo_t;

struct abb {
    abb_nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
};

struct abb_iter {
    abb_t* arbol;
    abb_nodo_t* actual;
    pila_t* pila;
};

/* FUNCIONES AUXILIARES */


abb_nodo_t* crear_nodo(const char* clave, void* dato) {
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if(nodo == NULL) return NULL;
    size_t largo = strlen(clave);
    char* copia_clave = malloc(sizeof(char) * (largo + 1));
    if(copia_clave == NULL) return NULL;
    for(int i=0; i<largo; i++) {
        copia_clave[i] = clave[i];
    }
    copia_clave[largo] = '\0';
    nodo->clave = copia_clave;
    nodo->dato = dato;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

abb_nodo_t* buscar_nodo(const abb_t* arbol, abb_nodo_t* nodo, const char* clave) {
    if(nodo == NULL || arbol->cmp(clave, nodo->clave) == 0)
        return nodo;
    if(arbol->cmp(clave, nodo->clave) > 0)
        return buscar_nodo(arbol, nodo->der, clave);
    return buscar_nodo(arbol, nodo->izq, clave);
}

// si no tiene padre, se devuelve a si mismo
abb_nodo_t* buscar_padre(const abb_t* arbol, abb_nodo_t* nodo, const char* clave) {
    if(nodo == NULL || (nodo->izq == NULL && nodo->der == NULL)) return nodo;

    int comparacion = arbol->cmp(clave, nodo->clave);
    if(comparacion == 0) return nodo;

    if((nodo->der && arbol->cmp(clave, nodo->der->clave) == 0)
       || (nodo->izq && arbol->cmp(clave, nodo->izq->clave) == 0)) {
        return nodo;
    }

    if(comparacion > 0) {
        if(nodo->der == NULL) return nodo;
        return buscar_padre(arbol, nodo->der, clave);
    } else {
        if(nodo->izq == NULL) return nodo;
        return buscar_padre(arbol, nodo->izq, clave);
    }
}

size_t cantidad_hijos(abb_nodo_t* nodo) {
    size_t cantidad = 0;
    if(nodo->izq) cantidad++;
    if(nodo->der) cantidad++;
    return cantidad;
}

/* PRIMITIVAS ABB*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* arbol = malloc(sizeof(abb_t));
    if (arbol == NULL) return NULL;
    arbol->raiz = NULL;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    arbol->cantidad = 0;
    return arbol;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato) {

    if (abb_pertenece(arbol, clave)) { // reemplazar
        abb_nodo_t* nodo1 = buscar_nodo(arbol, arbol->raiz, clave);
        if (arbol->destruir_dato != NULL){
            arbol->destruir_dato(nodo1->dato);
        }
        nodo1->dato = dato;
        return true;
    }
    //guardar
    abb_nodo_t* nodo_nuevo = crear_nodo(clave, dato);
    if (nodo_nuevo == NULL) return false;

    if (arbol->raiz == NULL) {
        arbol->raiz = nodo_nuevo;
    } else {
        abb_nodo_t* nodo = buscar_padre(arbol, arbol->raiz, clave);
        int comparacion = arbol->cmp(clave, nodo->clave);

        if(comparacion > 0) {
            nodo->der = nodo_nuevo;
        } else {
            nodo->izq = nodo_nuevo;
        }
    }
    arbol->cantidad++;

    return true;
}

void* abb_borrar(abb_t* arbol, const char* clave) {
    abb_nodo_t* nodo_a_borrar = buscar_nodo(arbol, arbol->raiz, clave);
    if(nodo_a_borrar == NULL) return NULL;
    void* dato = nodo_a_borrar->dato;
    abb_nodo_t* padre = buscar_padre(arbol, arbol->raiz,clave);
    size_t cant_hijos = cantidad_hijos(nodo_a_borrar);
    int comparacion = arbol->cmp(padre->clave, nodo_a_borrar->clave);

    if(cant_hijos == 0) {
        if(comparacion == 0) { //es raiz
            arbol->raiz = NULL;
        }
        else if(comparacion > 0)
            padre->izq = NULL;
        else
            padre->der = NULL;

    } else if(cant_hijos == 1) {
        abb_nodo_t* aux;
        if(nodo_a_borrar->izq != NULL) {
            aux = nodo_a_borrar->izq;
        } else {
            aux = nodo_a_borrar->der;
        }
        if(comparacion == 0) { //es raiz
            arbol->raiz = aux;
        } else {
            if(comparacion > 0) {//el hijo q tiene es a la izq del padre
                padre->izq = aux;
            } else {
                padre->der = aux;
            }
        }

    } else { //tiene 2 hijos
        abb_nodo_t* nieto = nodo_a_borrar->der;
        abb_nodo_t* padre_nieto = nodo_a_borrar;
        bool avanzo = false;
        while(nieto->izq) {
            avanzo = true;
            padre_nieto = nieto;
            nieto = nieto->izq;
        }
        //inicio swaps
        char* clave_aux = nodo_a_borrar->clave;
        void* dato_aux = nodo_a_borrar->dato;
        nodo_a_borrar->clave = nieto->clave;
        nodo_a_borrar->dato = nieto->dato;
        nieto->dato = dato_aux;
        nieto->clave = clave_aux;
        //fin swaps
        if (avanzo) padre_nieto->izq = nieto->der;
        else padre_nieto->der = nieto->der;
        free(nieto->clave);
        free(nieto);
        arbol->cantidad--;
        return dato_aux;
    }
    arbol->cantidad--;
    free(nodo_a_borrar->clave);
    free(nodo_a_borrar);
    return dato;
}

//devuelve null si no esta en el arbol
void* abb_obtener(const abb_t* arbol, const char* clave) {
    abb_nodo_t* nodo = buscar_nodo(arbol, arbol->raiz, clave);
    if(nodo == NULL) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t* arbol, const char* clave) {
    return buscar_nodo(arbol, arbol->raiz, clave) != NULL;
}

size_t abb_cantidad(abb_t* arbol) {
    return arbol->cantidad;
}

void abb_borrar_nodos(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    if (nodo == NULL) return;
    abb_borrar_nodos(nodo->izq, destruir_dato);
    abb_borrar_nodos(nodo->der, destruir_dato);
    if(destruir_dato != NULL){
        destruir_dato(nodo->dato);
    }
    free(nodo->clave);
    free(nodo);
}

void abb_destruir(abb_t* arbol) {
    abb_borrar_nodos(arbol->raiz, arbol->destruir_dato);
    free(arbol);
}


/* PRIMITIVAS Y FUNCIONES AUXILIARES ITERADOR INTERNO */

void abb_iterar(abb_nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra, bool* seguir) {

    if(nodo == NULL) return;

    abb_iterar(nodo->izq, visitar, extra, seguir);
    if (*seguir){
        *seguir = visitar(nodo->clave, nodo->dato, extra);
    }
    abb_iterar(nodo->der, visitar, extra, seguir);
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){

    bool seguir = true;
    abb_iterar(arbol->raiz, visitar, extra, &seguir);

}

/* PRIMITIVAS Y FUNCIONES AUXILIARES ITERADOR EXTERNO */

abb_iter_t *abb_iter_in_crear(abb_t *arbol, char* clave) {

    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (iter == NULL) return NULL;

    pila_t* pila = pila_crear();
    if(pila == NULL){
        free(iter);
        return NULL;
    }
    iter->pila = pila;
    iter->arbol = arbol;

    if (arbol->raiz == NULL){
        iter->actual = NULL;
        return iter;
    }
    abb_nodo_t* nodo = arbol->raiz;

    bool seguir = true;
    pila_apilar(iter->pila, nodo);
    while(nodo->izq != NULL && seguir){
        if (strcmp(clave, nodo->izq->clave) > 0) seguir = false;
        else { 
            pila_apilar(iter->pila, nodo->izq);
            nodo = nodo->izq;
        }
    }

    iter->actual = pila_ver_tope(iter->pila);

    return iter;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {

    if (pila_esta_vacia(iter->pila)) return true;
    return false;


}

bool abb_iter_in_avanzar(abb_iter_t *iter) {

    if (abb_iter_in_al_final(iter)) return false;

    pila_desapilar(iter->pila);

    if(iter->actual->der != NULL) {
        abb_nodo_t* nodo = iter->actual->der;
        pila_apilar(iter->pila,nodo);
        while(nodo->izq != NULL) {
            pila_apilar(iter->pila,nodo->izq);
            nodo = nodo->izq;
        }
    }
    iter->actual = pila_ver_tope(iter->pila);

    return true;

}

char* abb_iter_in_ver_actual(const abb_iter_t *iter) {
    abb_nodo_t* nodo = pila_ver_tope(iter->pila);
    if (nodo == NULL) return NULL;
    return nodo->clave;
}


void abb_iter_in_destruir(abb_iter_t* iter) {

    pila_destruir(iter->pila);
    free(iter);

}

void* abb_iter_borrar(abb_iter_t* iter) {
    if(abb_iter_in_al_final(iter)) return NULL;
    abb_nodo_t* nodo_anterior = iter->actual;
    abb_iter_in_avanzar(iter);
    return abb_borrar(iter->arbol, nodo_anterior->clave);
}