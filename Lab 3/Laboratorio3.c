#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Estructura de datos que representa un vertice del arbol de desiciones
typedef struct nodo{
	int nivel;	//Nivel del vertice, ademas sirve como indice de inicio para el calculo de la cota
	float valor;	//Suma de los valores que componen la solucion parcial
	float limite;	//Resto del limite original del problema
	float cota;		//Suma de valores maxima que se puede obtener a partir de esta ramificacion
	struct nodo *sig;	//Puntero al siguiente nodo dentro de una lista simplemente enlazada
} nodo;

//Estructura de una lista simplemente enlazada que contendra los nodos del arbol de desiciones
typedef struct listaNodo{
	nodo* head;		//Puntero al primer nodo de la lista
} listaNodo;


//Entrada: Nivel del nodo a crear, su valor, el resto del limite de ponderacion y su cota
//Salida: Nodo que almacena los datos ingresados
//Objetivo: Crea una estructura nodo con los datos ingresados
nodo* crearNodo(int nivel, float valor, float limite, float cota){
	nodo *aux;
	//Se reserva memoria para el nodo
	aux = (nodo*)malloc(sizeof(nodo));
	
	//Se copian los datos
	aux->nivel = nivel;
	aux->valor = valor;
	aux->limite = limite;
	aux->cota = cota;
	aux->sig = NULL;

	//Se retorna el nodo creado
	return aux;
}

//Entrada: Lista simplemente enlazada y el nodo que se desea agregar
//Salida: Nada, solamente se actualiza la lista ingresada
//Objetivo: Agrega un nodo al final de una lista simplemente enlazada
void agregarFinalNodo(listaNodo *L, nodo *nodoAgregar){

	//En caso de que la lista este vacia
	if(L->head == NULL){
		L->head = nodoAgregar;
		return ;
	}

	nodo *aux = L->head;
	//Caso general
	while(aux->sig != NULL){
		aux = aux->sig;
	}

	aux->sig = nodoAgregar;

	return;
}

//Entrada: Lista simplemente enlazada
//Salida: Entero que representa el indice del nodo que posee la mayor cota dentro de la lista
//Objetivo: Obtener el indice dentro de la lista simplemente enlazada del nodo que posee la mayor cota
int obtenerIndiceMayor(listaNodo *L){
	//Caso donde solo haya un elemento en la lista
	if(L->head->sig == NULL){
		return 1;
	}

	nodo *aux = L->head->sig;
	//Variable para recorrer la lista
	int i = 2;
	//Variable que indica el indice del mayor elemento de la lista
	int indiceMayor = 1;
	//Se establece el primer elemento como el mayor para ser utilizado como pivote
	float cotaMayor = L->head->cota;
	
	//Se recorre la lista mientras no se llegue al final de esta
	while(aux != NULL){
		//Se registra la informacion del nuevo elemento mayor si este tiene una mayor puntuacion
		if(aux->cota > cotaMayor){
			indiceMayor = i;
			cotaMayor = aux->cota;
		}
		
		//Se avanza al siguiente elemento de la lista
		aux = aux->sig;
		//Se actualiza el indice
		i +=1;
	}
	
	//Se retorna el mayor indice encontrado
	return indiceMayor;
}

//Entrada: Lista simplemente enlazada y entero que representa el indice dentro de la lista que tiene el nodo a extraer
//Salida: Nodo con el indice indicado
//Objetivo: Elimina el nodo con el indice indicado dentro de la lista y lo retorna
nodo* extrarNodo(listaNodo *L, int indiceExtraer){
	//Si la lista esta vacia
	if(L->head == NULL){
		return NULL;
	}
	
	//Si se desea extaer el primer elemento de la lista
	else if(indiceExtraer == 1){
		nodo *nodoExtraido = L->head;
		L->head = nodoExtraido->sig;
		nodoExtraido->sig = NULL;
		return nodoExtraido;
	}
	
	//Caso general
	else{
		nodo *aux = L->head;
		int i = 2;
		while(aux != NULL){
			if(i == indiceExtraer){
				nodo *nodoExtraido = aux->sig;
				aux->sig = nodoExtraido->sig;
				nodoExtraido->sig = NULL;
				return nodoExtraido;
			}
			//Se actualiza el contador y el nodo a comprobar
			i++;
			aux = aux->sig;
		}
	}
}

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
	//Se copia el nombre del archivo
	char nombre[25];
	strcpy(nombre, nombreArchivo);
	//Variables para almacenal los datos
	char auxChar[6];
	char otroAuxChar[6];
	char *token;
	//Separadores
	const char separador[2] = "_";
	const char otroSeparador[2] = ".";
	
	token = strtok(nombre, separador);
	for(int i=0; i<4; i++) {
      if(i == 2){
		  int n = atoi(token);
		  *cantidadElementos = n;
	  }
	  
	  if(i == 3){
		  token = strtok(token, otroSeparador);
		  int l = atoi(token);
		  *limiteSuma = l;
	  }
    
      token = strtok(NULL, separador);
   }
   
   return;
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

//Entrada: Cantidad de elementos que posee la lista de candidatos, el limite de ponderacion, la lista de candidatos y el indice desde donde se comienzan a considerar los candidatos
//Salida: Valor float que representa la cota del nodo
//Objetivo: Obtener la cota del nodo, que representa el mayor beneficio que se puede obtener de la ramificacion 
float calcularCota (int cantElem, int limitPondera, Elemento *listaElementos, int indiceComienzo){
	//Si no hay elementos para considerar
	if(indiceComienzo == cantElem){
		return 0;
	}
	
	float valorTotal = 0; 
	float ponderacionTotal = 0;
	int i = indiceComienzo;
	
	//Ciclo para agregar elementos a la solucion
	while(i < cantElem && (ponderacionTotal + listaElementos[i].ponderacion) <= limitPondera){
		valorTotal = valorTotal + listaElementos[i].valor;
		ponderacionTotal = ponderacionTotal + listaElementos[i].ponderacion;
		i = i + 1;	
	}
	//Se comprueba si sobro ponderacion
	int resto = limitPondera - ponderacionTotal;
	
	//Si sobro ponderacion, se agrega una fraccion del elemento para ultilizar toda la ponderacion
	if(ponderacionTotal < limitPondera){
		valorTotal = valorTotal + (listaElementos[i].valor_peso * (resto) );
	}
	//Se retorna la suma total de valores
	return valorTotal;
}


//Entrada: Cantidad de elementos que posee la lista de candidatos, limite de ponderacion del problema original, lista de candidatos y direccion de la variable que almacena la mayor solucion encontrada
//Salida: Valor de la mayor suma de valores encontrada para el problema
//Objetivo: Obtener la mayor suma de valores que no superen la ponderacion limite
float calcularMayorSumaValores(int cantidadElementos, int limitePonderacion, Elemento *listaElementos, int* mayorSolucion){
	//Se crea la lista de nodos del arbol de desiciones
	listaNodo* listaNodosArbol;
	//Se crea la raiz
	nodo *aux = crearNodo(0, 0, limitePonderacion, 0);
	//Se agrega a la lista de nodos
	agregarFinalNodo(listaNodosArbol, aux);
	
	do{
		//Se obtiene el indice del nodo con mayor cota
		int indiceNodoMayorCota = obtenerIndiceMayor(listaNodosArbol);
		//Se extrae el nodo de la lista
		nodo *nodoMayorCota = extrarNodo(listaNodosArbol, indiceNodoMayorCota);
		
		//Se comprueba si el nodo puede seguir ramificandose
		if (nodoMayorCota->limite > 0 && nodoMayorCota->nivel < cantidadElementos ){
			float nuevoLimite, nuevaCota;
			//Comprobamos si podemos generar al hijo izquierdo(Agregamos el nuevo elemento)
			nuevoLimite = nodoMayorCota->limite - listaElementos[nodoMayorCota->nivel].ponderacion;
			
			if(nuevoLimite >= 0){
				//Se calcula la cota del hijo izquierdo
				nuevaCota = nodoMayorCota->valor + listaElementos[nodoMayorCota->nivel].valor + calcularCota(cantidadElementos, nuevoLimite, listaElementos, nodoMayorCota->nivel+1);
				//Si la cota del nuevo nodo es mayor a la mayor solucion encontrada, se genera el nuevo nodo
				if(nuevaCota > *mayorSolucion){
					//Se agrega el valor del nuevo elemento al valor anterior
					int nuevoValor = nodoMayorCota->valor + listaElementos[nodoMayorCota->nivel].valor;
					//Se comprueba si el valor obtenido es el optimo del problema
					if(nuevoValor == nuevaCota){
						return nuevoValor;
					}
					//Se comprueba si la nueva solucion parcial es la mejor hasta el momento
					if(nuevoValor > *mayorSolucion){
						*mayorSolucion = nuevoValor;
					}
					
					//Se crea el nodo izquierdo
					nodo *nuevoNodo = crearNodo(nodoMayorCota->nivel+1, nuevoValor, nuevoLimite, nuevaCota);
					//Se agrega a la lista de nodos
					agregarFinalNodo(listaNodosArbol, nuevoNodo);					
				}
			}
			
			//Comprobamos si podemos generar al hijo derecho(no agregamos el nuevo elemento)
			nuevaCota = nodoMayorCota->valor + calcularCota(cantidadElementos, nodoMayorCota->limite, listaElementos, nodoMayorCota->nivel+1);
			//Si la cota del nuevo nodo es mayor a la mayor solucion encontrada, se genera el nuevo nodo
			if(nuevaCota > *mayorSolucion){
				//Se crea el nodo
				nodo* nuevoNodo = crearNodo(nodoMayorCota->nivel+1, nodoMayorCota->valor, nodoMayorCota->limite, nuevaCota);
				//Se agrega a la lista de nodos
				agregarFinalNodo(listaNodosArbol, nuevoNodo);
				
			}
		}
		//Se libera la memoria utilizada por el nodo extraido
		free(nodoMayorCota);
		//Se itera hasta que se hayan comprobado todos los nodos
	} while(listaNodosArbol->head != NULL);
	//Se retorna la mayor solucion encontrada
	return *mayorSolucion;	
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
	
	//Se ordena la lista de mayor a menor segun su valor_peso para el greedy relajado
	mergeSort(listaElementos, 0, cantidadElementos-1);
	int mayorSolucion = 0;
	//Se calcula la mayor suma de valores
	float resultado = calcularMayorSumaValores(cantidadElementos, limitePonderacion, listaElementos, &mayorSolucion);
	//Se toma el tiempo al termino de la ejecucion
	tiempo_final = clock();
	//Se calcula la cantidad de segundos 
	double segundos = (tiempo_final - tiempo_inicio) / CLOCKS_PER_SEC; 
	//Se muestra la mayot suma encontrada y el tiempo de ejecucion
	printf("La mayor suma de valores encontrada es: %d\n", (int)resultado);
	printf("Tiempo de ejecucion: %f\n",segundos); 
	
	return 0;
}