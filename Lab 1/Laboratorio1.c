#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

//Estrcutura que representa un valor junto con su ponderacion
//Probablemente se cambie el nombre
typedef struct Elemento{
    int valor;			//Entero con el valor del elemento
    int ponderacion;	//Entero con la ponderacion del elemento

} Elemento;

//Entrada: Valor del elemento, ponderacion que posee el elemento
//Salida: Elemento con la información almacenada
//Objetivo: Crear una estructura Elemento con los datos entregados
Elemento crearElemento(int valorNuevo, int ponderacionNuevo){
	Elemento nuevoElemento;
	nuevoElemento.valor = valorNuevo;
	nuevoElemento.ponderacion = ponderacionNuevo;
	
	return nuevoElemento;
}

//Entrada: Arreglo de caracteres que representa un numero binario y el largo de este arreglo(cantida de bits del numero binario)
//Salida: Nada, solamente se actualiza la información
//Objetivo: Se le suma un 1 binario al numero binario ingresado
void aumentarBinario(char *numeroBinario, int largo){
	//Si el bit de más a la derecha es un 0
	if(numeroBinario[largo-1] == '0'){
		numeroBinario[largo-1] = '1';
		return ;
	}
	//Caso general, se suma la "reserva" al bit siguiente
	else if(numeroBinario[largo-1] == '1'){
		numeroBinario[largo-1] = '0';
		for(int i=largo-2; i >= 0; i--){
			if(numeroBinario[i] == '0'){
				numeroBinario[i] = '1';
				return ;
			}
			else{
				numeroBinario[i] = '0';
			}
		}
	}
	
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
	if (pf == NULL){
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

//Entrada: String que contendra la representacion del numero binario y el largo de este string
//Salida: Nada, solo se modifica el contenido del string
//Objetivo: Asignar el caracter 0 en todas las posiciones del string
void crearBinario(char *numeroBinario, int largo){
	for(int i=0; i < largo; i++){
		numeroBinario[i] = '0';
	}
	numeroBinario[largo] = '\0';
	return;
}

//Entrada: La lista con todos los elementos, la cantidad de elementos, el string que representa el numero binario, el limite de ponderacion y la direccion de memorio de la variable que almacena la mayor suma de valores encontrada
//Salida: Nada, solo se modifica el valor de la mayor suma encontrada en caso de se mayor a la actualiza
//Objetivo: Generar las combinaciones, comprobar la suma de valores y ponderacion, y en caso de que cumpla el limite y la suma de valores sea mayor a las ya encontrada, se guarda como la mayor encontrada
void generarCombinaciones(Elemento *listaElementos, int cantidadElementos, char *numeroBinario, int limitePonderacion, int *mayorSumaEncontrada){
	aumentarBinario(numeroBinario, cantidadElementos);
	int sumaValorActual = 0;
	int sumaPonderacionActual = 0;
	//Ciclo para recorrer el numero binario y sumar el valor y ponderacion de los elementos que componen la combinacion
	for(int j = cantidadElementos-1; j >= 0; j--){
		if(numeroBinario[j] == '1'){
			sumaValorActual = sumaValorActual + listaElementos[j].valor;
			sumaPonderacionActual = sumaPonderacionActual + listaElementos[j].ponderacion;
		}
		//En caso de que en medio de la suma de la ponderacion de los elementos que componen la combinacion se supera el limite de ponderacion, se descarta la combinacion actual
		if(sumaPonderacionActual > limitePonderacion){
			return;
		}
	}
	//Se comprueba si se cumple el limite de ponderacion
	if(sumaPonderacionActual <= limitePonderacion){
		if(sumaValorActual > *mayorSumaEncontrada){
			*mayorSumaEncontrada = sumaValorActual;
		}
		return;
	}
}

int main(int argc, char const *argv[]){
	//Se definen las variables para almacenar la cantidad de elementos y el limite de ponderacion total
	int cantidadElementos, limitePonderacion;
	int mayorSumaEncontrada = 0;
	//Se define las variables para tomar el tiempo de ejecucion
	clock_t tiempo_inicio, tiempo_final;
	//Se toma el tiempo de inicio
	tiempo_inicio = clock();
	
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
	//Se crea un string con un largo igual a la cantidasd de elementos, esto respresentara la cantidad de bits del numero binario
	char binarioCombinaciones[cantidadElementos+1]; 
	//Se asignan 0's en cada posicion del string
	crearBinario(binarioCombinaciones, cantidadElementos);
	
	//Ciclo para generar todas las combinariones (2^n)
	for(int i=1; i < pow(2,cantidadElementos); i++){
		generarCombinaciones(listaElementos, cantidadElementos, binarioCombinaciones, limitePonderacion, &mayorSumaEncontrada);
	}
	
	//Se toma el tiempo al termino de la ejecucion
	tiempo_final = clock();
	//Se calcula la cantidad de segundos 
	double segundos = (tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; 
	//Se muestra la mayot suma encontrada y el tiempo de ejecucion
	printf("La mayor suma encontrada fue: %d\n", mayorSumaEncontrada);
	printf("Tiempo de ejecucion: %f\n",segundos); 
	//Se libera la memoria utilizada por el arreglo de caracteres
	free(binarioCombinaciones);
	
	return 0;
}