#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Estructura para modela el problemaa
typedef struct Grafo{
	int n;	//Cantidad de nodos(tareas)
	int m;	//Cantidad de aristas(conflictos)
	int **A; //Matriz de adyacencia
} Grafo;

//Estrcutura que representa una tarea con su cantidad de problemas
typedef struct Tarea{
    int indice;			//Entero con el valor del elemento
    int aristas;	//Cantidad de aristas que posee el vertice

} Tarea;

void merge(Tarea arr[], int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    Tarea L[n1], R[n2];
 
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
        if(L[i].aristas <= R[j].aristas) {
            arr[k] = L[i];
            i++;
        }
        else{
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
 

void mergeSort(Tarea arr[], int l, int r){
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

//Entrada: Cantidad de vertices del grafo y la cantidad de aristas que posee
//Salida: Un grafo de tamanio (vertices*vertices) llena de ceros
//Objetivo: Crear un grafo representado por una matriz de adyacencia lleno de ceros 
Grafo *crearGrafo(int vertices, int aristas){
	//Se crea el puntero
	Grafo *g;
	
	//Se crea el grafo y se asigna la memoria correspondiente
	g = (Grafo *)malloc(sizeof(Grafo));
	g->n = vertices;	//Tareas
	g->m = aristas;		//Conflictos
	//Se asigna memoria para la primera dimension de la matriz
	g->A = (int **)malloc(sizeof(int *)*vertices);
	//Se recorre cada fila
	for(int i = 0; i < vertices ; i++){
		//Se asigna memoria para la segunda dimension de la matriz
		g->A[i] = (int *)malloc(sizeof(int)*vertices);
		//Se recorre cada columna llenando con ceros
		for(int j = 0; j < vertices; j++){
			g->A[i][j] = 0;
		}
	}
	//Se retorna el puntero al grafo
	return  g;
}

//Entrada: Nombre del archivo de entrada
//Salida: Puntero a un grafo que contiene la informacion del problema
//Objetivo: Leer el archivo de entrada y almacenar la informacion contenida en el donde corresponda
Grafo *leerEntrada(char const *nombreArchivo){
	
	FILE *pf;
	//Se abre el archivo en modo lectura
	pf = fopen(nombreArchivo, "r");

	//En caso de que el archivo de entrada no exista
	if (pf == NULL){
		return NULL;
	}
	
	int tareas, conflictos, fila,columna;
	char aux[2];
	
	//Se leen los datos de la primer linea del archivo
	fscanf(pf, "%s", aux);
	fscanf(pf, "%d", &tareas);
	fscanf(pf, "%d", &conflictos);
	
	Grafo *G = crearGrafo(tareas, conflictos);
	
	for(int i=0; i < conflictos; i++){
		//Se guarda el identificador en un auxiliar
		fscanf(pf, "%s", aux);
		fscanf(pf, "%d", &fila);
		fscanf(pf, "%d", &columna);
		//Se guarda la arista en la matriz
		G->A[fila-1][columna-1] = 1;
		G->A[columna-1][fila-1] = 1;
	}
	
	//Se cierra el archivo de entrada
	fclose(pf);
	free(aux);
	//Se retorna el puntero al grafo
	return G;
}

//Entrada: Un puntero al grafo que contiene la informacion del problema
//Salida: Una lista de tareas con la cantidad de aristas que poseen(problemas)
//Objetivo: Contar la cantidad de conflictos que tiene cada tarea 
Tarea *contarConflictos(Grafo *G){
	Tarea *listaTareas = (Tarea*)malloc(sizeof(Tarea)*(G->n));
	//Se recorre cada fila de la matriz de adyacencia
	for(int i = 0; i < G->n; i++){
		//Se crea la estructura para almacenar la tarea y la cantidad de conflictos
		Tarea nuevaTarea;
		nuevaTarea.indice = i + 1;
		int conflictos = 0;
		//Se recorre la fila
		for (int j = 0; j < G->n; j++){
			conflictos = conflictos + G->A[i][j];
		}
		nuevaTarea.aristas = conflictos;
		listaTareas[i] = nuevaTarea;
	}
	//Se retorna el arreglo
	return listaTareas;
}

//Entrada: Grafo que contiene los datos del problema, arreglo que contiene la semana en la que se asigno cada tarea, el indice de la que se desea comprobar y la semana a revisar
//Salida: Entero con valor 1 si se puede asignar la tarea en la semana indicada o con valor 0 si no se puede asignar en la semana indicada
//Objetivo: Comprobar si la tarea indicada puede ser asignada en cierta semana
int puedoPonerTarea(Grafo *G, int *tareasEnSemanas, int indiceTarea, int semana){
	//Se recorre el arreglo
	for(int i = 0; i < G->n; i++){
		if(tareasEnSemanas[i] == semana && G->A[indiceTarea-1][i]){
			//No se puede poner en la semana indicada
			return 0;
		}
	}
	//Si se puede poner en la semana indicada
	return 1;
}

//Entrada: Entero que representa el largo del arreglo a crearArregloCero
//Salida: Arreglo de tipo entero que solo contiene 0's
//Objetivo: Crear un arreglo de tipo entero del largo indicado conteniendo solo 0's
int *crearArregloCero(int largo){
	//Se asigna memoria para el arreglo
	int *array = malloc(largo * sizeof(int));
	//Se recorre el arreglo llenandolo con ceros
	for(int i = 0; i < largo; i++){
		array[i] = 0;
	}
	//Se retorna el arreglo
	return array;
}

	
int main(int argc, char const *argv[]){
	int cantidadTareas, cantidadProblemas;
	//Se define las variables para tomar el tiempo de ejecucion
	clock_t tiempo_inicio, tiempo_final;
	//Se toma el tiempo de inicio
	tiempo_inicio = clock();
	//Se lee el archivo de entrada
	Grafo *G = leerEntrada(argv[1]);
	//Se comprueba que no ocurrieran errores al leer el archivo de entrada
	if(G == NULL){
		printf("No existe el archivo de entrada.\n");
		return 0;
	}
	
	//Se cuentan los conflictos que posee cada tarea
	Tarea *listaTareas = contarConflictos(G);
	//Se ordenan las tareas de manera creciente con respecto a la cantidad de conflictos
	mergeSort(listaTareas, 0, (G->n) - 1);
	//Se crea un arreglo para almacenar la semana en que se asigno cada tarea 
	int *tareasEnSemanas = crearArregloCero(G->n);
	//Variable que llevara la cuenta de la semana mayor en la que se asigno una tarea
	int semanasTotales = 1;
	
	//Ciclo para asignar una semana a cada tarea
	for(int t = 0; t < G->n; t++){
		//Semana en la que se compenzara a comprobar si se puede asignar
		int semanaActual = 1;
		//Se almacena el identificador de la tarea actual
		int tareaActual = listaTareas[t].indice;
		
		//Si la tarea se puede poner en la primera semana, se asigna en esta semana
		if(puedoPonerTarea(G, tareasEnSemanas, tareaActual, semanaActual)){
			//Se almacena la semana asignada a la tarea en el arreglo que contiene esta informacion
			tareasEnSemanas[tareaActual-1] = semanaActual;
		}
		
		//Si no se puede asignar en la primera semana
		else{
			//Se comprueba si se puede asignar en la semana siguiente 
			semanaActual = semanaActual + 1;
			//Ciclo para comprobar si la tarea puede ser asignada en alguna semana ya en uso
			while(semanaActual <= semanasTotales){
				//Si se puede asignar en la semana actual
				if(puedoPonerTarea(G, tareasEnSemanas, tareaActual, semanaActual)){
					//Se guarda la informacion de la semana en la que se asigno
					tareasEnSemanas[tareaActual-1] = semanaActual;
					//Se aumenta el valor de la semana actual para salir del ciclo
					semanaActual = semanasTotales + 1;
				}
				//Si no se pude asignar en la semana actual, se comprueba en la siguiente
				else{
					semanaActual = semanaActual + 1;
				}
			}
			//Si no se pudo asigna en ninguna semana de las que ya tiene asignada al menos una tarea, se asigna en una semana nueva
			if(semanaActual > semanasTotales && tareasEnSemanas[tareaActual-1] == 0){
				//Se almacena la informacion de la semana en la que se asigno
				tareasEnSemanas[tareaActual-1] = semanaActual;
				//Se aumenta en 1 la cantidad de semanas con tareas asignadas
				semanasTotales = semanasTotales + 1;
			}
		}
	}
	
	//Se toma el tiempo al termino de la ejecucion
	tiempo_final = clock();
	//Se calcula la cantidad de segundos 
	double segundos = (tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; 
	printf("Semanas totales: %d\n", semanasTotales);
	printf("Tiempo de ejecucion: %f\n",segundos); 
	
	return 0;
}
