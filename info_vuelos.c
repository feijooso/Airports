#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


bool info_vuelo(char* input[], aerolinea_t* vuelos) {
	char** vuelo = (char**)hash_obtener(vuelos->hash, input[0]);
	if (vuelo == NULL) return false;
	for (int i = 0; i < 9; i++) {
		printf("%s ", vuelo[i]);
	}
	printf("%s", vuelo[9]);
	printf("\n");
	return true;
}
