#include <stdio.h>
#include <stdlib.h>
#include "heap.c"
#include "estadisticoOrdenK.h"

#define TAM 20


/*******************************************************************
 *             		FUNCIONES AUXILIARES
 ******************************************************************/

/* Recibe un vector de enteros, el largo n del vector, un valor que es
 * el candidato a ser el k elemento más pequeño y el k que permite obtener
 * ese k-ésimo elemento. Devuelve un booleano indicando si el candidato es
 * el k más pequeño. */
bool verificador(int vector[], int n, int candidato, int k){

	int i, contador = 0;

	for(i = 0; i < n; i++){

		if (vector[i] < candidato)
			contador++;
	}

	return (contador == k);
}


/* La funcion swap intercambia dos valores enteros. */
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


/* Función de ordenamiento merge sort. */
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


/*******************************************************************
 *         ALGORITMOS QUE OBTIENEN EL ESTADÍSTICO DE ORDEN K
 ******************************************************************/



int fuerza_bruta(int vector[], int n, int k){
	
	int i;
	for(i = 0; i < n; i++){
		if (verificador(vector, n, vector[i], k))
			return vector[i];
	}

	return NULL;
}


int ordenar_y_seleccionar(int vector[], int n, int k){

	merge_sort(vector, n);
	return vector[k];
}


int k_selecciones(int vector[], int n, int k){

	int i, j, pos_minimo, primero = 0;

	for(i = 0; i < k + 1; i++){
		pos_minimo = i;
		for(j = i + 1; j < n; j++){
			if (vector[pos_minimo] > vector[j])
				pos_minimo = j;
		}

		swap(&vector[pos_minimo], &vector[i]);
	}

	return vector[k];
}


int k_heapsort(void *vector[], int n, int k){

	heap_t* heap = (heap_t*)heapify_vector(vector, n, (cmp_func_f) comparar_numeros_min);
	int i;
	int *kEstadistico;

	for(i = 0; i < k + 1; i++)
		kEstadistico = heap_desencolar(heap);	

	heap_destruir(heap, NULL);
	
	return (*kEstadistico);
	
}


int heap_select(int vector[], int n, int k){
	
	heap_t* heap = heap_crear((cmp_func_f) comparar_numeros_max);
	int i;

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


int quick_select(int vector[], int primero, int ultimo, int k){
	
	if (primero == ultimo)
		return vector[primero];

	int pos_pivote = partition(vector, primero, ultimo);

	if (pos_pivote == k)
		return vector[pos_pivote];

	else if (k < pos_pivote)
		return quick_select(vector, primero, pos_pivote - 1, k);

	return quick_select(vector, pos_pivote + 1, ultimo, k);
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

	print_test("Vector 1, k = 0", fuerza_bruta(vector1, 16, 0) == 1);
	print_test("Vector 1, k = 1", fuerza_bruta(vector1, 16, 1) == 2);
	print_test("Vector 1, k = 2", fuerza_bruta(vector1, 16, 2) == 3);
	print_test("Vector 1, k = 3", fuerza_bruta(vector1, 16, 3) == 4);
	print_test("Vector 1, k = 4", fuerza_bruta(vector1, 16, 4) == 5);
	print_test("Vector 1, k = 5", fuerza_bruta(vector1, 16, 5) == 6);
	print_test("Vector 1, k = 6", fuerza_bruta(vector1, 16, 6) == 7);
	print_test("Vector 1, k = 7", fuerza_bruta(vector1, 16, 7) == 8);
	print_test("Vector 1, k = 8", fuerza_bruta(vector1, 16, 8) == 9);
	print_test("Vector 1, k = 9", fuerza_bruta(vector1, 16, 9) == 10);
	print_test("Vector 1, k = 10", fuerza_bruta(vector1, 16, 10) == 11);
	print_test("Vector 1, k = 11", fuerza_bruta(vector1, 16, 11) == 12);
	print_test("Vector 1, k = 12", fuerza_bruta(vector1, 16, 12) == 13);
	print_test("Vector 1, k = 13", fuerza_bruta(vector1, 16, 13) == 14);
	print_test("Vector 1, k = 14", fuerza_bruta(vector1, 16, 14) == 15);
	print_test("Vector 1, k = 15", fuerza_bruta(vector1, 16, 15) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", fuerza_bruta(vector2, 16, 0) == -43);
	print_test("Vector 2, k = 1", fuerza_bruta(vector2, 16, 1) == -7);
	print_test("Vector 2, k = 2", fuerza_bruta(vector2, 16, 2) == -6);
	print_test("Vector 2, k = 3", fuerza_bruta(vector2, 16, 3) == 0);
	print_test("Vector 2, k = 4", fuerza_bruta(vector2, 16, 4) == 4);
	print_test("Vector 2, k = 5", fuerza_bruta(vector2, 16, 5) == 6);
	print_test("Vector 2, k = 6", fuerza_bruta(vector2, 16, 6) == 7);
	print_test("Vector 2, k = 7", fuerza_bruta(vector2, 16, 7) == 8);
	print_test("Vector 2, k = 8", fuerza_bruta(vector2, 16, 8) == 9);
	print_test("Vector 2, k = 9", fuerza_bruta(vector2, 16, 9) == 10);
	print_test("Vector 2, k = 10", fuerza_bruta(vector2, 16, 10) == 17);
	print_test("Vector 2, k = 11", fuerza_bruta(vector2, 16, 11) == 20);
	print_test("Vector 2, k = 12", fuerza_bruta(vector2, 16, 12) == 35);
	print_test("Vector 2, k = 13", fuerza_bruta(vector2, 16, 13) == 40);
	print_test("Vector 2, k = 14", fuerza_bruta(vector2, 16, 14) == 45);
	print_test("Vector 2, k = 15", fuerza_bruta(vector2, 16, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", fuerza_bruta(vector3, 16, 0) == -46);
	print_test("Vector 3, k = 1", fuerza_bruta(vector3, 16, 1) == -45);
	print_test("Vector 3, k = 2", fuerza_bruta(vector3, 16, 2) == -9);
	print_test("Vector 3, k = 3", fuerza_bruta(vector3, 16, 3) == 4);
	print_test("Vector 3, k = 4", fuerza_bruta(vector3, 16, 4) == 12);
	print_test("Vector 3, k = 5", fuerza_bruta(vector3, 16, 5) == 17);
	print_test("Vector 3, k = 6", fuerza_bruta(vector3, 16, 6) == 19);
	print_test("Vector 3, k = 7", fuerza_bruta(vector3, 16, 7) == 22);
	print_test("Vector 3, k = 8", fuerza_bruta(vector3, 16, 8) == 27);
	print_test("Vector 3, k = 9", fuerza_bruta(vector3, 16, 9) == 37);
	print_test("Vector 3, k = 10", fuerza_bruta(vector3, 16, 10) == 39);
	print_test("Vector 3, k = 11", fuerza_bruta(vector3, 16, 11) == 49);
	print_test("Vector 3, k = 12", fuerza_bruta(vector3, 16, 12) == 60);
	print_test("Vector 3, k = 13", fuerza_bruta(vector3, 16, 13) == 65);
	print_test("Vector 3, k = 14", fuerza_bruta(vector3, 16, 14) == 70);
	print_test("Vector 3, k = 15", fuerza_bruta(vector3, 16, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", fuerza_bruta(vector4, 16, 0) == -46);
	print_test("Vector 4, k = 1", fuerza_bruta(vector4, 16, 1) == -45);
	print_test("Vector 4, k = 2", fuerza_bruta(vector4, 16, 2) == -9);
	print_test("Vector 4, k = 3", fuerza_bruta(vector4, 16, 3) == 4);
	print_test("Vector 4, k = 4", fuerza_bruta(vector4, 16, 4) == 12);
	print_test("Vector 4, k = 5", fuerza_bruta(vector4, 16, 5) == 17);
	print_test("Vector 4, k = 6", fuerza_bruta(vector4, 16, 6) == 19);
	print_test("Vector 4, k = 7", fuerza_bruta(vector4, 16, 7) == 22);
	print_test("Vector 4, k = 8", fuerza_bruta(vector4, 16, 8) == 27);
	print_test("Vector 4, k = 9", fuerza_bruta(vector4, 16, 9) == 37);
	print_test("Vector 4, k = 10", fuerza_bruta(vector4, 16, 10) == 39);
	print_test("Vector 4, k = 11", fuerza_bruta(vector4, 16, 11) == 49);
	print_test("Vector 4, k = 12", fuerza_bruta(vector4, 16, 12) == 60);
	print_test("Vector 4, k = 13", fuerza_bruta(vector4, 16, 13) == 65);
	print_test("Vector 4, k = 14", fuerza_bruta(vector4, 16, 14) == 70);
	print_test("Vector 4, k = 15", fuerza_bruta(vector4, 16, 15) == 143);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de ordenar y seleccionar. */
void prueba_ordenar_y_seleccionar(){
	printf("INICIO DE PRUEBAS DE ORDENAR Y SELECCIONAR\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};

	print_test("Vector 1, k = 0", ordenar_y_seleccionar(vector1, 16, 0) == 1);
	print_test("Vector 1, k = 1", ordenar_y_seleccionar(vector1, 16, 1) == 2);
	print_test("Vector 1, k = 2", ordenar_y_seleccionar(vector1, 16, 2) == 3);
	print_test("Vector 1, k = 3", ordenar_y_seleccionar(vector1, 16, 3) == 4);
	print_test("Vector 1, k = 4", ordenar_y_seleccionar(vector1, 16, 4) == 5);
	print_test("Vector 1, k = 5", ordenar_y_seleccionar(vector1, 16, 5) == 6);
	print_test("Vector 1, k = 6", ordenar_y_seleccionar(vector1, 16, 6) == 7);
	print_test("Vector 1, k = 7", ordenar_y_seleccionar(vector1, 16, 7) == 8);
	print_test("Vector 1, k = 8", ordenar_y_seleccionar(vector1, 16, 8) == 9);
	print_test("Vector 1, k = 9", ordenar_y_seleccionar(vector1, 16, 9) == 10);
	print_test("Vector 1, k = 10", ordenar_y_seleccionar(vector1, 16, 10) == 11);
	print_test("Vector 1, k = 11", ordenar_y_seleccionar(vector1, 16, 11) == 12);
	print_test("Vector 1, k = 12", ordenar_y_seleccionar(vector1, 16, 12) == 13);
	print_test("Vector 1, k = 13", ordenar_y_seleccionar(vector1, 16, 13) == 14);
	print_test("Vector 1, k = 14", ordenar_y_seleccionar(vector1, 16, 14) == 15);
	print_test("Vector 1, k = 15", ordenar_y_seleccionar(vector1, 16, 15) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", ordenar_y_seleccionar(vector2, 16, 0) == -43);
	print_test("Vector 2, k = 1", ordenar_y_seleccionar(vector2, 16, 1) == -7);
	print_test("Vector 2, k = 2", ordenar_y_seleccionar(vector2, 16, 2) == -6);
	print_test("Vector 2, k = 3", ordenar_y_seleccionar(vector2, 16, 3) == 0);
	print_test("Vector 2, k = 4", ordenar_y_seleccionar(vector2, 16, 4) == 4);
	print_test("Vector 2, k = 5", ordenar_y_seleccionar(vector2, 16, 5) == 6);
	print_test("Vector 2, k = 6", ordenar_y_seleccionar(vector2, 16, 6) == 7);
	print_test("Vector 2, k = 7", ordenar_y_seleccionar(vector2, 16, 7) == 8);
	print_test("Vector 2, k = 8", ordenar_y_seleccionar(vector2, 16, 8) == 9);
	print_test("Vector 2, k = 9", ordenar_y_seleccionar(vector2, 16, 9) == 10);
	print_test("Vector 2, k = 10", ordenar_y_seleccionar(vector2, 16, 10) == 17);
	print_test("Vector 2, k = 11", ordenar_y_seleccionar(vector2, 16, 11) == 20);
	print_test("Vector 2, k = 12", ordenar_y_seleccionar(vector2, 16, 12) == 35);
	print_test("Vector 2, k = 13", ordenar_y_seleccionar(vector2, 16, 13) == 40);
	print_test("Vector 2, k = 14", ordenar_y_seleccionar(vector2, 16, 14) == 45);
	print_test("Vector 2, k = 15", ordenar_y_seleccionar(vector2, 16, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", ordenar_y_seleccionar(vector3, 16, 0) == -46);
	print_test("Vector 3, k = 1", ordenar_y_seleccionar(vector3, 16, 1) == -45);
	print_test("Vector 3, k = 2", ordenar_y_seleccionar(vector3, 16, 2) == -9);
	print_test("Vector 3, k = 3", ordenar_y_seleccionar(vector3, 16, 3) == 4);
	print_test("Vector 3, k = 4", ordenar_y_seleccionar(vector3, 16, 4) == 12);
	print_test("Vector 3, k = 5", ordenar_y_seleccionar(vector3, 16, 5) == 17);
	print_test("Vector 3, k = 6", ordenar_y_seleccionar(vector3, 16, 6) == 19);
	print_test("Vector 3, k = 7", ordenar_y_seleccionar(vector3, 16, 7) == 22);
	print_test("Vector 3, k = 8", ordenar_y_seleccionar(vector3, 16, 8) == 27);
	print_test("Vector 3, k = 9", ordenar_y_seleccionar(vector3, 16, 9) == 37);
	print_test("Vector 3, k = 10", ordenar_y_seleccionar(vector3, 16, 10) == 39);
	print_test("Vector 3, k = 11", ordenar_y_seleccionar(vector3, 16, 11) == 49);
	print_test("Vector 3, k = 12", ordenar_y_seleccionar(vector3, 16, 12) == 60);
	print_test("Vector 3, k = 13", ordenar_y_seleccionar(vector3, 16, 13) == 65);
	print_test("Vector 3, k = 14", ordenar_y_seleccionar(vector3, 16, 14) == 70);
	print_test("Vector 3, k = 15", ordenar_y_seleccionar(vector3, 16, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", ordenar_y_seleccionar(vector4, 16, 0) == -46);
	print_test("Vector 4, k = 1", ordenar_y_seleccionar(vector4, 16, 1) == -45);
	print_test("Vector 4, k = 2", ordenar_y_seleccionar(vector4, 16, 2) == -9);
	print_test("Vector 4, k = 3", ordenar_y_seleccionar(vector4, 16, 3) == 4);
	print_test("Vector 4, k = 4", ordenar_y_seleccionar(vector4, 16, 4) == 12);
	print_test("Vector 4, k = 5", ordenar_y_seleccionar(vector4, 16, 5) == 17);
	print_test("Vector 4, k = 6", ordenar_y_seleccionar(vector4, 16, 6) == 19);
	print_test("Vector 4, k = 7", ordenar_y_seleccionar(vector4, 16, 7) == 22);
	print_test("Vector 4, k = 8", ordenar_y_seleccionar(vector4, 16, 8) == 27);
	print_test("Vector 4, k = 9", ordenar_y_seleccionar(vector4, 16, 9) == 37);
	print_test("Vector 4, k = 10", ordenar_y_seleccionar(vector4, 16, 10) == 39);
	print_test("Vector 4, k = 11", ordenar_y_seleccionar(vector4, 16, 11) == 49);
	print_test("Vector 4, k = 12", ordenar_y_seleccionar(vector4, 16, 12) == 60);
	print_test("Vector 4, k = 13", ordenar_y_seleccionar(vector4, 16, 13) == 65);
	print_test("Vector 4, k = 14", ordenar_y_seleccionar(vector4, 16, 14) == 70);
	print_test("Vector 4, k = 15", ordenar_y_seleccionar(vector4, 16, 15) == 143);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de k-selecciones. */
void prueba_k_selecciones(){
	printf("INICIO DE PRUEBAS DE K-SELECCIONES\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", k_selecciones(vector1, 16, 0) == 1);
	print_test("Vector 1, k = 1", k_selecciones(vector1, 16, 1) == 2);
	print_test("Vector 1, k = 2", k_selecciones(vector1, 16, 2) == 3);
	print_test("Vector 1, k = 3", k_selecciones(vector1, 16, 3) == 4);
	print_test("Vector 1, k = 4", k_selecciones(vector1, 16, 4) == 5);
	print_test("Vector 1, k = 5", k_selecciones(vector1, 16, 5) == 6);
	print_test("Vector 1, k = 6", k_selecciones(vector1, 16, 6) == 7);
	print_test("Vector 1, k = 7", k_selecciones(vector1, 16, 7) == 8);
	print_test("Vector 1, k = 8", k_selecciones(vector1, 16, 8) == 9);
	print_test("Vector 1, k = 9", k_selecciones(vector1, 16, 9) == 10);
	print_test("Vector 1, k = 10", k_selecciones(vector1, 16, 10) == 11);
	print_test("Vector 1, k = 11", k_selecciones(vector1, 16, 11) == 12);
	print_test("Vector 1, k = 12", k_selecciones(vector1, 16, 12) == 13);
	print_test("Vector 1, k = 13", k_selecciones(vector1, 16, 13) == 14);
	print_test("Vector 1, k = 14", k_selecciones(vector1, 16, 14) == 15);
	print_test("Vector 1, k = 15", k_selecciones(vector1, 16, 15) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", k_selecciones(vector2, 16, 0) == -43);
	print_test("Vector 2, k = 1", k_selecciones(vector2, 16, 1) == -7);
	print_test("Vector 2, k = 2", k_selecciones(vector2, 16, 2) == -6);
	print_test("Vector 2, k = 3", k_selecciones(vector2, 16, 3) == 0);
	print_test("Vector 2, k = 4", k_selecciones(vector2, 16, 4) == 4);
	print_test("Vector 2, k = 5", k_selecciones(vector2, 16, 5) == 6);
	print_test("Vector 2, k = 6", k_selecciones(vector2, 16, 6) == 7);
	print_test("Vector 2, k = 7", k_selecciones(vector2, 16, 7) == 8);
	print_test("Vector 2, k = 8", k_selecciones(vector2, 16, 8) == 9);
	print_test("Vector 2, k = 9", k_selecciones(vector2, 16, 9) == 10);
	print_test("Vector 2, k = 10", k_selecciones(vector2, 16, 10) == 17);
	print_test("Vector 2, k = 11", k_selecciones(vector2, 16, 11) == 20);
	print_test("Vector 2, k = 12", k_selecciones(vector2, 16, 12) == 35);
	print_test("Vector 2, k = 13", k_selecciones(vector2, 16, 13) == 40);
	print_test("Vector 2, k = 14", k_selecciones(vector2, 16, 14) == 45);
	print_test("Vector 2, k = 15", k_selecciones(vector2, 16, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", k_selecciones(vector3, 16, 0) == -46);
	print_test("Vector 3, k = 1", k_selecciones(vector3, 16, 1) == -45);
	print_test("Vector 3, k = 2", k_selecciones(vector3, 16, 2) == -9);
	print_test("Vector 3, k = 3", k_selecciones(vector3, 16, 3) == 4);
	print_test("Vector 3, k = 4", k_selecciones(vector3, 16, 4) == 12);
	print_test("Vector 3, k = 5", k_selecciones(vector3, 16, 5) == 17);
	print_test("Vector 3, k = 6", k_selecciones(vector3, 16, 6) == 19);
	print_test("Vector 3, k = 7", k_selecciones(vector3, 16, 7) == 22);
	print_test("Vector 3, k = 8", k_selecciones(vector3, 16, 8) == 27);
	print_test("Vector 3, k = 9", k_selecciones(vector3, 16, 9) == 37);
	print_test("Vector 3, k = 10", k_selecciones(vector3, 16, 10) == 39);
	print_test("Vector 3, k = 11", k_selecciones(vector3, 16, 11) == 49);
	print_test("Vector 3, k = 12", k_selecciones(vector3, 16, 12) == 60);
	print_test("Vector 3, k = 13", k_selecciones(vector3, 16, 13) == 65);
	print_test("Vector 3, k = 14", k_selecciones(vector3, 16, 14) == 70);
	print_test("Vector 3, k = 15", k_selecciones(vector3, 16, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", k_selecciones(vector4, 16, 0) == -46);
	print_test("Vector 4, k = 1", k_selecciones(vector4, 16, 1) == -45);
	print_test("Vector 4, k = 2", k_selecciones(vector4, 16, 2) == -9);
	print_test("Vector 4, k = 3", k_selecciones(vector4, 16, 3) == 4);
	print_test("Vector 4, k = 4", k_selecciones(vector4, 16, 4) == 12);
	print_test("Vector 4, k = 5", k_selecciones(vector4, 16, 5) == 17);
	print_test("Vector 4, k = 6", k_selecciones(vector4, 16, 6) == 19);
	print_test("Vector 4, k = 7", k_selecciones(vector4, 16, 7) == 22);
	print_test("Vector 4, k = 8", k_selecciones(vector4, 16, 8) == 27);
	print_test("Vector 4, k = 9", k_selecciones(vector4, 16, 9) == 37);
	print_test("Vector 4, k = 10", k_selecciones(vector4, 16, 10) == 39);
	print_test("Vector 4, k = 11", k_selecciones(vector4, 16, 11) == 49);
	print_test("Vector 4, k = 12", k_selecciones(vector4, 16, 12) == 60);
	print_test("Vector 4, k = 13", k_selecciones(vector4, 16, 13) == 65);
	print_test("Vector 4, k = 14", k_selecciones(vector4, 16, 14) == 70);
	print_test("Vector 4, k = 15", k_selecciones(vector4, 16, 15) == 143);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de k-heapsort. */
void prueba_k_heapsort(){
	printf("INICIO DE PRUEBAS DE K-HEAPSORT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	int i;

	void *vector_aux1[TAM];
	for(i = 0; i < 16; i++){
		vector_aux1[i]=(void*)&vector1[i];
	}

	print_test("Vector 1, k = 0", k_heapsort(vector_aux1, 16, 0) == 1);
	print_test("Vector 1, k = 1", k_heapsort(vector_aux1, 16, 1) == 2);
	print_test("Vector 1, k = 2", k_heapsort(vector_aux1, 16, 2) == 3);
	print_test("Vector 1, k = 3", k_heapsort(vector_aux1, 16, 3) == 4);
	print_test("Vector 1, k = 4", k_heapsort(vector_aux1, 16, 4) == 5);
	print_test("Vector 1, k = 5", k_heapsort(vector_aux1, 16, 5) == 6);
	print_test("Vector 1, k = 6", k_heapsort(vector_aux1, 16, 6) == 7);
	print_test("Vector 1, k = 7", k_heapsort(vector_aux1, 16, 7) == 8);
	print_test("Vector 1, k = 8", k_heapsort(vector_aux1, 16, 8) == 9);
	print_test("Vector 1, k = 9", k_heapsort(vector_aux1, 16, 9) == 10);
	print_test("Vector 1, k = 10", k_heapsort(vector_aux1, 16, 10) == 11);
	print_test("Vector 1, k = 11", k_heapsort(vector_aux1, 16, 11) == 12);
	print_test("Vector 1, k = 12", k_heapsort(vector_aux1, 16, 12) == 13);
	print_test("Vector 1, k = 13", k_heapsort(vector_aux1, 16, 13) == 14);
	print_test("Vector 1, k = 14", k_heapsort(vector_aux1, 16, 14) == 15);
	print_test("Vector 1, k = 15", k_heapsort(vector_aux1, 16, 15) == 16);


	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	void *vector_aux2[TAM];
	for(i = 0; i < 16; i++){
		vector_aux2[i]=(void*)&vector2[i];
	}

	print_test("Vector 2, k = 0", k_heapsort(vector_aux2, 16, 0) == -43);
	print_test("Vector 2, k = 1", k_heapsort(vector_aux2, 16, 1) == -7);
	print_test("Vector 2, k = 2", k_heapsort(vector_aux2, 16, 2) == -6);
	print_test("Vector 2, k = 3", k_heapsort(vector_aux2, 16, 3) == 0);
	print_test("Vector 2, k = 4", k_heapsort(vector_aux2, 16, 4) == 4);
	print_test("Vector 2, k = 5", k_heapsort(vector_aux2, 16, 5) == 6);
	print_test("Vector 2, k = 6", k_heapsort(vector_aux2, 16, 6) == 7);
	print_test("Vector 2, k = 7", k_heapsort(vector_aux2, 16, 7) == 8);
	print_test("Vector 2, k = 8", k_heapsort(vector_aux2, 16, 8) == 9);
	print_test("Vector 2, k = 9", k_heapsort(vector_aux2, 16, 9) == 10);
	print_test("Vector 2, k = 10", k_heapsort(vector_aux2, 16, 10) == 17);
	print_test("Vector 2, k = 11", k_heapsort(vector_aux2, 16, 11) == 20);
	print_test("Vector 2, k = 12", k_heapsort(vector_aux2, 16, 12) == 35);
	print_test("Vector 2, k = 13", k_heapsort(vector_aux2, 16, 13) == 40);
	print_test("Vector 2, k = 14", k_heapsort(vector_aux2, 16, 14) == 45);
	print_test("Vector 2, k = 15", k_heapsort(vector_aux2, 16, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	void *vector_aux3[TAM];
	for(i = 0; i < 16; i++){
		vector_aux3[i]=(void*)&vector3[i];
	}

	print_test("Vector 3, k = 0", k_heapsort(vector_aux3, 16, 0) == -46);
	print_test("Vector 3, k = 1", k_heapsort(vector_aux3, 16, 1) == -45);
	print_test("Vector 3, k = 2", k_heapsort(vector_aux3, 16, 2) == -9);
	print_test("Vector 3, k = 3", k_heapsort(vector_aux3, 16, 3) == 4);
	print_test("Vector 3, k = 4", k_heapsort(vector_aux3, 16, 4) == 12);
	print_test("Vector 3, k = 5", k_heapsort(vector_aux3, 16, 5) == 17);
	print_test("Vector 3, k = 6", k_heapsort(vector_aux3, 16, 6) == 19);
	print_test("Vector 3, k = 7", k_heapsort(vector_aux3, 16, 7) == 22);
	print_test("Vector 3, k = 8", k_heapsort(vector_aux3, 16, 8) == 27);
	print_test("Vector 3, k = 9", k_heapsort(vector_aux3, 16, 9) == 37);
	print_test("Vector 3, k = 10", k_heapsort(vector_aux3, 16, 10) == 39);
	print_test("Vector 3, k = 11", k_heapsort(vector_aux3, 16, 11) == 49);
	print_test("Vector 3, k = 12", k_heapsort(vector_aux3, 16, 12) == 60);
	print_test("Vector 3, k = 13", k_heapsort(vector_aux3, 16, 13) == 65);
	print_test("Vector 3, k = 14", k_heapsort(vector_aux3, 16, 14) == 70);
	print_test("Vector 3, k = 15", k_heapsort(vector_aux3, 16, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	void *vector_aux4[TAM];
	for(i = 0; i < 16; i++){
		vector_aux4[i]=(void*)&vector4[i];
	}

	print_test("Vector 4, k = 0", k_heapsort(vector_aux4, 16, 0) == -46);
	print_test("Vector 4, k = 1", k_heapsort(vector_aux4, 16, 1) == -45);
	print_test("Vector 4, k = 2", k_heapsort(vector_aux4, 16, 2) == -9);
	print_test("Vector 4, k = 3", k_heapsort(vector_aux4, 16, 3) == 4);
	print_test("Vector 4, k = 4", k_heapsort(vector_aux4, 16, 4) == 12);
	print_test("Vector 4, k = 5", k_heapsort(vector_aux4, 16, 5) == 17);
	print_test("Vector 4, k = 6", k_heapsort(vector_aux4, 16, 6) == 19);
	print_test("Vector 4, k = 7", k_heapsort(vector_aux4, 16, 7) == 22);
	print_test("Vector 4, k = 8", k_heapsort(vector_aux4, 16, 8) == 27);
	print_test("Vector 4, k = 9", k_heapsort(vector_aux4, 16, 9) == 37);
	print_test("Vector 4, k = 10", k_heapsort(vector_aux4, 16, 10) == 39);
	print_test("Vector 4, k = 11", k_heapsort(vector_aux4, 16, 11) == 49);
	print_test("Vector 4, k = 12", k_heapsort(vector_aux4, 16, 12) == 60);
	print_test("Vector 4, k = 13", k_heapsort(vector_aux4, 16, 13) == 65);
	print_test("Vector 4, k = 14", k_heapsort(vector_aux4, 16, 14) == 70);
	print_test("Vector 4, k = 15", k_heapsort(vector_aux4, 16, 15) == 143);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de HeapSelect. */
void prueba_heap_select(){
	printf("INICIO DE PRUEBAS DE HEAPSELECT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", heap_select(vector1, 16, 0) == 1);
	print_test("Vector 1, k = 1", heap_select(vector1, 16, 1) == 2);
	print_test("Vector 1, k = 2", heap_select(vector1, 16, 2) == 3);
	print_test("Vector 1, k = 3", heap_select(vector1, 16, 3) == 4);
	print_test("Vector 1, k = 4", heap_select(vector1, 16, 4) == 5);
	print_test("Vector 1, k = 5", heap_select(vector1, 16, 5) == 6);
	print_test("Vector 1, k = 6", heap_select(vector1, 16, 6) == 7);
	print_test("Vector 1, k = 7", heap_select(vector1, 16, 7) == 8);
	print_test("Vector 1, k = 8", heap_select(vector1, 16, 8) == 9);
	print_test("Vector 1, k = 9", heap_select(vector1, 16, 9) == 10);
	print_test("Vector 1, k = 10", heap_select(vector1, 16, 10) == 11);
	print_test("Vector 1, k = 11", heap_select(vector1, 16, 11) == 12);
	print_test("Vector 1, k = 12", heap_select(vector1, 16, 12) == 13);
	print_test("Vector 1, k = 13", heap_select(vector1, 16, 13) == 14);
	print_test("Vector 1, k = 14", heap_select(vector1, 16, 14) == 15);
	print_test("Vector 1, k = 15", heap_select(vector1, 16, 15) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", heap_select(vector2, 16, 0) == -43);
	print_test("Vector 2, k = 1", heap_select(vector2, 16, 1) == -7);
	print_test("Vector 2, k = 2", heap_select(vector2, 16, 2) == -6);
	print_test("Vector 2, k = 3", heap_select(vector2, 16, 3) == 0);
	print_test("Vector 2, k = 4", heap_select(vector2, 16, 4) == 4);
	print_test("Vector 2, k = 5", heap_select(vector2, 16, 5) == 6);
	print_test("Vector 2, k = 6", heap_select(vector2, 16, 6) == 7);
	print_test("Vector 2, k = 7", heap_select(vector2, 16, 7) == 8);
	print_test("Vector 2, k = 8", heap_select(vector2, 16, 8) == 9);
	print_test("Vector 2, k = 9", heap_select(vector2, 16, 9) == 10);
	print_test("Vector 2, k = 10", heap_select(vector2, 16, 10) == 17);
	print_test("Vector 2, k = 11", heap_select(vector2, 16, 11) == 20);
	print_test("Vector 2, k = 12", heap_select(vector2, 16, 12) == 35);
	print_test("Vector 2, k = 13", heap_select(vector2, 16, 13) == 40);
	print_test("Vector 2, k = 14", heap_select(vector2, 16, 14) == 45);
	print_test("Vector 2, k = 15", heap_select(vector2, 16, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", heap_select(vector3, 16, 0) == -46);
	print_test("Vector 3, k = 1", heap_select(vector3, 16, 1) == -45);
	print_test("Vector 3, k = 2", heap_select(vector3, 16, 2) == -9);
	print_test("Vector 3, k = 3", heap_select(vector3, 16, 3) == 4);
	print_test("Vector 3, k = 4", heap_select(vector3, 16, 4) == 12);
	print_test("Vector 3, k = 5", heap_select(vector3, 16, 5) == 17);
	print_test("Vector 3, k = 6", heap_select(vector3, 16, 6) == 19);
	print_test("Vector 3, k = 7", heap_select(vector3, 16, 7) == 22);
	print_test("Vector 3, k = 8", heap_select(vector3, 16, 8) == 27);
	print_test("Vector 3, k = 9", heap_select(vector3, 16, 9) == 37);
	print_test("Vector 3, k = 10", heap_select(vector3, 16, 10) == 39);
	print_test("Vector 3, k = 11", heap_select(vector3, 16, 11) == 49);
	print_test("Vector 3, k = 12", heap_select(vector3, 16, 12) == 60);
	print_test("Vector 3, k = 13", heap_select(vector3, 16, 13) == 65);
	print_test("Vector 3, k = 14", heap_select(vector3, 16, 14) == 70);
	print_test("Vector 3, k = 15", heap_select(vector3, 16, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", heap_select(vector4, 16, 0) == -46);
	print_test("Vector 4, k = 1", heap_select(vector4, 16, 1) == -45);
	print_test("Vector 4, k = 2", heap_select(vector4, 16, 2) == -9);
	print_test("Vector 4, k = 3", heap_select(vector4, 16, 3) == 4);
	print_test("Vector 4, k = 4", heap_select(vector4, 16, 4) == 12);
	print_test("Vector 4, k = 5", heap_select(vector4, 16, 5) == 17);
	print_test("Vector 4, k = 6", heap_select(vector4, 16, 6) == 19);
	print_test("Vector 4, k = 7", heap_select(vector4, 16, 7) == 22);
	print_test("Vector 4, k = 8", heap_select(vector4, 16, 8) == 27);
	print_test("Vector 4, k = 9", heap_select(vector4, 16, 9) == 37);
	print_test("Vector 4, k = 10", heap_select(vector4, 16, 10) == 39);
	print_test("Vector 4, k = 11", heap_select(vector4, 16, 11) == 49);
	print_test("Vector 4, k = 12", heap_select(vector4, 16, 12) == 60);
	print_test("Vector 4, k = 13", heap_select(vector4, 16, 13) == 65);
	print_test("Vector 4, k = 14", heap_select(vector4, 16, 14) == 70);
	print_test("Vector 4, k = 15", heap_select(vector4, 16, 15) == 143);

	printf("\n");
}


/* Función que realiza pruebas para el algoritmo de QuickSelect. */
void prueba_quick_select(){
	printf("INICIO DE PRUEBAS DE QUICKSELECT\n");
	
	printf("Pruebas elementos sin repetir:\n");
	printf("Prueba vector semi-ordenado\n");
	int vector1[] = {5,3,2,1,4,6,7,8,9,10,11,12,13,14,15,16};
	
	print_test("Vector 1, k = 0", quick_select(vector1, 0, 15, 0) == 1);
	print_test("Vector 1, k = 1", quick_select(vector1, 0, 15, 1) == 2);
	print_test("Vector 1, k = 2", quick_select(vector1, 0, 15, 2) == 3);
	print_test("Vector 1, k = 3", quick_select(vector1, 0, 15, 3) == 4);
	print_test("Vector 1, k = 4", quick_select(vector1, 0, 15, 4) == 5);
	print_test("Vector 1, k = 5", quick_select(vector1, 0, 15, 5) == 6);
	print_test("Vector 1, k = 6", quick_select(vector1, 0, 15, 6) == 7);
	print_test("Vector 1, k = 7", quick_select(vector1, 0, 15, 7) == 8);
	print_test("Vector 1, k = 8", quick_select(vector1, 0, 15, 8) == 9);
	print_test("Vector 1, k = 9", quick_select(vector1, 0, 15, 9) == 10);
	print_test("Vector 1, k = 10", quick_select(vector1, 0, 15, 10) == 11);
	print_test("Vector 1, k = 11", quick_select(vector1, 0, 15, 11) == 12);
	print_test("Vector 1, k = 12", quick_select(vector1, 0, 15, 12) == 13);
	print_test("Vector 1, k = 13", quick_select(vector1, 0, 15, 13) == 14);
	print_test("Vector 1, k = 14", quick_select(vector1, 0, 15, 14) == 15);
	print_test("Vector 1, k = 15", quick_select(vector1, 0, 15, 15) == 16);

	printf("Prueba vector ordenado de menor a mayor\n");
	int vector2[] = {-43,-7,-6,0,4,6,7,8,9,10,17,20,35,40,45,46};

	print_test("Vector 2, k = 0", quick_select(vector2, 0, 15, 0) == -43);
	print_test("Vector 2, k = 1", quick_select(vector2, 0, 15, 1) == -7);
	print_test("Vector 2, k = 2", quick_select(vector2, 0, 15, 2) == -6);
	print_test("Vector 2, k = 3", quick_select(vector2, 0, 15, 3) == 0);
	print_test("Vector 2, k = 4", quick_select(vector2, 0, 15, 4) == 4);
	print_test("Vector 2, k = 5", quick_select(vector2, 0, 15, 5) == 6);
	print_test("Vector 2, k = 6", quick_select(vector2, 0, 15, 6) == 7);
	print_test("Vector 2, k = 7", quick_select(vector2, 0, 15, 7) == 8);
	print_test("Vector 2, k = 8", quick_select(vector2, 0, 15, 8) == 9);
	print_test("Vector 2, k = 9", quick_select(vector2, 0, 15, 9) == 10);
	print_test("Vector 2, k = 10", quick_select(vector2, 0, 15, 10) == 17);
	print_test("Vector 2, k = 11", quick_select(vector2, 0, 15, 11) == 20);
	print_test("Vector 2, k = 12", quick_select(vector2, 0, 15, 12) == 35);
	print_test("Vector 2, k = 13", quick_select(vector2, 0, 15, 13) == 40);
	print_test("Vector 2, k = 14", quick_select(vector2, 0, 15, 14) == 45);
	print_test("Vector 2, k = 15", quick_select(vector2, 0, 15, 15) == 46);


	printf("Prueba vector ordenado de mayor a menor\n");
	int vector3[] = {143,70,65,60,49,39,37,27,22,19,17,12,4,-9,-45,-46};

	print_test("Vector 3, k = 0", quick_select(vector3, 0, 15, 0) == -46);
	print_test("Vector 3, k = 1", quick_select(vector3, 0, 15, 1) == -45);
	print_test("Vector 3, k = 2", quick_select(vector3, 0, 15, 2) == -9);
	print_test("Vector 3, k = 3", quick_select(vector3, 0, 15, 3) == 4);
	print_test("Vector 3, k = 4", quick_select(vector3, 0, 15, 4) == 12);
	print_test("Vector 3, k = 5", quick_select(vector3, 0, 15, 5) == 17);
	print_test("Vector 3, k = 6", quick_select(vector3, 0, 15, 6) == 19);
	print_test("Vector 3, k = 7", quick_select(vector3, 0, 15, 7) == 22);
	print_test("Vector 3, k = 8", quick_select(vector3, 0, 15, 8) == 27);
	print_test("Vector 3, k = 9", quick_select(vector3, 0, 15, 9) == 37);
	print_test("Vector 3, k = 10", quick_select(vector3, 0, 15, 10) == 39);
	print_test("Vector 3, k = 11", quick_select(vector3, 0, 15, 11) == 49);
	print_test("Vector 3, k = 12", quick_select(vector3, 0, 15, 12) == 60);
	print_test("Vector 3, k = 13", quick_select(vector3, 0, 15, 13) == 65);
	print_test("Vector 3, k = 14", quick_select(vector3, 0, 15, 14) == 70);
	print_test("Vector 3, k = 15", quick_select(vector3, 0, 15, 15) == 143);


	printf("Prueba vector desordenado\n");
	int vector4[] = {70,39,65,49,37,143,4,22,17,19,-9,12,-45,27,-46,60};

	print_test("Vector 4, k = 0", quick_select(vector4, 0, 15, 0) == -46);
	print_test("Vector 4, k = 1", quick_select(vector4, 0, 15, 1) == -45);
	print_test("Vector 4, k = 2", quick_select(vector4, 0, 15, 2) == -9);
	print_test("Vector 4, k = 3", quick_select(vector4, 0, 15, 3) == 4);
	print_test("Vector 4, k = 4", quick_select(vector4, 0, 15, 4) == 12);
	print_test("Vector 4, k = 5", quick_select(vector4, 0, 15, 5) == 17);
	print_test("Vector 4, k = 6", quick_select(vector4, 0, 15, 6) == 19);
	print_test("Vector 4, k = 7", quick_select(vector4, 0, 15, 7) == 22);
	print_test("Vector 4, k = 8", quick_select(vector4, 0, 15, 8) == 27);
	print_test("Vector 4, k = 9", quick_select(vector4, 0, 15, 9) == 37);
	print_test("Vector 4, k = 10", quick_select(vector4, 0, 15, 10) == 39);
	print_test("Vector 4, k = 11", quick_select(vector4, 0, 15, 11) == 49);
	print_test("Vector 4, k = 12", quick_select(vector4, 0, 15, 12) == 60);
	print_test("Vector 4, k = 13", quick_select(vector4, 0, 15, 13) == 65);
	print_test("Vector 4, k = 14", quick_select(vector4, 0, 15, 14) == 70);
	print_test("Vector 4, k = 15", quick_select(vector4, 0, 15, 15) == 143);

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
