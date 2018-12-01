
void ver_tablero(aerolinea_t* vuelos, char* input[],size_t pos){
/*	char* parametros[4];
	if(!cantidad_de_parametros_correcta(input,pos,4, parametros)) return false;
*/	size_t n = parametros[0];
	char* modo = parametros[1];
	char* fecha_inicio = parametros[2];
	char* fecha_fin = parametros[3];
	void* tablero[] = malloc(sizeof(void*) * n);
	char* vector_clave [2] = {fecha_inicio, 0000}
	char* clave_inicial = join(vector_clave, ' - ')
	abb_iter_t* iter = abb_iter_in_crear(vuelos->abb, clave_inicial);
	int i = 0;
	bool seguir = true;
	abb_iter_in_avanzar(iter); //porque se inicializa en el padre (que tiene fecha anterior)
	char* clave = abb_iter_in_ver_actual(iter);
	char* datos[] = split(clave, '-');
	while(i < n && seguir){
		if (vuelos->abb->cmp(fecha_fin, datos[0]) > 0){
			tablero[i] = datos;
			seguir = abb_iter_in_avanzar(iter);
			if (seguir){
				datos = abb_iter_in_ver_actual(iter);
				i++;
			}
	}

	if(modo == asc){

		for (int j = 0; j <= i; ++j) 
		{
			printf("%s",tablero[j][0]);
			printf("%s",' - ');
			printf("%s",tablero[j][1]);
		}
	}

	if(modo == desc){

		for (int j = i; j >= 0; --j) 
		{
			printf("%s",tablero[j][0]);
			printf("%s",' - ');
			printf("%s",tablero[j][1]);
		}
	}

	return true;
}
