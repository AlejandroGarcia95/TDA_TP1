#include <stdio.h>
#include <stdlib.h>
#include "heap.c"
#include "estadisticoOrdenK.h"

#define TAM 20


/*******************************************************************
 *             		FUNCIONES AUXILIARES
 ******************************************************************/


/* Recibe un vector de enteros, el largo n del vector, un valor que es
 * el índice del candidato a ser el k elemento más pequeño y el valor k.
 * Devuelve un booleano indicando si el candidato es el k más pequeño. */
bool verificador(int vector[], int n, int candidato, int k){

	int i, contador = 0;

	for(i = 0; i < n; i++){

		if (vector[i] < vector[candidato])
			contador++;

		if ((vector[i] == vector[candidato]) && (i < candidato))
			contador++;
	}

	return (contador == k);
}


/* Intercambia dos valores enteros. */
void swap (int* x, int* y){

	int auxiliar = *x;
	*x = *y;
	*y = auxiliar;
}


/* Dados dos vectores ordenados, junto con la cantidad de elementos de cada uno, 
 * ordena el vector de la entrada a partir de la intercalación de ambos vectores. */
void intercalar_ordenado(int *vector, int *prim, int largoPrim, int *seg, int largoSeg){
		
	int i = 0, j = 0, pos = 0;
	
	while ((i < largoPrim) && (j < largoSeg)){
		if (prim[i] < seg[j]){
			vector[pos] = prim[i];
			pos++;
			i++;
		}
		
		else {
			vector[pos] = seg[j];
			pos++;
			j++;
		}
	}

	while (i < largoPrim) {		
		vector[pos] = prim[i];
		i++;
		pos++;
	}

	while (j < largoSeg) {
		vector[pos] = seg[j];
		j++;
		pos++;
	}

}


/* Función de ordenamiento MergeSort. */
void merge_sort(int *vector, int ultimo){

	int i, j, medio;

	if (ultimo < 2)
		return;
	
	medio = ultimo/2;
	int *prim = (int*)malloc(medio * sizeof(int));
	int *seg = (int*)malloc((ultimo - medio) * sizeof(int));

	for(i = 0; i < medio; i++)
		prim[i] = vector[i];

	for(j = medio; j < ultimo; j++)
		seg[j - medio] = vector[j];

	merge_sort(prim, medio);
	merge_sort(seg, ultimo - medio);

	intercalar_ordenado(vector, prim, medio, seg, ultimo - medio);

	free(prim);
	free(seg);
	
}


/* Función auxiliar que compara dos números. Devuelve cero si son iguales, 1 si a es
 * menor que b, -1 en otro caso. */
int comparar_numeros_min(int *a, int *b){

	if (*a == *b)
		return 0;

	if (*a < *b)
		return 1;

	return -1;
}


/* Función auxiliar que compara dos números. Devuelve cero si son iguales, 1 si a es
 * mayor que b, -1 si b es mayor que a. */
int comparar_numeros_max(int *a, int *b){

	if (*a == *b)
		return 0;

	if (*a < *b)
		return -1;

	return 1;
}


/* Función partition de QuickSort. */
int partition(int vector[], int inicial, int ultimo){
	int j;
	int pivote = vector[ultimo];
	int i = inicial;

	for(j = inicial; j < ultimo; j++){
		if(vector[j] < pivote){
			
			swap(&vector[i], &vector[j]);
			i++;
		}
	}
	
	swap(&vector[i], &vector[ultimo]);
	return i;
}


/* Recibe un vector, su largo n, y un puntero a una posición.
 * Elimina los valores repetidos del vector. */
void eliminar_repetidos(int *vector, int n, int* pos){
	
	*pos = 0;
	hash_t *hash = hash_crear(NULL);
	
	for(int i = 0; i < n; i++){
		
		char str[TAM];
  		sprintf(str, "%d", vector[i]);
		
		if (! hash_pertenece(hash, str)) {
			hash_guardar(hash, str, "1");
			vector[(*pos)] = vector[i];
			(*pos)++;
		}
	}

	hash_destruir(hash);
}


/*******************************************************************
 *         ALGORITMOS QUE OBTIENEN EL ESTADÍSTICO DE ORDEN K
 ******************************************************************/


/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Itera todos los elementos del conjunto y verifica de a uno si es la solución. 
 * Cuendo el verificador devuelve true, devuelve ese elemento.*/
int fuerza_bruta(int vector[], int n, int k, bool sin_repetidos){

	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, n, pos);
		n = *pos;
		free(pos);
	}
	
	for(int i = 0; i < n; i++){
		if (verificador(vector, n, i, k))
			return vector[i];
	}
}


/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Ordena el conjunto mediante MergeSort y luego selecciona el k elemento del arreglo
 * ordenado. */
int ordenar_y_seleccionar(int vector[], int n, int k, bool sin_repetidos){

	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, n, pos);
		n = *pos;
		free(pos);
	}

	merge_sort(vector, n);
	return vector[k];
}


/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Se realizan k-selecciones para encontrar el k elemento más pequeño. */
int k_selecciones(int vector[], int n, int k, bool sin_repetidos){
	
	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, n, pos);
		n = *pos;
		free(pos);
	}

	int pos_minimo;

	for(int i = 0; i < k + 1; i++){
		pos_minimo = i;
		for(int j = i + 1; j < n; j++){
			if (vector[pos_minimo] > vector[j])
				pos_minimo = j;
		}

		swap(&vector[pos_minimo], &vector[i]);
	}

	return vector[k];
}


/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Realiza k-extracciones a un arreglo con la propiedad de heap. */
int k_heapsort(int vector[], int n, int k, bool sin_repetidos){

	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, n, pos);
		n = *pos;
		free(pos);
	}

	void *vector_aux[TAM];
	for(int i = 0; i < n; i++){
		vector_aux[i]=(void*)&vector[i];
	}

	heap_t* heap = (heap_t*)heapify_vector(vector_aux, n, (cmp_func_f) comparar_numeros_min);
	int *kEstadistico;

	for(int i = 0; i < k + 1; i++)
		kEstadistico = heap_desencolar(heap);	

	heap_destruir(heap, NULL);
	
	return (*kEstadistico);
	
}


/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Se usa un heap para almacenar los k elementos más chicos, intercambiándolos cuando
 * sea necesario. */
int heap_select(int vector[], int n, int k, bool sin_repetidos){

	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, n, pos);
		n = *pos;
		free(pos);
	}
	
	int i;
	heap_t* heap = heap_crear((cmp_func_f) comparar_numeros_max);

	for(i = 0; i < k + 1; i++)
		heap_encolar(heap, &vector[i]);
	
	for(; i < n; i++){
		int *max = heap_ver_max(heap);
		if(*max > vector[i]){
			heap_desencolar(heap);
			heap_encolar(heap, &vector[i]);
		}
	}
	
	int *estadisticoK = heap_desencolar(heap);
	heap_destruir(heap, NULL);

	return *estadisticoK;	
}


/* Recibe un vector, sus índices (primero y ultimo), el estadístico de orden k, y un 
 * booleano indicando si se consideran los elementos repetidos en el orden k.
 * Se utiliza una estrategia de división y conquista similar a la de quicksort pero 
 * descartando las divisiones que sabemos que no incluyen al k buscado. */
int quick_select(int vector[], int primero, int ultimo, int k, bool sin_repetidos){
	
	if (sin_repetidos){
		int *pos = malloc(sizeof(int));
		eliminar_repetidos(vector, ultimo + 1, pos);
		ultimo = (*pos) - 1;
		free(pos);
	}
	
	if (primero == ultimo)
		return vector[primero];

	int pos_pivote = partition(vector, primero, ultimo);

	if (pos_pivote == k)
		return vector[pos_pivote];

	else if (k < pos_pivote)
		return quick_select(vector, primero, pos_pivote - 1, k, false);

	return quick_select(vector, pos_pivote + 1, ultimo, k, false);
}


/*******************************************************************
 *             			PRUEBAS
 ******************************************************************/


/* Función auxiliar para imprimir si la prueba estuvo OK o no. */
void print_test(char* name, bool result)
{
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}


/* Función que realiza pruebas para el algoritmo de fuerza bruta. */
void prueba_fuerza_bruta(){
	printf("INICIO DE PRUEBAS DE FUERZA BRUTA\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};

	print_test("Vector 1, k = 0", fuerza_bruta(vector1, 16, 0, false) == 1);
	print_test("Vector 1, k = 1", fuerza_bruta(vector1, 16, 1, false) == 2);
	print_test("Vector 1, k = 2", fuerza_bruta(vector1, 16, 2, false) == 3);
	print_test("Vector 1, k = 3", fuerza_bruta(vector1, 16, 3, false) == 4);
	print_test("Vector 1, k = 4", fuerza_bruta(vector1, 16, 4, false) == 5);
	print_test("Vector 1, k = 5", fuerza_bruta(vector1, 16, 5, false) == 6);
	print_test("Vector 1, k = 6", fuerza_bruta(vector1, 16, 6, false) == 7);
	print_test("Vector 1, k = 7", fuerza_bruta(vector1, 16, 7, false) == 8);
	print_test("Vector 1, k = 8", fuerza_bruta(vector1, 16, 8, false) == 9);
	print_test("Vector 1, k = 9", fuerza_bruta(vector1, 16, 9, false) == 10);
	print_test("Vector 1, k = 10", fuerza_bruta(vector1, 16, 10, false) == 11);
	print_test("Vector 1, k = 11", fuerza_bruta(vector1, 16, 11, false) == 12);
	print_test("Vector 1, k = 12", fuerza_bruta(vector1, 16, 12, false) == 13);
	print_test("Vector 1, k = 13", fuerza_bruta(vector1, 16, 13, false) == 14);
	print_test("Vector 1, k = 14", fuerza_bruta(vector1, 16, 14, false) == 15);
	print_test("Vector 1, k = 15", fuerza_bruta(vector1, 16, 15, false) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", fuerza_bruta(vector2, 16, 0, false) == -43);
	print_test("Vector 2, k = 1", fuerza_bruta(vector2, 16, 1, false) == -7);
	print_test("Vector 2, k = 2", fuerza_bruta(vector2, 16, 2, false) == -6);
	print_test("Vector 2, k = 3", fuerza_bruta(vector2, 16, 3, false) == 0);
	print_test("Vector 2, k = 4", fuerza_bruta(vector2, 16, 4, false) == 4);
	print_test("Vector 2, k = 5", fuerza_bruta(vector2, 16, 5, false) == 6);
	print_test("Vector 2, k = 6", fuerza_bruta(vector2, 16, 6, false) == 7);
	print_test("Vector 2, k = 7", fuerza_bruta(vector2, 16, 7, false) == 8);
	print_test("Vector 2, k = 8", fuerza_bruta(vector2, 16, 8, false) == 9);
	print_test("Vector 2, k = 9", fuerza_bruta(vector2, 16, 9, false) == 10);
	print_test("Vector 2, k = 10", fuerza_bruta(vector2, 16, 10, false) == 17);
	print_test("Vector 2, k = 11", fuerza_bruta(vector2, 16, 11, false) == 20);
	print_test("Vector 2, k = 12", fuerza_bruta(vector2, 16, 12, false) == 35);
	print_test("Vector 2, k = 13", fuerza_bruta(vector2, 16, 13, false) == 40);
	print_test("Vector 2, k = 14", fuerza_bruta(vector2, 16, 14, false) == 45);
	print_test("Vector 2, k = 15", fuerza_bruta(vector2, 16, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", fuerza_bruta(vector3, 16, 0, false) == -46);
	print_test("Vector 3, k = 1", fuerza_bruta(vector3, 16, 1, false) == -45);
	print_test("Vector 3, k = 2", fuerza_bruta(vector3, 16, 2, false) == -9);
	print_test("Vector 3, k = 3", fuerza_bruta(vector3, 16, 3, false) == 4);
	print_test("Vector 3, k = 4", fuerza_bruta(vector3, 16, 4, false) == 12);
	print_test("Vector 3, k = 5", fuerza_bruta(vector3, 16, 5, false) == 17);
	print_test("Vector 3, k = 6", fuerza_bruta(vector3, 16, 6, false) == 19);
	print_test("Vector 3, k = 7", fuerza_bruta(vector3, 16, 7, false) == 22);
	print_test("Vector 3, k = 8", fuerza_bruta(vector3, 16, 8, false) == 27);
	print_test("Vector 3, k = 9", fuerza_bruta(vector3, 16, 9, false) == 37);
	print_test("Vector 3, k = 10", fuerza_bruta(vector3, 16, 10, false) == 39);
	print_test("Vector 3, k = 11", fuerza_bruta(vector3, 16, 11, false) == 49);
	print_test("Vector 3, k = 12", fuerza_bruta(vector3, 16, 12, false) == 60);
	print_test("Vector 3, k = 13", fuerza_bruta(vector3, 16, 13, false) == 65);
	print_test("Vector 3, k = 14", fuerza_bruta(vector3, 16, 14, false) == 70);
	print_test("Vector 3, k = 15", fuerza_bruta(vector3, 16, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", fuerza_bruta(vector4, 16, 0, false) == -46);
	print_test("Vector 4, k = 1", fuerza_bruta(vector4, 16, 1, false) == -45);
	print_test("Vector 4, k = 2", fuerza_bruta(vector4, 16, 2, false) == -9);
	print_test("Vector 4, k = 3", fuerza_bruta(vector4, 16, 3, false) == 4);
	print_test("Vector 4, k = 4", fuerza_bruta(vector4, 16, 4, false) == 12);
	print_test("Vector 4, k = 5", fuerza_bruta(vector4, 16, 5, false) == 17);
	print_test("Vector 4, k = 6", fuerza_bruta(vector4, 16, 6, false) == 19);
	print_test("Vector 4, k = 7", fuerza_bruta(vector4, 16, 7, false) == 22);
	print_test("Vector 4, k = 8", fuerza_bruta(vector4, 16, 8, false) == 27);
	print_test("Vector 4, k = 9", fuerza_bruta(vector4, 16, 9, false) == 37);
	print_test("Vector 4, k = 10", fuerza_bruta(vector4, 16, 10, false) == 39);
	print_test("Vector 4, k = 11", fuerza_bruta(vector4, 16, 11, false) == 49);
	print_test("Vector 4, k = 12", fuerza_bruta(vector4, 16, 12, false) == 60);
	print_test("Vector 4, k = 13", fuerza_bruta(vector4, 16, 13, false) == 65);
	print_test("Vector 4, k = 14", fuerza_bruta(vector4, 16, 14, false) == 70);
	print_test("Vector 4, k = 15", fuerza_bruta(vector4, 16, 15, false) == 143);


	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", fuerza_bruta(vector5, 16, 0, false) == 1);
	print_test("Vector 5, k = 1", fuerza_bruta(vector5, 16, 1, false) == 2);
	print_test("Vector 5, k = 2", fuerza_bruta(vector5, 16, 2, false) == 2);
	print_test("Vector 5, k = 3", fuerza_bruta(vector5, 16, 3, false) == 2);
	print_test("Vector 5, k = 4", fuerza_bruta(vector5, 16, 4, false) == 3);
	print_test("Vector 5, k = 5", fuerza_bruta(vector5, 16, 5, false) == 4);
	print_test("Vector 5, k = 6", fuerza_bruta(vector5, 16, 6, false) == 5);
	print_test("Vector 5, k = 7", fuerza_bruta(vector5, 16, 7, false) == 6);
	print_test("Vector 5, k = 8", fuerza_bruta(vector5, 16, 8, false) == 7);
	print_test("Vector 5, k = 9", fuerza_bruta(vector5, 16, 9, false) == 8);
	print_test("Vector 5, k = 10", fuerza_bruta(vector5, 16, 10, false) == 9);
	print_test("Vector 5, k = 11", fuerza_bruta(vector5, 16, 11, false) == 11);
	print_test("Vector 5, k = 12", fuerza_bruta(vector5, 16, 12, false) == 12);
	print_test("Vector 5, k = 13", fuerza_bruta(vector5, 16, 13, false) == 13);
	print_test("Vector 5, k = 14", fuerza_bruta(vector5, 16, 14, false) == 15);
	print_test("Vector 5, k = 15", fuerza_bruta(vector5, 16, 15, false) == 15);


	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", fuerza_bruta(vector6, 16, 0, true) == -11);
	print_test("Vector 6, k = 1", fuerza_bruta(vector6, 16, 1, true) == -8);
	print_test("Vector 6, k = 2", fuerza_bruta(vector6, 16, 2, true) == 1);
	print_test("Vector 6, k = 3", fuerza_bruta(vector6, 16, 3, true) == 2);
	print_test("Vector 6, k = 4", fuerza_bruta(vector6, 16, 4, true) == 3);
	print_test("Vector 6, k = 5", fuerza_bruta(vector6, 16, 5, true) == 4);
	print_test("Vector 6, k = 6", fuerza_bruta(vector6, 16, 6, true) == 5);
	print_test("Vector 6, k = 7", fuerza_bruta(vector6, 16, 7, true) == 6);
	print_test("Vector 6, k = 8", fuerza_bruta(vector6, 16, 8, true) == 7);
	print_test("Vector 6, k = 9", fuerza_bruta(vector6, 16, 9, true) == 9);
	print_test("Vector 6, k = 10", fuerza_bruta(vector6, 16, 10, true) == 12);
	print_test("Vector 6, k = 11", fuerza_bruta(vector6, 16, 11, true) == 13);
	print_test("Vector 6, k = 12", fuerza_bruta(vector6, 16, 12, true) == 15);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de ordenar y seleccionar. */
void prueba_ordenar_y_seleccionar(){
	printf("INICIO DE PRUEBAS DE ORDENAR Y SELECCIONAR\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};

	print_test("Vector 1, k = 0", ordenar_y_seleccionar(vector1, 16, 0, false) == 1);
	print_test("Vector 1, k = 1", ordenar_y_seleccionar(vector1, 16, 1, false) == 2);
	print_test("Vector 1, k = 2", ordenar_y_seleccionar(vector1, 16, 2, false) == 3);
	print_test("Vector 1, k = 3", ordenar_y_seleccionar(vector1, 16, 3, false) == 4);
	print_test("Vector 1, k = 4", ordenar_y_seleccionar(vector1, 16, 4, false) == 5);
	print_test("Vector 1, k = 5", ordenar_y_seleccionar(vector1, 16, 5, false) == 6);
	print_test("Vector 1, k = 6", ordenar_y_seleccionar(vector1, 16, 6, false) == 7);
	print_test("Vector 1, k = 7", ordenar_y_seleccionar(vector1, 16, 7, false) == 8);
	print_test("Vector 1, k = 8", ordenar_y_seleccionar(vector1, 16, 8, false) == 9);
	print_test("Vector 1, k = 9", ordenar_y_seleccionar(vector1, 16, 9, false) == 10);
	print_test("Vector 1, k = 10", ordenar_y_seleccionar(vector1, 16, 10, false) == 11);
	print_test("Vector 1, k = 11", ordenar_y_seleccionar(vector1, 16, 11, false) == 12);
	print_test("Vector 1, k = 12", ordenar_y_seleccionar(vector1, 16, 12, false) == 13);
	print_test("Vector 1, k = 13", ordenar_y_seleccionar(vector1, 16, 13, false) == 14);
	print_test("Vector 1, k = 14", ordenar_y_seleccionar(vector1, 16, 14, false) == 15);
	print_test("Vector 1, k = 15", ordenar_y_seleccionar(vector1, 16, 15, false) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", ordenar_y_seleccionar(vector2, 16, 0, false) == -43);
	print_test("Vector 2, k = 1", ordenar_y_seleccionar(vector2, 16, 1, false) == -7);
	print_test("Vector 2, k = 2", ordenar_y_seleccionar(vector2, 16, 2, false) == -6);
	print_test("Vector 2, k = 3", ordenar_y_seleccionar(vector2, 16, 3, false) == 0);
	print_test("Vector 2, k = 4", ordenar_y_seleccionar(vector2, 16, 4, false) == 4);
	print_test("Vector 2, k = 5", ordenar_y_seleccionar(vector2, 16, 5, false) == 6);
	print_test("Vector 2, k = 6", ordenar_y_seleccionar(vector2, 16, 6, false) == 7);
	print_test("Vector 2, k = 7", ordenar_y_seleccionar(vector2, 16, 7, false) == 8);
	print_test("Vector 2, k = 8", ordenar_y_seleccionar(vector2, 16, 8, false) == 9);
	print_test("Vector 2, k = 9", ordenar_y_seleccionar(vector2, 16, 9, false) == 10);
	print_test("Vector 2, k = 10", ordenar_y_seleccionar(vector2, 16, 10, false) == 17);
	print_test("Vector 2, k = 11", ordenar_y_seleccionar(vector2, 16, 11, false) == 20);
	print_test("Vector 2, k = 12", ordenar_y_seleccionar(vector2, 16, 12, false) == 35);
	print_test("Vector 2, k = 13", ordenar_y_seleccionar(vector2, 16, 13, false) == 40);
	print_test("Vector 2, k = 14", ordenar_y_seleccionar(vector2, 16, 14, false) == 45);
	print_test("Vector 2, k = 15", ordenar_y_seleccionar(vector2, 16, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", ordenar_y_seleccionar(vector3, 16, 0, false) == -46);
	print_test("Vector 3, k = 1", ordenar_y_seleccionar(vector3, 16, 1, false) == -45);
	print_test("Vector 3, k = 2", ordenar_y_seleccionar(vector3, 16, 2, false) == -9);
	print_test("Vector 3, k = 3", ordenar_y_seleccionar(vector3, 16, 3, false) == 4);
	print_test("Vector 3, k = 4", ordenar_y_seleccionar(vector3, 16, 4, false) == 12);
	print_test("Vector 3, k = 5", ordenar_y_seleccionar(vector3, 16, 5, false) == 17);
	print_test("Vector 3, k = 6", ordenar_y_seleccionar(vector3, 16, 6, false) == 19);
	print_test("Vector 3, k = 7", ordenar_y_seleccionar(vector3, 16, 7, false) == 22);
	print_test("Vector 3, k = 8", ordenar_y_seleccionar(vector3, 16, 8, false) == 27);
	print_test("Vector 3, k = 9", ordenar_y_seleccionar(vector3, 16, 9, false) == 37);
	print_test("Vector 3, k = 10", ordenar_y_seleccionar(vector3, 16, 10, false) == 39);
	print_test("Vector 3, k = 11", ordenar_y_seleccionar(vector3, 16, 11, false) == 49);
	print_test("Vector 3, k = 12", ordenar_y_seleccionar(vector3, 16, 12, false) == 60);
	print_test("Vector 3, k = 13", ordenar_y_seleccionar(vector3, 16, 13, false) == 65);
	print_test("Vector 3, k = 14", ordenar_y_seleccionar(vector3, 16, 14, false) == 70);
	print_test("Vector 3, k = 15", ordenar_y_seleccionar(vector3, 16, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", ordenar_y_seleccionar(vector4, 16, 0, false) == -46);
	print_test("Vector 4, k = 1", ordenar_y_seleccionar(vector4, 16, 1, false) == -45);
	print_test("Vector 4, k = 2", ordenar_y_seleccionar(vector4, 16, 2, false) == -9);
	print_test("Vector 4, k = 3", ordenar_y_seleccionar(vector4, 16, 3, false) == 4);
	print_test("Vector 4, k = 4", ordenar_y_seleccionar(vector4, 16, 4, false) == 12);
	print_test("Vector 4, k = 5", ordenar_y_seleccionar(vector4, 16, 5, false) == 17);
	print_test("Vector 4, k = 6", ordenar_y_seleccionar(vector4, 16, 6, false) == 19);
	print_test("Vector 4, k = 7", ordenar_y_seleccionar(vector4, 16, 7, false) == 22);
	print_test("Vector 4, k = 8", ordenar_y_seleccionar(vector4, 16, 8, false) == 27);
	print_test("Vector 4, k = 9", ordenar_y_seleccionar(vector4, 16, 9, false) == 37);
	print_test("Vector 4, k = 10", ordenar_y_seleccionar(vector4, 16, 10, false) == 39);
	print_test("Vector 4, k = 11", ordenar_y_seleccionar(vector4, 16, 11, false) == 49);
	print_test("Vector 4, k = 12", ordenar_y_seleccionar(vector4, 16, 12, false) == 60);
	print_test("Vector 4, k = 13", ordenar_y_seleccionar(vector4, 16, 13, false) == 65);
	print_test("Vector 4, k = 14", ordenar_y_seleccionar(vector4, 16, 14, false) == 70);
	print_test("Vector 4, k = 15", ordenar_y_seleccionar(vector4, 16, 15, false) == 143);

	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", ordenar_y_seleccionar(vector5, 16, 0, false) == 1);
	print_test("Vector 5, k = 1", ordenar_y_seleccionar(vector5, 16, 1, false) == 2);
	print_test("Vector 5, k = 2", ordenar_y_seleccionar(vector5, 16, 2, false) == 2);
	print_test("Vector 5, k = 3", ordenar_y_seleccionar(vector5, 16, 3, false) == 2);
	print_test("Vector 5, k = 4", ordenar_y_seleccionar(vector5, 16, 4, false) == 3);
	print_test("Vector 5, k = 5", ordenar_y_seleccionar(vector5, 16, 5, false) == 4);
	print_test("Vector 5, k = 6", ordenar_y_seleccionar(vector5, 16, 6, false) == 5);
	print_test("Vector 5, k = 7", ordenar_y_seleccionar(vector5, 16, 7, false) == 6);
	print_test("Vector 5, k = 8", ordenar_y_seleccionar(vector5, 16, 8, false) == 7);
	print_test("Vector 5, k = 9", ordenar_y_seleccionar(vector5, 16, 9, false) == 8);
	print_test("Vector 5, k = 10", ordenar_y_seleccionar(vector5, 16, 10, false) == 9);
	print_test("Vector 5, k = 11", ordenar_y_seleccionar(vector5, 16, 11, false) == 11);
	print_test("Vector 5, k = 12", ordenar_y_seleccionar(vector5, 16, 12, false) == 12);
	print_test("Vector 5, k = 13", ordenar_y_seleccionar(vector5, 16, 13, false) == 13);
	print_test("Vector 5, k = 14", ordenar_y_seleccionar(vector5, 16, 14, false) == 15);
	print_test("Vector 5, k = 15", ordenar_y_seleccionar(vector5, 16, 15, false) == 15);

	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", ordenar_y_seleccionar(vector6, 16, 0, true) == -11);
	print_test("Vector 6, k = 1", ordenar_y_seleccionar(vector6, 16, 1, true) == -8);
	print_test("Vector 6, k = 2", ordenar_y_seleccionar(vector6, 16, 2, true) == 1);
	print_test("Vector 6, k = 3", ordenar_y_seleccionar(vector6, 16, 3, true) == 2);
	print_test("Vector 6, k = 4", ordenar_y_seleccionar(vector6, 16, 4, true) == 3);
	print_test("Vector 6, k = 5", ordenar_y_seleccionar(vector6, 16, 5, true) == 4);
	print_test("Vector 6, k = 6", ordenar_y_seleccionar(vector6, 16, 6, true) == 5);
	print_test("Vector 6, k = 7", ordenar_y_seleccionar(vector6, 16, 7, true) == 6);
	print_test("Vector 6, k = 8", ordenar_y_seleccionar(vector6, 16, 8, true) == 7);
	print_test("Vector 6, k = 9", ordenar_y_seleccionar(vector6, 16, 9, true) == 9);
	print_test("Vector 6, k = 10", ordenar_y_seleccionar(vector6, 16, 10, true) == 12);
	print_test("Vector 6, k = 11", ordenar_y_seleccionar(vector6, 16, 11, true) == 13);
	print_test("Vector 6, k = 12", ordenar_y_seleccionar(vector6, 16, 12, true) == 15);


	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de k-selecciones. */
void prueba_k_selecciones(){
	printf("INICIO DE PRUEBAS DE K-SELECCIONES\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", k_selecciones(vector1, 16, 0, false) == 1);
	print_test("Vector 1, k = 1", k_selecciones(vector1, 16, 1, false) == 2);
	print_test("Vector 1, k = 2", k_selecciones(vector1, 16, 2, false) == 3);
	print_test("Vector 1, k = 3", k_selecciones(vector1, 16, 3, false) == 4);
	print_test("Vector 1, k = 4", k_selecciones(vector1, 16, 4, false) == 5);
	print_test("Vector 1, k = 5", k_selecciones(vector1, 16, 5, false) == 6);
	print_test("Vector 1, k = 6", k_selecciones(vector1, 16, 6, false) == 7);
	print_test("Vector 1, k = 7", k_selecciones(vector1, 16, 7, false) == 8);
	print_test("Vector 1, k = 8", k_selecciones(vector1, 16, 8, false) == 9);
	print_test("Vector 1, k = 9", k_selecciones(vector1, 16, 9, false) == 10);
	print_test("Vector 1, k = 10", k_selecciones(vector1, 16, 10, false) == 11);
	print_test("Vector 1, k = 11", k_selecciones(vector1, 16, 11, false) == 12);
	print_test("Vector 1, k = 12", k_selecciones(vector1, 16, 12, false) == 13);
	print_test("Vector 1, k = 13", k_selecciones(vector1, 16, 13, false) == 14);
	print_test("Vector 1, k = 14", k_selecciones(vector1, 16, 14, false) == 15);
	print_test("Vector 1, k = 15", k_selecciones(vector1, 16, 15, false) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", k_selecciones(vector2, 16, 0, false) == -43);
	print_test("Vector 2, k = 1", k_selecciones(vector2, 16, 1, false) == -7);
	print_test("Vector 2, k = 2", k_selecciones(vector2, 16, 2, false) == -6);
	print_test("Vector 2, k = 3", k_selecciones(vector2, 16, 3, false) == 0);
	print_test("Vector 2, k = 4", k_selecciones(vector2, 16, 4, false) == 4);
	print_test("Vector 2, k = 5", k_selecciones(vector2, 16, 5, false) == 6);
	print_test("Vector 2, k = 6", k_selecciones(vector2, 16, 6, false) == 7);
	print_test("Vector 2, k = 7", k_selecciones(vector2, 16, 7, false) == 8);
	print_test("Vector 2, k = 8", k_selecciones(vector2, 16, 8, false) == 9);
	print_test("Vector 2, k = 9", k_selecciones(vector2, 16, 9, false) == 10);
	print_test("Vector 2, k = 10", k_selecciones(vector2, 16, 10, false) == 17);
	print_test("Vector 2, k = 11", k_selecciones(vector2, 16, 11, false) == 20);
	print_test("Vector 2, k = 12", k_selecciones(vector2, 16, 12, false) == 35);
	print_test("Vector 2, k = 13", k_selecciones(vector2, 16, 13, false) == 40);
	print_test("Vector 2, k = 14", k_selecciones(vector2, 16, 14, false) == 45);
	print_test("Vector 2, k = 15", k_selecciones(vector2, 16, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", k_selecciones(vector3, 16, 0, false) == -46);
	print_test("Vector 3, k = 1", k_selecciones(vector3, 16, 1, false) == -45);
	print_test("Vector 3, k = 2", k_selecciones(vector3, 16, 2, false) == -9);
	print_test("Vector 3, k = 3", k_selecciones(vector3, 16, 3, false) == 4);
	print_test("Vector 3, k = 4", k_selecciones(vector3, 16, 4, false) == 12);
	print_test("Vector 3, k = 5", k_selecciones(vector3, 16, 5, false) == 17);
	print_test("Vector 3, k = 6", k_selecciones(vector3, 16, 6, false) == 19);
	print_test("Vector 3, k = 7", k_selecciones(vector3, 16, 7, false) == 22);
	print_test("Vector 3, k = 8", k_selecciones(vector3, 16, 8, false) == 27);
	print_test("Vector 3, k = 9", k_selecciones(vector3, 16, 9, false) == 37);
	print_test("Vector 3, k = 10", k_selecciones(vector3, 16, 10, false) == 39);
	print_test("Vector 3, k = 11", k_selecciones(vector3, 16, 11, false) == 49);
	print_test("Vector 3, k = 12", k_selecciones(vector3, 16, 12, false) == 60);
	print_test("Vector 3, k = 13", k_selecciones(vector3, 16, 13, false) == 65);
	print_test("Vector 3, k = 14", k_selecciones(vector3, 16, 14, false) == 70);
	print_test("Vector 3, k = 15", k_selecciones(vector3, 16, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", k_selecciones(vector4, 16, 0, false) == -46);
	print_test("Vector 4, k = 1", k_selecciones(vector4, 16, 1, false) == -45);
	print_test("Vector 4, k = 2", k_selecciones(vector4, 16, 2, false) == -9);
	print_test("Vector 4, k = 3", k_selecciones(vector4, 16, 3, false) == 4);
	print_test("Vector 4, k = 4", k_selecciones(vector4, 16, 4, false) == 12);
	print_test("Vector 4, k = 5", k_selecciones(vector4, 16, 5, false) == 17);
	print_test("Vector 4, k = 6", k_selecciones(vector4, 16, 6, false) == 19);
	print_test("Vector 4, k = 7", k_selecciones(vector4, 16, 7, false) == 22);
	print_test("Vector 4, k = 8", k_selecciones(vector4, 16, 8, false) == 27);
	print_test("Vector 4, k = 9", k_selecciones(vector4, 16, 9, false) == 37);
	print_test("Vector 4, k = 10", k_selecciones(vector4, 16, 10, false) == 39);
	print_test("Vector 4, k = 11", k_selecciones(vector4, 16, 11, false) == 49);
	print_test("Vector 4, k = 12", k_selecciones(vector4, 16, 12, false) == 60);
	print_test("Vector 4, k = 13", k_selecciones(vector4, 16, 13, false) == 65);
	print_test("Vector 4, k = 14", k_selecciones(vector4, 16, 14, false) == 70);
	print_test("Vector 4, k = 15", k_selecciones(vector4, 16, 15, false) == 143);

	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", k_selecciones(vector5, 16, 0, false) == 1);
	print_test("Vector 5, k = 1", k_selecciones(vector5, 16, 1, false) == 2);
	print_test("Vector 5, k = 2", k_selecciones(vector5, 16, 2, false) == 2);
	print_test("Vector 5, k = 3", k_selecciones(vector5, 16, 3, false) == 2);
	print_test("Vector 5, k = 4", k_selecciones(vector5, 16, 4, false) == 3);
	print_test("Vector 5, k = 5", k_selecciones(vector5, 16, 5, false) == 4);
	print_test("Vector 5, k = 6", k_selecciones(vector5, 16, 6, false) == 5);
	print_test("Vector 5, k = 7", k_selecciones(vector5, 16, 7, false) == 6);
	print_test("Vector 5, k = 8", k_selecciones(vector5, 16, 8, false) == 7);
	print_test("Vector 5, k = 9", k_selecciones(vector5, 16, 9, false) == 8);
	print_test("Vector 5, k = 10", k_selecciones(vector5, 16, 10, false) == 9);
	print_test("Vector 5, k = 11", k_selecciones(vector5, 16, 11, false) == 11);
	print_test("Vector 5, k = 12", k_selecciones(vector5, 16, 12, false) == 12);
	print_test("Vector 5, k = 13", k_selecciones(vector5, 16, 13, false) == 13);
	print_test("Vector 5, k = 14", k_selecciones(vector5, 16, 14, false) == 15);
	print_test("Vector 5, k = 15", k_selecciones(vector5, 16, 15, false) == 15);

	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", k_selecciones(vector6, 16, 0, true) == -11);
	print_test("Vector 6, k = 1", k_selecciones(vector6, 16, 1, true) == -8);
	print_test("Vector 6, k = 2", k_selecciones(vector6, 16, 2, true) == 1);
	print_test("Vector 6, k = 3", k_selecciones(vector6, 16, 3, true) == 2);
	print_test("Vector 6, k = 4", k_selecciones(vector6, 16, 4, true) == 3);
	print_test("Vector 6, k = 5", k_selecciones(vector6, 16, 5, true) == 4);
	print_test("Vector 6, k = 6", k_selecciones(vector6, 16, 6, true) == 5);
	print_test("Vector 6, k = 7", k_selecciones(vector6, 16, 7, true) == 6);
	print_test("Vector 6, k = 8", k_selecciones(vector6, 16, 8, true) == 7);
	print_test("Vector 6, k = 9", k_selecciones(vector6, 16, 9, true) == 9);
	print_test("Vector 6, k = 10", k_selecciones(vector6, 16, 10, true) == 12);
	print_test("Vector 6, k = 11", k_selecciones(vector6, 16, 11, true) == 13);
	print_test("Vector 6, k = 12", k_selecciones(vector6, 16, 12, true) == 15);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de k-heapsort. */
void prueba_k_heapsort(){
	printf("INICIO DE PRUEBAS DE K-HEAPSORT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector_aux1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};

	print_test("Vector 1, k = 0", k_heapsort(vector_aux1, 16, 0, false) == 1);
	print_test("Vector 1, k = 1", k_heapsort(vector_aux1, 16, 1, false) == 2);
	print_test("Vector 1, k = 2", k_heapsort(vector_aux1, 16, 2, false) == 3);
	print_test("Vector 1, k = 3", k_heapsort(vector_aux1, 16, 3, false) == 4);
	print_test("Vector 1, k = 4", k_heapsort(vector_aux1, 16, 4, false) == 5);
	print_test("Vector 1, k = 5", k_heapsort(vector_aux1, 16, 5, false) == 6);
	print_test("Vector 1, k = 6", k_heapsort(vector_aux1, 16, 6, false) == 7);
	print_test("Vector 1, k = 7", k_heapsort(vector_aux1, 16, 7, false) == 8);
	print_test("Vector 1, k = 8", k_heapsort(vector_aux1, 16, 8, false) == 9);
	print_test("Vector 1, k = 9", k_heapsort(vector_aux1, 16, 9, false) == 10);
	print_test("Vector 1, k = 10", k_heapsort(vector_aux1, 16, 10, false) == 11);
	print_test("Vector 1, k = 11", k_heapsort(vector_aux1, 16, 11, false) == 12);
	print_test("Vector 1, k = 12", k_heapsort(vector_aux1, 16, 12, false) == 13);
	print_test("Vector 1, k = 13", k_heapsort(vector_aux1, 16, 13, false) == 14);
	print_test("Vector 1, k = 14", k_heapsort(vector_aux1, 16, 14, false) == 15);
	print_test("Vector 1, k = 15", k_heapsort(vector_aux1, 16, 15, false) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector_aux2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", k_heapsort(vector_aux2, 16, 0, false) == -43);
	print_test("Vector 2, k = 1", k_heapsort(vector_aux2, 16, 1, false) == -7);
	print_test("Vector 2, k = 2", k_heapsort(vector_aux2, 16, 2, false) == -6);
	print_test("Vector 2, k = 3", k_heapsort(vector_aux2, 16, 3, false) == 0);
	print_test("Vector 2, k = 4", k_heapsort(vector_aux2, 16, 4, false) == 4);
	print_test("Vector 2, k = 5", k_heapsort(vector_aux2, 16, 5, false) == 6);
	print_test("Vector 2, k = 6", k_heapsort(vector_aux2, 16, 6, false) == 7);
	print_test("Vector 2, k = 7", k_heapsort(vector_aux2, 16, 7, false) == 8);
	print_test("Vector 2, k = 8", k_heapsort(vector_aux2, 16, 8, false) == 9);
	print_test("Vector 2, k = 9", k_heapsort(vector_aux2, 16, 9, false) == 10);
	print_test("Vector 2, k = 10", k_heapsort(vector_aux2, 16, 10, false) == 17);
	print_test("Vector 2, k = 11", k_heapsort(vector_aux2, 16, 11, false) == 20);
	print_test("Vector 2, k = 12", k_heapsort(vector_aux2, 16, 12, false) == 35);
	print_test("Vector 2, k = 13", k_heapsort(vector_aux2, 16, 13, false) == 40);
	print_test("Vector 2, k = 14", k_heapsort(vector_aux2, 16, 14, false) == 45);
	print_test("Vector 2, k = 15", k_heapsort(vector_aux2, 16, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector_aux3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", k_heapsort(vector_aux3, 16, 0, false) == -46);
	print_test("Vector 3, k = 1", k_heapsort(vector_aux3, 16, 1, false) == -45);
	print_test("Vector 3, k = 2", k_heapsort(vector_aux3, 16, 2, false) == -9);
	print_test("Vector 3, k = 3", k_heapsort(vector_aux3, 16, 3, false) == 4);
	print_test("Vector 3, k = 4", k_heapsort(vector_aux3, 16, 4, false) == 12);
	print_test("Vector 3, k = 5", k_heapsort(vector_aux3, 16, 5, false) == 17);
	print_test("Vector 3, k = 6", k_heapsort(vector_aux3, 16, 6, false) == 19);
	print_test("Vector 3, k = 7", k_heapsort(vector_aux3, 16, 7, false) == 22);
	print_test("Vector 3, k = 8", k_heapsort(vector_aux3, 16, 8, false) == 27);
	print_test("Vector 3, k = 9", k_heapsort(vector_aux3, 16, 9, false) == 37);
	print_test("Vector 3, k = 10", k_heapsort(vector_aux3, 16, 10, false) == 39);
	print_test("Vector 3, k = 11", k_heapsort(vector_aux3, 16, 11, false) == 49);
	print_test("Vector 3, k = 12", k_heapsort(vector_aux3, 16, 12, false) == 60);
	print_test("Vector 3, k = 13", k_heapsort(vector_aux3, 16, 13, false) == 65);
	print_test("Vector 3, k = 14", k_heapsort(vector_aux3, 16, 14, false) == 70);
	print_test("Vector 3, k = 15", k_heapsort(vector_aux3, 16, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector_aux4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", k_heapsort(vector_aux4, 16, 0, false) == -46);
	print_test("Vector 4, k = 1", k_heapsort(vector_aux4, 16, 1, false) == -45);
	print_test("Vector 4, k = 2", k_heapsort(vector_aux4, 16, 2, false) == -9);
	print_test("Vector 4, k = 3", k_heapsort(vector_aux4, 16, 3, false) == 4);
	print_test("Vector 4, k = 4", k_heapsort(vector_aux4, 16, 4, false) == 12);
	print_test("Vector 4, k = 5", k_heapsort(vector_aux4, 16, 5, false) == 17);
	print_test("Vector 4, k = 6", k_heapsort(vector_aux4, 16, 6, false) == 19);
	print_test("Vector 4, k = 7", k_heapsort(vector_aux4, 16, 7, false) == 22);
	print_test("Vector 4, k = 8", k_heapsort(vector_aux4, 16, 8, false) == 27);
	print_test("Vector 4, k = 9", k_heapsort(vector_aux4, 16, 9, false) == 37);
	print_test("Vector 4, k = 10", k_heapsort(vector_aux4, 16, 10, false) == 39);
	print_test("Vector 4, k = 11", k_heapsort(vector_aux4, 16, 11, false) == 49);
	print_test("Vector 4, k = 12", k_heapsort(vector_aux4, 16, 12, false) == 60);
	print_test("Vector 4, k = 13", k_heapsort(vector_aux4, 16, 13, false) == 65);
	print_test("Vector 4, k = 14", k_heapsort(vector_aux4, 16, 14, false) == 70);
	print_test("Vector 4, k = 15", k_heapsort(vector_aux4, 16, 15, false) == 143);

	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector_aux5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", k_heapsort(vector_aux5, 16, 0, false) == 1);
	print_test("Vector 5, k = 1", k_heapsort(vector_aux5, 16, 1, false) == 2);
	print_test("Vector 5, k = 2", k_heapsort(vector_aux5, 16, 2, false) == 2);
	print_test("Vector 5, k = 3", k_heapsort(vector_aux5, 16, 3, false) == 2);
	print_test("Vector 5, k = 4", k_heapsort(vector_aux5, 16, 4, false) == 3);
	print_test("Vector 5, k = 5", k_heapsort(vector_aux5, 16, 5, false) == 4);
	print_test("Vector 5, k = 6", k_heapsort(vector_aux5, 16, 6, false) == 5);
	print_test("Vector 5, k = 7", k_heapsort(vector_aux5, 16, 7, false) == 6);
	print_test("Vector 5, k = 8", k_heapsort(vector_aux5, 16, 8, false) == 7);
	print_test("Vector 5, k = 9", k_heapsort(vector_aux5, 16, 9, false) == 8);
	print_test("Vector 5, k = 10", k_heapsort(vector_aux5, 16, 10, false) == 9);
	print_test("Vector 5, k = 11", k_heapsort(vector_aux5, 16, 11, false) == 11);
	print_test("Vector 5, k = 12", k_heapsort(vector_aux5, 16, 12, false) == 12);
	print_test("Vector 5, k = 13", k_heapsort(vector_aux5, 16, 13, false) == 13);
	print_test("Vector 5, k = 14", k_heapsort(vector_aux5, 16, 14, false) == 15);
	print_test("Vector 5, k = 15", k_heapsort(vector_aux5, 16, 15, false) == 15);

	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector_aux6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", k_heapsort(vector_aux6, 16, 0, true) == -11);
	print_test("Vector 6, k = 1", k_heapsort(vector_aux6, 16, 1, true) == -8);
	print_test("Vector 6, k = 2", k_heapsort(vector_aux6, 16, 2, true) == 1);
	print_test("Vector 6, k = 3", k_heapsort(vector_aux6, 16, 3, true) == 2);
	print_test("Vector 6, k = 4", k_heapsort(vector_aux6, 16, 4, true) == 3);
	print_test("Vector 6, k = 5", k_heapsort(vector_aux6, 16, 5, true) == 4);
	print_test("Vector 6, k = 6", k_heapsort(vector_aux6, 16, 6, true) == 5);
	print_test("Vector 6, k = 7", k_heapsort(vector_aux6, 16, 7, true) == 6);
	print_test("Vector 6, k = 8", k_heapsort(vector_aux6, 16, 8, true) == 7);
	print_test("Vector 6, k = 9", k_heapsort(vector_aux6, 16, 9, true) == 9);
	print_test("Vector 6, k = 10", k_heapsort(vector_aux6, 16, 10, true) == 12);
	print_test("Vector 6, k = 11", k_heapsort(vector_aux6, 16, 11, true) == 13);
	print_test("Vector 6, k = 12", k_heapsort(vector_aux6, 16, 12, true) == 15);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de HeapSelect. */
void prueba_heap_select(){
	printf("INICIO DE PRUEBAS DE HEAPSELECT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", heap_select(vector1, 16, 0, false) == 1);
	print_test("Vector 1, k = 1", heap_select(vector1, 16, 1, false) == 2);
	print_test("Vector 1, k = 2", heap_select(vector1, 16, 2, false) == 3);
	print_test("Vector 1, k = 3", heap_select(vector1, 16, 3, false) == 4);
	print_test("Vector 1, k = 4", heap_select(vector1, 16, 4, false) == 5);
	print_test("Vector 1, k = 5", heap_select(vector1, 16, 5, false) == 6);
	print_test("Vector 1, k = 6", heap_select(vector1, 16, 6, false) == 7);
	print_test("Vector 1, k = 7", heap_select(vector1, 16, 7, false) == 8);
	print_test("Vector 1, k = 8", heap_select(vector1, 16, 8, false) == 9);
	print_test("Vector 1, k = 9", heap_select(vector1, 16, 9, false) == 10);
	print_test("Vector 1, k = 10", heap_select(vector1, 16, 10, false) == 11);
	print_test("Vector 1, k = 11", heap_select(vector1, 16, 11, false) == 12);
	print_test("Vector 1, k = 12", heap_select(vector1, 16, 12, false) == 13);
	print_test("Vector 1, k = 13", heap_select(vector1, 16, 13, false) == 14);
	print_test("Vector 1, k = 14", heap_select(vector1, 16, 14, false) == 15);
	print_test("Vector 1, k = 15", heap_select(vector1, 16, 15, false) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", heap_select(vector2, 16, 0, false) == -43);
	print_test("Vector 2, k = 1", heap_select(vector2, 16, 1, false) == -7);
	print_test("Vector 2, k = 2", heap_select(vector2, 16, 2, false) == -6);
	print_test("Vector 2, k = 3", heap_select(vector2, 16, 3, false) == 0);
	print_test("Vector 2, k = 4", heap_select(vector2, 16, 4, false) == 4);
	print_test("Vector 2, k = 5", heap_select(vector2, 16, 5, false) == 6);
	print_test("Vector 2, k = 6", heap_select(vector2, 16, 6, false) == 7);
	print_test("Vector 2, k = 7", heap_select(vector2, 16, 7, false) == 8);
	print_test("Vector 2, k = 8", heap_select(vector2, 16, 8, false) == 9);
	print_test("Vector 2, k = 9", heap_select(vector2, 16, 9, false) == 10);
	print_test("Vector 2, k = 10", heap_select(vector2, 16, 10, false) == 17);
	print_test("Vector 2, k = 11", heap_select(vector2, 16, 11, false) == 20);
	print_test("Vector 2, k = 12", heap_select(vector2, 16, 12, false) == 35);
	print_test("Vector 2, k = 13", heap_select(vector2, 16, 13, false) == 40);
	print_test("Vector 2, k = 14", heap_select(vector2, 16, 14, false) == 45);
	print_test("Vector 2, k = 15", heap_select(vector2, 16, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", heap_select(vector3, 16, 0, false) == -46);
	print_test("Vector 3, k = 1", heap_select(vector3, 16, 1, false) == -45);
	print_test("Vector 3, k = 2", heap_select(vector3, 16, 2, false) == -9);
	print_test("Vector 3, k = 3", heap_select(vector3, 16, 3, false) == 4);
	print_test("Vector 3, k = 4", heap_select(vector3, 16, 4, false) == 12);
	print_test("Vector 3, k = 5", heap_select(vector3, 16, 5, false) == 17);
	print_test("Vector 3, k = 6", heap_select(vector3, 16, 6, false) == 19);
	print_test("Vector 3, k = 7", heap_select(vector3, 16, 7, false) == 22);
	print_test("Vector 3, k = 8", heap_select(vector3, 16, 8, false) == 27);
	print_test("Vector 3, k = 9", heap_select(vector3, 16, 9, false) == 37);
	print_test("Vector 3, k = 10", heap_select(vector3, 16, 10, false) == 39);
	print_test("Vector 3, k = 11", heap_select(vector3, 16, 11, false) == 49);
	print_test("Vector 3, k = 12", heap_select(vector3, 16, 12, false) == 60);
	print_test("Vector 3, k = 13", heap_select(vector3, 16, 13, false) == 65);
	print_test("Vector 3, k = 14", heap_select(vector3, 16, 14, false) == 70);
	print_test("Vector 3, k = 15", heap_select(vector3, 16, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", heap_select(vector4, 16, 0, false) == -46);
	print_test("Vector 4, k = 1", heap_select(vector4, 16, 1, false) == -45);
	print_test("Vector 4, k = 2", heap_select(vector4, 16, 2, false) == -9);
	print_test("Vector 4, k = 3", heap_select(vector4, 16, 3, false) == 4);
	print_test("Vector 4, k = 4", heap_select(vector4, 16, 4, false) == 12);
	print_test("Vector 4, k = 5", heap_select(vector4, 16, 5, false) == 17);
	print_test("Vector 4, k = 6", heap_select(vector4, 16, 6, false) == 19);
	print_test("Vector 4, k = 7", heap_select(vector4, 16, 7, false) == 22);
	print_test("Vector 4, k = 8", heap_select(vector4, 16, 8, false) == 27);
	print_test("Vector 4, k = 9", heap_select(vector4, 16, 9, false) == 37);
	print_test("Vector 4, k = 10", heap_select(vector4, 16, 10, false) == 39);
	print_test("Vector 4, k = 11", heap_select(vector4, 16, 11, false) == 49);
	print_test("Vector 4, k = 12", heap_select(vector4, 16, 12, false) == 60);
	print_test("Vector 4, k = 13", heap_select(vector4, 16, 13, false) == 65);
	print_test("Vector 4, k = 14", heap_select(vector4, 16, 14, false) == 70);
	print_test("Vector 4, k = 15", heap_select(vector4, 16, 15, false) == 143);

	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", heap_select(vector5, 16, 0, false) == 1);
	print_test("Vector 5, k = 1", heap_select(vector5, 16, 1, false) == 2);
	print_test("Vector 5, k = 2", heap_select(vector5, 16, 2, false) == 2);
	print_test("Vector 5, k = 3", heap_select(vector5, 16, 3, false) == 2);
	print_test("Vector 5, k = 4", heap_select(vector5, 16, 4, false) == 3);
	print_test("Vector 5, k = 5", heap_select(vector5, 16, 5, false) == 4);
	print_test("Vector 5, k = 6", heap_select(vector5, 16, 6, false) == 5);
	print_test("Vector 5, k = 7", heap_select(vector5, 16, 7, false) == 6);
	print_test("Vector 5, k = 8", heap_select(vector5, 16, 8, false) == 7);
	print_test("Vector 5, k = 9", heap_select(vector5, 16, 9, false) == 8);
	print_test("Vector 5, k = 10", heap_select(vector5, 16, 10, false) == 9);
	print_test("Vector 5, k = 11", heap_select(vector5, 16, 11, false) == 11);
	print_test("Vector 5, k = 12", heap_select(vector5, 16, 12, false) == 12);
	print_test("Vector 5, k = 13", heap_select(vector5, 16, 13, false) == 13);
	print_test("Vector 5, k = 14", heap_select(vector5, 16, 14, false) == 15);
	print_test("Vector 5, k = 15", heap_select(vector5, 16, 15, false) == 15);

	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", heap_select(vector6, 16, 0, true) == -11);
	print_test("Vector 6, k = 1", heap_select(vector6, 16, 1, true) == -8);
	print_test("Vector 6, k = 2", heap_select(vector6, 16, 2, true) == 1);
	print_test("Vector 6, k = 3", heap_select(vector6, 16, 3, true) == 2);
	print_test("Vector 6, k = 4", heap_select(vector6, 16, 4, true) == 3);
	print_test("Vector 6, k = 5", heap_select(vector6, 16, 5, true) == 4);
	print_test("Vector 6, k = 6", heap_select(vector6, 16, 6, true) == 5);
	print_test("Vector 6, k = 7", heap_select(vector6, 16, 7, true) == 6);
	print_test("Vector 6, k = 8", heap_select(vector6, 16, 8, true) == 7);
	print_test("Vector 6, k = 9", heap_select(vector6, 16, 9, true) == 9);
	print_test("Vector 6, k = 10", heap_select(vector6, 16, 10, true) == 12);
	print_test("Vector 6, k = 11", heap_select(vector6, 16, 11, true) == 13);
	print_test("Vector 6, k = 12", heap_select(vector6, 16, 12, true) == 15);


	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de QuickSelect. */
void prueba_quick_select(){
	printf("INICIO DE PRUEBAS DE QUICKSELECT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", quick_select(vector1, 0, 15, 0, false) == 1);
	print_test("Vector 1, k = 1", quick_select(vector1, 0, 15, 1, false) == 2);
	print_test("Vector 1, k = 2", quick_select(vector1, 0, 15, 2, false) == 3);
	print_test("Vector 1, k = 3", quick_select(vector1, 0, 15, 3, false) == 4);
	print_test("Vector 1, k = 4", quick_select(vector1, 0, 15, 4, false) == 5);
	print_test("Vector 1, k = 5", quick_select(vector1, 0, 15, 5, false) == 6);
	print_test("Vector 1, k = 6", quick_select(vector1, 0, 15, 6, false) == 7);
	print_test("Vector 1, k = 7", quick_select(vector1, 0, 15, 7, false) == 8);
	print_test("Vector 1, k = 8", quick_select(vector1, 0, 15, 8, false) == 9);
	print_test("Vector 1, k = 9", quick_select(vector1, 0, 15, 9, false) == 10);
	print_test("Vector 1, k = 10", quick_select(vector1, 0, 15, 10, false) == 11);
	print_test("Vector 1, k = 11", quick_select(vector1, 0, 15, 11, false) == 12);
	print_test("Vector 1, k = 12", quick_select(vector1, 0, 15, 12, false) == 13);
	print_test("Vector 1, k = 13", quick_select(vector1, 0, 15, 13, false) == 14);
	print_test("Vector 1, k = 14", quick_select(vector1, 0, 15, 14, false) == 15);
	print_test("Vector 1, k = 15", quick_select(vector1, 0, 15, 15, false) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", quick_select(vector2, 0, 15, 0, false) == -43);
	print_test("Vector 2, k = 1", quick_select(vector2, 0, 15, 1, false) == -7);
	print_test("Vector 2, k = 2", quick_select(vector2, 0, 15, 2, false) == -6);
	print_test("Vector 2, k = 3", quick_select(vector2, 0, 15, 3, false) == 0);
	print_test("Vector 2, k = 4", quick_select(vector2, 0, 15, 4, false) == 4);
	print_test("Vector 2, k = 5", quick_select(vector2, 0, 15, 5, false) == 6);
	print_test("Vector 2, k = 6", quick_select(vector2, 0, 15, 6, false) == 7);
	print_test("Vector 2, k = 7", quick_select(vector2, 0, 15, 7, false) == 8);
	print_test("Vector 2, k = 8", quick_select(vector2, 0, 15, 8, false) == 9);
	print_test("Vector 2, k = 9", quick_select(vector2, 0, 15, 9, false) == 10);
	print_test("Vector 2, k = 10", quick_select(vector2, 0, 15, 10, false) == 17);
	print_test("Vector 2, k = 11", quick_select(vector2, 0, 15, 11, false) == 20);
	print_test("Vector 2, k = 12", quick_select(vector2, 0, 15, 12, false) == 35);
	print_test("Vector 2, k = 13", quick_select(vector2, 0, 15, 13, false) == 40);
	print_test("Vector 2, k = 14", quick_select(vector2, 0, 15, 14, false) == 45);
	print_test("Vector 2, k = 15", quick_select(vector2, 0, 15, 15, false) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", quick_select(vector3, 0, 15, 0, false) == -46);
	print_test("Vector 3, k = 1", quick_select(vector3, 0, 15, 1, false) == -45);
	print_test("Vector 3, k = 2", quick_select(vector3, 0, 15, 2, false) == -9);
	print_test("Vector 3, k = 3", quick_select(vector3, 0, 15, 3, false) == 4);
	print_test("Vector 3, k = 4", quick_select(vector3, 0, 15, 4, false) == 12);
	print_test("Vector 3, k = 5", quick_select(vector3, 0, 15, 5, false) == 17);
	print_test("Vector 3, k = 6", quick_select(vector3, 0, 15, 6, false) == 19);
	print_test("Vector 3, k = 7", quick_select(vector3, 0, 15, 7, false) == 22);
	print_test("Vector 3, k = 8", quick_select(vector3, 0, 15, 8, false) == 27);
	print_test("Vector 3, k = 9", quick_select(vector3, 0, 15, 9, false) == 37);
	print_test("Vector 3, k = 10", quick_select(vector3, 0, 15, 10, false) == 39);
	print_test("Vector 3, k = 11", quick_select(vector3, 0, 15, 11, false) == 49);
	print_test("Vector 3, k = 12", quick_select(vector3, 0, 15, 12, false) == 60);
	print_test("Vector 3, k = 13", quick_select(vector3, 0, 15, 13, false) == 65);
	print_test("Vector 3, k = 14", quick_select(vector3, 0, 15, 14, false) == 70);
	print_test("Vector 3, k = 15", quick_select(vector3, 0, 15, 15, false) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", quick_select(vector4, 0, 15, 0, false) == -46);
	print_test("Vector 4, k = 1", quick_select(vector4, 0, 15, 1, false) == -45);
	print_test("Vector 4, k = 2", quick_select(vector4, 0, 15, 2, false) == -9);
	print_test("Vector 4, k = 3", quick_select(vector4, 0, 15, 3, false) == 4);
	print_test("Vector 4, k = 4", quick_select(vector4, 0, 15, 4, false) == 12);
	print_test("Vector 4, k = 5", quick_select(vector4, 0, 15, 5, false) == 17);
	print_test("Vector 4, k = 6", quick_select(vector4, 0, 15, 6, false) == 19);
	print_test("Vector 4, k = 7", quick_select(vector4, 0, 15, 7, false) == 22);
	print_test("Vector 4, k = 8", quick_select(vector4, 0, 15, 8, false) == 27);
	print_test("Vector 4, k = 9", quick_select(vector4, 0, 15, 9, false) == 37);
	print_test("Vector 4, k = 10", quick_select(vector4, 0, 15, 10, false) == 39);
	print_test("Vector 4, k = 11", quick_select(vector4, 0, 15, 11, false) == 49);
	print_test("Vector 4, k = 12", quick_select(vector4, 0, 15, 12, false) == 60);
	print_test("Vector 4, k = 13", quick_select(vector4, 0, 15, 13, false) == 65);
	print_test("Vector 4, k = 14", quick_select(vector4, 0, 15, 14, false) == 70);
	print_test("Vector 4, k = 15", quick_select(vector4, 0, 15, 15, false) == 143);

	printf("Pruebas elementos repetidos:\n");
	printf("Prueba vector considerando repetidos en el orden\n");
	int vector5[] = {5,3,2,1,4,6,7,8,9,2,12,11,13,15,15,2};

	print_test("Vector 5, k = 0", quick_select(vector5, 0, 15, 0, false) == 1);
	print_test("Vector 5, k = 1", quick_select(vector5, 0, 15, 1, false) == 2);
	print_test("Vector 5, k = 2", quick_select(vector5, 0, 15, 2, false) == 2);
	print_test("Vector 5, k = 3", quick_select(vector5, 0, 15, 3, false) == 2);
	print_test("Vector 5, k = 4", quick_select(vector5, 0, 15, 4, false) == 3);
	print_test("Vector 5, k = 5", quick_select(vector5, 0, 15, 5, false) == 4);
	print_test("Vector 5, k = 6", quick_select(vector5, 0, 15, 6, false) == 5);
	print_test("Vector 5, k = 7", quick_select(vector5, 0, 15, 7, false) == 6);
	print_test("Vector 5, k = 8", quick_select(vector5, 0, 15, 8, false) == 7);
	print_test("Vector 5, k = 9", quick_select(vector5, 0, 15, 9, false) == 8);
	print_test("Vector 5, k = 10", quick_select(vector5, 0, 15, 10, false) == 9);
	print_test("Vector 5, k = 11", quick_select(vector5, 0, 15, 11, false) == 11);
	print_test("Vector 5, k = 12", quick_select(vector5, 0, 15, 12, false) == 12);
	print_test("Vector 5, k = 13", quick_select(vector5, 0, 15, 13, false) == 13);
	print_test("Vector 5, k = 14", quick_select(vector5, 0, 15, 14, false) == 15);
	print_test("Vector 5, k = 15", quick_select(vector5, 0, 15, 15, false) == 15);

	printf("Prueba vector sin considerar repetidos en el orden\n");
	int vector6[] = {5,3,2,1,4,6,7,-8,9,2,12,-11,13,15,15,2};

	print_test("Vector 6, k = 0", quick_select(vector6, 0, 15, 0, true) == -11);
	print_test("Vector 6, k = 1", quick_select(vector6, 0, 15, 1, true) == -8);
	print_test("Vector 6, k = 2", quick_select(vector6, 0, 15, 2, true) == 1);
	print_test("Vector 6, k = 3", quick_select(vector6, 0, 15, 3, true) == 2);
	print_test("Vector 6, k = 4", quick_select(vector6, 0, 15, 4, true) == 3);
	print_test("Vector 6, k = 5", quick_select(vector6, 0, 15, 5, true) == 4);
	print_test("Vector 6, k = 6", quick_select(vector6, 0, 15, 6, true) == 5);
	print_test("Vector 6, k = 7", quick_select(vector6, 0, 15, 7, true) == 6);
	print_test("Vector 6, k = 8", quick_select(vector6, 0, 15, 8, true) == 7);
	print_test("Vector 6, k = 9", quick_select(vector6, 0, 15, 9, true) == 9);
	print_test("Vector 6, k = 10", quick_select(vector6, 0, 15, 10, true) == 12);
	print_test("Vector 6, k = 11", quick_select(vector6, 0, 15, 11, true) == 13);
	print_test("Vector 6, k = 12", quick_select(vector6, 0, 15, 12, true) == 15);


	printf("\n");
}


/* Función principal que ejecuta las pruebas para cada algoritmo que obtiene el estadístico de orden k. */
int main(){

	prueba_fuerza_bruta();
	prueba_ordenar_y_seleccionar();
	prueba_k_selecciones();
	prueba_k_heapsort();
	prueba_heap_select();
	prueba_quick_select();

	return 0;
}
