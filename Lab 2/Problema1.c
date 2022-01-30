#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Estrcutura que representa un valor junto con su ponderacion
//Probablemente se cambie el nombre
typedef struct Elemento{
    float valor;			//Entero con el valor del elemento
    float ponderacion;	//Entero con la ponderacion del elemento
	float valor_peso;	//Relacion del valor con respecto al peso(ponderacion)

} Elemento;

//Entrada: Valor del elemento, ponderacion que posee el elemento
//Salida: Elemento con la información almacenada
//Objetivo: Crear una estructura Elemento con los datos entregados
Elemento crearElemento(float valorNuevo, float ponderacionNuevo){
	Elemento nuevoElemento;
	nuevoElemento.valor = valorNuevo;
	nuevoElemento.ponderacion = ponderacionNuevo;
	nuevoElemento.valor_peso = valorNuevo/ponderacionNuevo;
	
	return nuevoElemento;
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

//Entrada: Nombre del archivo de entrada, direccion de memoria de las variables donde se almacena la cantidad de elementos, el limite de la suma
//Salida: Lista de Elemento con todos los elementos presentes en el archivo de entrada
//Objetivo: Leer el archivo de entrada y almacenar la informacion contenida en el donde corresponda
Elemento *leerEntrada(char const *nombreArchivo, int *cantidadElementos, int *limiteSuma){
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
	//Se asigna memoria para el arreglo
	Elemento *listaElementos = (Elemento*)malloc(sizeof(Elemento)*(*cantidadElementos));
	float valorTemporal, ponderacionTemporal;
	//Se recorre el archivo de entrada
	for(int i=0; i < *cantidadElementos; i++){
		//Se leen y almacenan los datos del elemento`
		fscanf(pf, "%f", &valorTemporal);
		fscanf(pf, "%f", &ponderacionTemporal);
		
		//Se crea un nuevo postulante
		Elemento nuevoElemento = crearElemento(valorTemporal, ponderacionTemporal);
		//Se agrega el postulante a la lista de elementos
		listaElementos[i] = nuevoElemento;
	}
	//Se cierra el archivo de entrada
	fclose(pf);
	//Se retorna la lista de postulantes
	return listaElementos;
}


void merge(Elemento arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    Elemento L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i].valor_peso >= R[j].valor_peso) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 

void mergeSort(Elemento arr[], int l, int r){
    if (l < r) {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l + (r - l) / 2;
 
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
 
        merge(arr, l, m, r);
    }
}
 

int main(int argc, char const *argv[]){
	//Se definen las variables para almacenar la cantidad de elementos y el limite de ponderacion total
	int cantidadElementos, limitePonderacion;
	//Se define las variables para tomar el tiempo de ejecucion
	clock_t tiempo_inicio, tiempo_final;
	//Se toma el tiempo de inicio
	tiempo_inicio = clock();

	//Se lee el archivo de entrada
	Elemento *listaElementos = leerEntrada(argv[1], &cantidadElementos, &limitePonderacion);
	
	//Se comprueba que no exista un error al momento de leer el archivo
	if(cantidadElementos == 0){
		printf("No existen elementos en el archivo de entrada\n");
		return 0;
	}
	
	else if(cantidadElementos == -1){
		printf("El archivo de entrada indicado no existe\n");
		return 0;
	}
	
	//Se ordena la listade mayor a menor segun su valor_peso 
	mergeSort(listaElementos, 0, cantidadElementos-1);
	printf("Elementos: %d, limite: %d\n", cantidadElementos, limitePonderacion);
	float valorTotal, ponderacionTotal;
	int i = 0;
	
	while(i< cantidadElementos && ponderacionTotal < limitePonderacion){
		if(ponderacionTotal + listaElementos[i].ponderacion <= limitePonderacion){
			valorTotal = valorTotal + listaElementos[i].valor;
			ponderacionTotal = ponderacionTotal + listaElementos[i].ponderacion;
			//printf("Entra: %.0f, %.0f\n", listaElementos[i].valor, listaElementos[i].ponderacion);
		}
		i = i + 1;
	}
	
	//Se toma el tiempo al termino de la ejecucion
	tiempo_final = clock();
	//Se calcula la cantidad de segundos 
	double segundos = (tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; 
	//Se muestra la mayot suma encontrada y el tiempo de ejecucion
	printf("La mayor suma encontrada fue: %.0f\n", valorTotal);
	printf("Tiempo de ejecucion: %f\n",segundos); 
	
	return 0;
}