#include <stdio.h>
#include <stdlib.h>
#include "heap.c"
#include "estadisticoOrdenK.h"


bool verificador(int vector[], int n, int candidato, int k){

	int i, contador = 0;

	for(i = 0; i < n; i++){
		if (vector[i] < candidato)
			contador++;
	}

	return (contador == k);
}

int fuerza_bruta(int vector[], int n, int k){
	
	int i;
	for(i = 0; i < n; i++){
		if (verificador(vector, n, vector[i], k))
			return vector[i];
	}

	return NULL;
}


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

int ordenar_y_seleccionar(int vector[], int n, int k){

	merge_sort(vector, n);
	return vector[k];
}

int k_selecciones(int vector[], int n, int k){

	int cont,i, pos_minimo = 0;

	for(cont = 0; cont < k; cont++){
		for(i = 0; i < n; i++){
			if (vector[pos_minimo] > vector[i])
				pos_minimo = i;
		}
		
		int aux = vector[pos_minimo];
		vector[pos_minimo] = vector[cont];
		vector[cont] = aux;
	}
	
	return vector[k];
}

int comparar_numeros_min(int *a, int *b){

	if (*a == *b)
		return 0;

	if (*a < *b)
		return 1;

	return -1;
}

int comparar_numeros_max(int *a, int *b){

	if (*a == *b)
		return 0;

	if (*a < *b)
		return -1;

	return 1;
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

void swap (int* x, int* y){

	int auxiliar = *x;
	*x = *y;
	*y = auxiliar;
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

int quick_select(int vector[], int primero, int ultimo, int k){
	
	if (primero == ultimo)
		return vector[primero];

	int pos_pivote = partition(vector, primero, ultimo);

/*
	if ((pos_pivote - primero + 1) == k)
		return vector[pos_pivote];

	else if (k < (pos_pivote - primero + 1))
		return quick_select(vector, primero, pos_pivote - 1, k);

	return quick_select(vector, pos_pivote + 1, ultimo, k - (pos_pivote - primero + 1));*/

	if (pos_pivote == k)
		return vector[pos_pivote];

	else if (k < pos_pivote)
		return quick_select(vector, primero, pos_pivote - 1, k);

	return quick_select(vector, pos_pivote + 1, ultimo, k);
}

int main(){

	int vector[] = {5,3,2,1,4,6};
	//int vector[] = {5,0,23,3,-6,89};

	//Pruebo para todos los k
	int i;	
	void *vector_aux[20];
	for(i = 0; i < 6; i++){
		vector_aux[i]=(void*)&vector[i];
	}
	for(i = 0; i < 6; i++){
		int fb = fuerza_bruta(vector, 6, i);
		int os = ordenar_y_seleccionar(vector, 6, i);
		int ks = k_selecciones(vector, 6, i);
		int kh = k_heapsort(vector_aux, 6, i);
		int hs = heap_select(vector, 6, i);
		int qs = quick_select(vector, 0, 5, i);

		printf("fb%d: %d \tos%d: %d \tks%d: %d \tkh%d: %d \ths%d: %d \tqs%d: %d \n", i,fb,i,os,i,ks,i,kh,i,hs,i,qs);
	}
	/*
	merge_sort(vector, 6);
	for(i = 0; i < 6; i++){
		
		printf("%d \n", vector[i]);
	}*/

	return 0;
}
