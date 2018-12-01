#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "utils.h"
#include "constantes.c"
#include "hash.h"

bool info_vuelo(char* input[], aerolinea_t* vuelos, size_t pos){
/*	char* parametros[1];
	if(!cantidad_de_parametros_correcta(input,pos,1, parametros)) return false;
*/	void** vuelo = hash_obtener(vuelos->hash, parametros[0]); 
	if (vuelo == NULL){
		return false;
	}
	else {
		for (int i = 0; i < 9; ++i)
		{
			printf("%s ", vuelo[i]);
			
		}
	return true;	
}
