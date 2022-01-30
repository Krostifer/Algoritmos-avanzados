#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrcutura que representa un valor junto con su ponderacion
//Probablemente se cambie el nombre
typedef struct Elemento{
    int valor;
    int ponderacion;

} Elemento;

//Estructura que contiene la informacion de una combinacion de Elementos
typedef struct Combinacion{
	int sumaValor;	//Suma total de todos los valores que componen la combinacion
	int sumaPonderacion;	//Suma total de todas las ponderaciones que componen la combinacion
	int cantidadElementos;	//Cantidad de elementos que componen la combinacion
	int *listaPosiciones;	//Arreglo de enteros que contiene la posicion de los elementos que componen la combinacion
} Combinacion;

//Entrada: Valor del elemento, ponderacion que posee el elemento
//Salida: Elemento ?????
//Objetivo: Crear una estructura Elemento con los datos entregados
Elemento crearElemento(int valorNuevo, int ponderacionNuevo){
	Elemento nuevoElemento;
	nuevoElemento.valor = valorNuevo;
	nuevoElemento.ponderacion = ponderacionNuevo;
	
	return nuevoElemento;
}

//Entrada: Dos elementos que compondran la combinacion, junto con el indice que tiene cada elemento dentro del arreglo que los contiene
//Salida: Una combinacion compuesta por ambos elementos ingresados
//Objetivo: Crear una combinacion con dos elementos
Combinacion crearCombinacionDoble(Elemento primerElemento, int primerIndice, Elemento segundoElemento, int segundoIndice){
	Combinacion nuevaCombinacion;
	nuevaCombinacion.sumaValor = primerElemento.valor + segundoElemento.valor;
	nuevaCombinacion.sumaPonderacion = primerElemento.ponderacion + segundoElemento.ponderacion;
	nuevaCombinacion.cantidadElementos = 2;
	//Se crea el arreglo para almacenar las posiciones de los elementos
	nuevaCombinacion.listaPosiciones = malloc(sizeof(int)*2);
	//Se guardan las posiciones en el arreglo
	nuevaCombinacion.listaPosiciones[0] = primerIndice;
	nuevaCombinacion.listaPosiciones[1] = segundoIndice;
	//Se retorna la combinacion creada
	return nuevaCombinacion;
}

//Entrada: Combinacion existente a la que se le agregara un elemento, el elemento a agregar y su indice dentro del arreglo que lo contiene
//Salida: Una nueva combinacion formada por la original, mas el nuevo elemento
//Objetivo: Agregar un nuevo elemento a una combinacion existente
Combinacion agregarElementoACombinacion(Combinacion combinacion, Elemento nuevoElemento, int indiceElemento){
	Combinacion nuevaCombinacion;
	nuevaCombinacion.sumaValor = combinacion.sumaValor + nuevoElemento.valor;
	nuevaCombinacion.sumaPonderacion = combinacion.sumaPonderacion + nuevoElemento.ponderacion;
	nuevaCombinacion.cantidadElementos = combinacion.cantidadElementos + 1;
	//Se crea el arreglo para almacenar las posiciones de todos los elementos
	nuevaCombinacion.listaPosiciones = malloc(sizeof(int)*nuevaCombinacion.cantidadElementos);
	//Se copian las posiciones de los elementos de la combinacion original a la nueva
	for(int i=0; i<combinacion.cantidadElementos; i++){
		nuevaCombinacion.listaPosiciones[i] = combinacion.listaPosiciones[i];
	}
	//Se agrega el indice del nuevo elemento agregado al final de la lista de posiciones
	nuevaCombinacion.listaPosiciones[combinacion.cantidadElementos] = indiceElemento;
	//Se retorna la nueva combinacion
	return nuevaCombinacion;
}

//Entrada: Lista de combinaciones existente, cantidad de elementos que tiene esta lista y la nueva combinacion a agregar
//Salida: Nueva lista de combinaciones con las combinaciones originales y la nueva
//Objetivo: Crear una nueva lista de combinaciones formada por una lista de combinaciones existente mas una nueva combinacion
Combinacion *agregarCombinacion(Combinacion *listaCombinaciones, int *cantidadCombinaciones, Combinacion nuevaCombinacion){
	Combinacion * nuevaListaCombinaciones = (Combinacion *)malloc(sizeof(Combinacion)*(*cantidadCombinaciones+1));
	//Se agregan las combinaciones originales a la nueva lista
	for(int i=0; i < *cantidadCombinaciones; i++){
		nuevaListaCombinaciones[i] = listaCombinaciones[i];
	}
	//Se agrega la nueva combinacion a la nueva lista
	nuevaListaCombinaciones[*cantidadCombinaciones] = nuevaCombinacion;
	//Se actualiza la nueva cantidad de elementos que posee la nueva lista de combinaciones
	*cantidadCombinaciones = *cantidadCombinaciones + 1;
	//Se libera la memoria utilizada la la anterior lista de combinaciones
	free(listaCombinaciones);
	//Se retorna la nueva lista de combinaciones
	return nuevaListaCombinaciones;
}

//Entrada: Una combinacion ya existente, el nuevo elemento, el indice del elemento dentro del arreglo que lo contiene y el limite de ponderacion total
//Salida: Un entero igual a 0 si no se pueden combinar o un entero igual a 1 si es que se pueden combinar 
//Objetivo: Comprueba si es posible agregar un elemento a una combinacion. Se comprueba que no se supere el limite de la ponderacion total y que el elemento no se encuentre ya en esta combinacion
int sePuedeCombinar(Combinacion combinacion, Elemento nuevoElemento, int indiceElemento, int ponderacionMaxima){
	//Se comprueba que el nuevo elemento no se encuentre dentro de la combinacion ya existente
	for(int i=0; i < combinacion.cantidadElementos; i++){
		if(combinacion.listaPosiciones[i] == indiceElemento){
			return 0;	//El elemento ya se encuentra en esta combinacion
		}
	}
	//Si no se encuentra en la combinacion, se comprueba que no se supere el limite de ponderacion total
	if((combinacion.sumaPonderacion + nuevoElemento.ponderacion) <= ponderacionMaxima){
		return 1;	//Si no supera el limite de ponderacion total, se puede crear la combinacion
	}
	else{
		return 0;	//Si no cumple con el limite de ponderacion total, no se puede crear la combinacion
	}
}

//Entrada: Lista que contiene todas las combinaciones creadas, la cantidad de combinaciones que posee esta lista y la posible nueva combinacion
//Salida: Un entero igual a 0 si ya existe la combinacion o un entero igual a 1 si no es una combinacion ya existente
//Objetivo: Comprobar si la posible nueva combinacion generada no existe dentro de la lista de combinaciones ya generadas
int combinacionUnica(Combinacion *listaCombinaciones, int cantidadCombinaciones, Combinacion posibleCombinacion){
	//Se recorre la lista de combinaciones
	for(int i=0; i < cantidadCombinaciones; i++){
		if(listaCombinaciones[i].cantidadElementos == posibleCombinacion.cantidadElementos){
			//Se comprueba que todas la posiciones de las combinaciones no coincidan
			int posicionesIguales = 0;
			//Se recorre la lista de posiciones de la actual combinacion existente
			for(int j=0; j < listaCombinaciones[i].cantidadElementos; j++){
				//Se compara la posicion con todas las posiciones de la lista de posiciones que comforman la posible combinacion
				for(int k=0; k < posibleCombinacion.cantidadElementos; k++){
					if(listaCombinaciones[i].listaPosiciones[j] == posibleCombinacion.listaPosiciones[k]){
						posicionesIguales = posicionesIguales + 1;	//Si las posiciones coinciden, se aumenta el contador en 1
					}
				}
			}
			
			if(posicionesIguales == posibleCombinacion.cantidadElementos){
				return 0;	//Todos sus elementos son iguales, por lo que la posible combinacion ya existe
			}
		}
	}
	
	return 1;	//Si sus elementos son diferentes a todas las combinaciones ya generadas, la posible combinacion es unica
}

//Entrada: Lista de elementos, la cantidad de elementos en la lista de elementos, direccion de la variable donde se almacenara la cantidad de combinaciones de la lista, la ponderacion total maxima y la direccion de la mayor ponderacion total encontrada
//Salida: Lista de combinaciones con todas las combinaciones de 2 elementos
//Objetivo: Generar todas las combinaciones de dos elementos 
Combinacion *generarCombinacionesDobles(Elemento *listaElementos, int cantidadElementos, int *cantidadCombinaciones, int limitePonderacion, int *mayorSumaEncontrada){
	Combinacion *listaCombinaciones = (Combinacion *)malloc(sizeof(Combinacion)*(*cantidadCombinaciones));
	for(int i = 0; i < cantidadElementos; i++){
		for(int j = i+1; j < cantidadElementos; j++){
			//Se comprueba que la combinacion no supere el limite de ponderacion total
			if((listaElementos[i].ponderacion + listaElementos[j].ponderacion) <= limitePonderacion){
				//Si no supera el limite, se crea la combinacion y se agrega a la lista
				Combinacion nuevaCombinacion = crearCombinacionDoble(listaElementos[i], i, listaElementos[j], j);
				listaCombinaciones = agregarCombinacion(listaCombinaciones, cantidadCombinaciones, nuevaCombinacion);
				printf("%d\n", *cantidadCombinaciones);
				//Se comprueba si la nueva combinacion tiene una ponderacion total mayor a la actual mayor ponderacion econtrada
				
				if(nuevaCombinacion.sumaValor > *mayorSumaEncontrada){
					//La suma total de ponderacion de la nueva combinacion pasa a ser la nueva mayor ponderacion encontrada
					*mayorSumaEncontrada = nuevaCombinacion.sumaValor;
				}
			}
		}
	}
	//Se retorna la lista de combinaciones
	return listaCombinaciones;
}

//Entrada: Lista de elementos, la cantidad de elementos en la lista de elementos, la lista de combinaciones, direccion de la variable donde se almacenara la cantidad de combinaciones de la lista, 
//         la ponderacion total maxima y la direccion de la mayor ponderacion total encontrada
//Salida: Lista de combinaciones con todas las combinaciones posibles
//Objetivo: Generar todas las combinaciones
Combinacion *generarTodasLasCombinaciones(Elemento *listaElementos, int cantidadElementos, Combinacion *listaCombinaciones, int *cantidadCombinaciones, int limitePonderacion, int *mayorSumaEncontrada){
	//Se recorre la lista de combinaciones
	for(int i=0; i < *cantidadCombinaciones; i++){
		//Se recorre la lista de elementos
		for(int j=0; j < cantidadElementos; j++){
			//Se comprueba que se puedea generar la nueva solucion
			if(sePuedeCombinar(listaCombinaciones[i], listaElementos[j], j, limitePonderacion)){
				//Si se puede combinar se crea la posible nueva combinacion
				Combinacion nuevaCombinacion = agregarElementoACombinacion(listaCombinaciones[i], listaElementos[j], j);
				//Se comprueba que esta nueva posible combinacion no haya sido previamente generada
				if(combinacionUnica(listaCombinaciones, *cantidadCombinaciones, nuevaCombinacion)){
					listaCombinaciones = agregarCombinacion(listaCombinaciones, cantidadCombinaciones, nuevaCombinacion);
					printf("%d\n", *cantidadCombinaciones);
					//Se comprueba si la nueva combinacion tiene una ponderacion total mayor a la actual mayor ponderacion econtrada
					if(nuevaCombinacion.sumaValor > *mayorSumaEncontrada){
						//La suma total de ponderacion de la nueva combinacion pasa a ser la nueva mayor ponderacion encontrada
						*mayorSumaEncontrada = nuevaCombinacion.sumaValor;
					}
				}
			}
		}
	}
	//Se retorna la lista de combinaciones
	return listaCombinaciones;
}

//Entrada: Nombre del archivo de entrada, la direccion de memoria de la variable donde se almacena la cantidad de elementos y el limite de suma
//Salida: No se retorna nada, solamente se actualiza la informacion
//Objetivo: Extraer la cantidad de elementos y el limite de la suma del archivo de entrada 
void extrarInfoNombre(char const *nombreArchivo, int *cantidadElementos, int *limiteSuma){
	char auxChar[6];
	int iniN, finN, iniLimite, finLimite;
	int estaIniN = 0; 
	int estaFinN = 0; 
	int largoNombre = strlen(nombreArchivo);
	int guiones = 0;
	
	for(int i=0; i < largoNombre; i++){
		if(nombreArchivo[i] == '_'){
			guiones++;
		}
		
		if(guiones == 2 && estaIniN == 0){
			iniN = i+1;
			estaIniN = 1;
		}
		
		else if(guiones == 3 && estaFinN == 0){
			finN = i;
			estaFinN = 1;
			iniLimite = i+1;
		}
		
		else if(nombreArchivo[i] == '.'){
			finLimite = i;
			//Se extrae la cantidad de elementos del nombre
			strncpy(auxChar, &nombreArchivo[iniN], (finN - iniN));
			//Se transforma a un entero
			int n = atoi(auxChar);
			//Se actualiza su valor
			*cantidadElementos = n;
			
			//Se extrae el limite de la suma del nombre
			strncpy(auxChar, &nombreArchivo[iniLimite], (finLimite - iniLimite));
			//Se transforma a un entero
			int limite = atoi(auxChar);
			//Se actualiza su valor
			*limiteSuma = limite;
			//Se libera la memoria utilizada por el string
			free(auxChar);
			
			return ;
		}
		
	}
}

//Entrada: Nombre del archivo de entrada, direccion de memoria de las variables donde se almacena la cantidad de elementos, el limite de la suma y la mayor suma encontrada
//Salida: Lista de Elemento con todos los elementos presentes en el archivo de entrada
//Objetivo: Leer el archivo de entrada y almacenar la informacion contenida en el donde corresponda
Elemento *leerEntrada(char const *nombreArchivo, int *cantidadElementos, int *limiteSuma, int *mayorSumaEncontrada){
	FILE *pf;
	//Se abre el archivo en modo lectura
	pf = fopen(nombreArchivo, "r");

	//En caso de que el archivo de entrada no exista
	if (pf == NULL)
	{
		*cantidadElementos = -1;
		Elemento * listaElementos = (Elemento*)malloc(sizeof(Elemento)*(0));
		return listaElementos;
	}
	
	extrarInfoNombre(nombreArchivo, cantidadElementos, limiteSuma);
	
	Elemento *listaElementos = (Elemento*)malloc(sizeof(Elemento)*(*cantidadElementos));
	
	for(int i=0; i < *cantidadElementos; i++){
		//Se crea un nuevo postulante
		Elemento nuevoElemento;

		//Se leen y almacenan los datos del elemento`
		fscanf(pf, "%d", &nuevoElemento.valor);
		fscanf(pf, "%d", &nuevoElemento.ponderacion);

		//Se comprueba si el nuevo valor es el mayor encontrado
		if (nuevoElemento.valor > *mayorSumaEncontrada && nuevoElemento.ponderacion <= *limiteSuma){
			*mayorSumaEncontrada = nuevoElemento.valor;
		}
		//Se agrega el postulante a la lista de elementos
		listaElementos[i] = nuevoElemento;
	}
	
	//Se cierra el archivo de entrada
	fclose(pf);

	//Se retorna la lista de postulantes
	return listaElementos;
}

int main(int argc, char const *argv[]){
	//Se definen las variables para almacenar la cantidad de elementos y el limite de ponderacion total
	int cantidadElementos, limitePonderacion;
	int mayorSumaEncontrada = 0;
	int cantidadCombinaciones = 0;
	
	//Se lee el archivo de entrada
	Elemento *listaElementos = leerEntrada(argv[1], &cantidadElementos, &limitePonderacion, &mayorSumaEncontrada);
	
	//Se comprueba que no exista un error al momento de leer el archivo
	
	if(cantidadElementos == 0){
		printf("No existen elementos en el archivo de entrada\n");
		return 0;
	}
	
	else if(cantidadElementos == -1){
		printf("El archivo de entrada indicado no existe\n");
		return 0;
	}
	/*
	else if(cantidadElementos == -2){
		printf("Error en el formato del archivo de entrada\n");
		return 0;
	}
	*/
	
	
	//Si no existieron errores al momento de leer el archivo de entrada, se procede a generar las combinaciones
	Combinacion *combinacionesGeneradas = generarCombinacionesDobles(listaElementos, cantidadElementos, &cantidadCombinaciones, limitePonderacion, &mayorSumaEncontrada);
	//Luego se agregara de a un elemento a cada combinacion generada previo y durante este proceso
	combinacionesGeneradas = generarTodasLasCombinaciones(listaElementos, cantidadElementos, combinacionesGeneradas, &cantidadCombinaciones, limitePonderacion, &mayorSumaEncontrada);
	
	//Se genera la salida
	printf("La mayor suma encontrada fue: %d\n", mayorSumaEncontrada);
	
	return 0;
}

