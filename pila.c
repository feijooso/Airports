#include "pila.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 10

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

bool redimensionar_pila(pila_t* pila, size_t nuevo_tamanio) {
	void** pointer = realloc(pila->datos, nuevo_tamanio * sizeof(void*));

	if(pointer == NULL) return false;

	pila->datos = pointer;
	pila->capacidad = nuevo_tamanio;
	//printf("Nuevo tamaño de memoria: %zu\n", nuevo_tamanio);
	return true;
}

bool verificar_redimension(pila_t* pila) {
	if(pila->cantidad == pila->capacidad) {
		return redimensionar_pila(pila, pila->capacidad * 2);
	}

	size_t nueva_capacidad = pila->capacidad / 2;
	if(pila->capacidad/4 >= pila->cantidad && nueva_capacidad >= CAPACIDAD_INICIAL) {
		return redimensionar_pila(pila, nueva_capacidad);
	}

	return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
// Crea una pila.
// Post: devuelve una nueva pila vacía.
pila_t* pila_crear() {
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL) return NULL;

	pila->datos = malloc(sizeof(void*) * CAPACIDAD_INICIAL);
	if(pila->datos == NULL) {
		free(pila);
		return NULL;
	}

	pila->capacidad = CAPACIDAD_INICIAL;
	pila->cantidad = 0;

	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.
void pila_destruir(pila_t *pila) {
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.
bool pila_esta_vacia(const pila_t *pila) {
	return pila->cantidad == 0;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor) {
	if(!verificar_redimension(pila))
		return false;

	pila->datos[pila->cantidad] = valor;
	pila->cantidad += 1;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.
void* pila_ver_tope(const pila_t *pila) {
	if(pila_esta_vacia(pila)) return NULL;
	return pila->datos[pila->cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior
// y la pila contiene un elemento menos.
void* pila_desapilar(pila_t *pila) {
	if(pila_esta_vacia(pila)) return NULL;

	void* valor = pila_ver_tope(pila);
	pila->cantidad -= 1;

	verificar_redimension(pila);

	return valor;
}