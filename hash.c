#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LARGO_INICIAL 50
#define CARGA 0.53

typedef void (*hash_destruir_dato_t)(char**);

typedef enum estado {
	libre,
	ocupado,
	borrado
} estado_t;

typedef struct hash_campo {
    char* clave;
    void* valor;
    estado_t estado; 
} hash_campo_t;

typedef struct hash {
	size_t cantidad;                  
    size_t largo;                
	size_t elementos_no_libres;     
    hash_campo_t** tabla; 
    hash_destruir_dato_t destruir_dato; 
} hash_t;

typedef struct hash_iter {	//no estoy seguro si estos son los atributos del iterador.
	const hash_t* hash;
	size_t posicion;
} hash_iter_t;

bool hash_guardar(hash_t *hash, const char *clave, void *dato);
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);
void *hash_borrar(hash_t *hash, const char *clave);
void *hash_obtener(const hash_t *hash, const char *clave);
bool hash_pertenece(const hash_t *hash, const char *clave);
size_t hash_cantidad(const hash_t *hash);
void hash_destruir(hash_t *hash);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura hash fue inicializada
 * Post: La estructura hash fue destruida*/
void hash_destruir(hash_t* hash) {
	for(int i=0; i<hash->largo; i++) {
		hash_campo_t* campo = hash->tabla[i];
		if(campo == NULL) break;
		if(campo->estado != libre) {
			free(campo->clave);
		}
		if(campo->estado == ocupado && hash->destruir_dato != NULL) hash->destruir_dato((char**)campo->valor);
		free(campo);
	}
	free(hash->tabla);
	free(hash);
}

size_t funcion_hash(const char* clave, size_t largo_tabla) {
    size_t hashval;
    for (hashval = 0; *clave != '\0'; clave++) hashval = *clave + 31*hashval;
    return hashval % largo_tabla;
}

hash_campo_t* campo_crear() {
	hash_campo_t* campo = calloc(1, sizeof(hash_campo_t));
	if(campo == NULL) return NULL;
	campo->estado = libre;
	return campo;
}

bool rellenar_tabla(hash_t* hash) {
	for(size_t i=0; i<hash->largo; i++) {
		hash_campo_t* campo = campo_crear();
		if(campo == NULL) {
			return false;
		}
		hash->tabla[i] = campo;
	}
	return true;
}

bool claves_son_iguales(char* campo_clave, const char* clave) {
	return campo_clave != NULL && strcmp(campo_clave, clave) == 0;
}

void hasheando_por_hay(size_t* posicion, const hash_t* hash, const char* clave) {
	while(*posicion < hash->largo && hash->tabla[*posicion]->estado != libre && !claves_son_iguales(hash->tabla[*posicion]->clave, clave)) {
		*posicion += 1;
	}
	if(*posicion == hash->largo) {
		*posicion = 0;
		hasheando_por_hay(posicion, hash, clave);
	}
}

/* Crea el hash*/
//preguntar por la carga del hash abierto.
hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if(hash == NULL) return NULL;
	hash_campo_t** tabla = malloc(sizeof(hash_campo_t*) * LARGO_INICIAL);
	if(tabla == NULL) return NULL;
	hash->tabla = tabla;
	hash->cantidad = 0;
	hash->elementos_no_libres = 0;
	hash->largo = LARGO_INICIAL;
	if(!rellenar_tabla(hash)) {
		hash_destruir(hash);
		return NULL;
	}	
	hash->destruir_dato = destruir_dato;
	return hash;
}

bool necesita_redimensionar(hash_t* hash) {
	return CARGA <= ((float)hash->elementos_no_libres / (float)hash->largo);
}

bool redimensionar(hash_t* hash) {
	size_t nuevo_largo = hash->largo * 2;
	hash_campo_t** nueva_tabla = malloc(nuevo_largo * sizeof(hash_campo_t*));
	if(nueva_tabla == NULL) return false;
	hash_campo_t** vieja_tabla = hash->tabla;
	hash->tabla = nueva_tabla;
	size_t viejo_largo = hash->largo; 
	hash->largo = nuevo_largo;
	size_t vieja_cantidad = hash->cantidad;
	hash->cantidad = 0;
	size_t viejo_elementos_no_libres = hash->elementos_no_libres;
	hash->elementos_no_libres = 0;
	if(!rellenar_tabla(hash)) {
		hash->largo = viejo_largo;
		hash->tabla = vieja_tabla;
		hash->cantidad = vieja_cantidad;
		hash->elementos_no_libres = viejo_elementos_no_libres;
		return false;
	}
	for(size_t i = 0; i < viejo_largo; i++) {
		hash_campo_t* campo = vieja_tabla[i];
		if(campo->estado == ocupado) {
			if(!hash_guardar(hash, campo->clave, campo->valor)) {
				return false;
			}
			free(campo->clave);
		}
		if (campo->estado == borrado){
			free(campo->clave);
		}
		free(campo);
	}
	free(vieja_tabla);
	return true;
}

void copiar(char* destino, const char* origen) {
	int i = 0;
	while(origen[i] != '\0') {
		destino[i] = origen[i];
		i++;
	}
	destino[i] = '\0';
}

/* Guarda un elemento en el hash, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura hash fue inicializada
 * Post: Se almacenó el par (clave, dato)*/
bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
	if(necesita_redimensionar(hash) && !redimensionar(hash))return false;
	size_t posicion = funcion_hash(clave, hash->largo);
	hasheando_por_hay(&posicion, hash, clave);
	hash_campo_t* campo = hash->tabla[posicion];
	if(campo->estado == borrado || !claves_son_iguales(campo->clave, clave)) {
		hash->cantidad++;
		if(campo->estado == borrado) 
			free(campo->clave);
		char* copia_clave = malloc(sizeof(char)* strlen(clave) +1);
		if(copia_clave == NULL) return false;
		copiar(copia_clave, clave);
		campo->clave = copia_clave;
		hash->elementos_no_libres++;
	}
	if(hash->destruir_dato != NULL && campo->estado == ocupado) hash->destruir_dato(campo->valor);
	campo->valor = dato;
	campo->estado = ocupado;
	return true;
}

/* Obtiene el valor de un elemento del hash, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura hash fue inicializada*/
void* hash_obtener(const hash_t* hash, const char* clave) {
	size_t posicion = funcion_hash(clave, hash->largo);
	hasheando_por_hay(&posicion, hash, clave);
	hash_campo_t* campo = hash->tabla[posicion];
	if(campo->estado == borrado || !claves_son_iguales(campo->clave, clave)) return NULL;
	return hash->tabla[posicion]->valor;
}

/* Determina si clave pertenece o no al hash.
 * Pre: La estructura hash fue inicializada*/
bool hash_pertenece(const hash_t* hash, const char* clave) {
	size_t posicion = funcion_hash(clave, hash->largo);
	hasheando_por_hay(&posicion, hash, clave);
	hash_campo_t* campo = hash->tabla[posicion];
	if(campo->estado == borrado) return false;
	return claves_son_iguales(campo->clave, clave);
}

/* Borra un elemento del hash y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura hash fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void* hash_borrar(hash_t* hash, const char* clave) {
	size_t posicion = funcion_hash(clave, hash->largo);
	hasheando_por_hay(&posicion, hash, clave);
	hash_campo_t* campo = hash->tabla[posicion];
	void* valor = campo->valor;
	if(campo->estado == borrado) return NULL;
	campo->estado = borrado;
	hash->cantidad--;
	return valor;
}

/* Devuelve la cantidad de elementos del hash.
 * Pre: La estructura hash fue inicializada*/
size_t hash_cantidad(const hash_t* hash) {
	return hash->cantidad;
}

/* Iterador del hash */

// Comprueba si terminó la iteración
bool hash_iter_al_final(const hash_iter_t* iter) { 
	return iter->posicion == iter->hash->largo;
}

void ocupando_por_hay(hash_iter_t* iterador){
	while(!hash_iter_al_final(iterador) && iterador->hash->tabla[iterador->posicion]->estado != ocupado) {
		iterador->posicion++;
	}
}

void iterando_por_hay(hash_iter_t* iter) {
	iter->posicion++;
	ocupando_por_hay(iter);
}

// Crea iterador
hash_iter_t* hash_iter_crear(const hash_t* hash) {
	hash_iter_t* iterador = malloc(sizeof(hash_iter_t));
	if(iterador == NULL) return NULL;
	iterador->hash = hash;
	iterador->posicion = 0;
	ocupando_por_hay(iterador);
	return iterador;
}

// Avanza iterador
bool hash_iter_avanzar(hash_iter_t* iter) {
	if(hash_iter_al_final(iter)) return false;
	iterando_por_hay(iter);
	return true;
}

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char* hash_iter_ver_actual(const hash_iter_t* iter) {
	if(hash_iter_al_final(iter)) return NULL;
	return iter->hash->tabla[iter->posicion]->clave;
}

// Destruye iterador
void hash_iter_destruir(hash_iter_t* iter) {
	free(iter);
}