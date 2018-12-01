#include <stdbool.h>
#include "pila.h"
#include "abb.h"
#include "hash.h"
#include "utils.c"
#include "constantes.c"

typedef struct aerolinea {
    hash_t* hash;
    abb_t* abb;
} aerolinea_t;

aerolinea_t* aerolinea_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato_abb, hash_destruir_dato_t destruir_dato_hash);

void aerolinea_destruir(aerolinea_t* aerolinea);

bool agregar_archivo(aerolinea_t* aerolinea, char* input[], size_t pos);