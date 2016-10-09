#ifndef _ESTADISTICO_H
#define _ESTADISTICO_H

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "hash.h"

/*** El estadístico de orden k de un conjunto de n elementos es el el k-ésimo elemento más pequeño.
 *** Se realizaron los siguientes algoritmos que computan este valor. ***/

/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Itera todos los elementos del conjunto y verifica de a uno si es la solución. 
 * Cuendo el verificador devuelve true, devuelve ese elemento. */
int fuerza_bruta(int vector[], int n, int k, bool sin_repetidos);

/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Ordena el conjunto mediante MergeSort y luego selecciona el k elemento del arreglo
 * ordenado. */
int ordenar_y_seleccionar(int vector[], int n, int k, bool sin_repetidos);

/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Se realizan k-selecciones para encontrar el k elemento más pequeño. */
int k_selecciones(int vector[], int n, int k, bool sin_repetidos);

/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Realiza k-extracciones a un arreglo con la propiedad de heap. */
int k_heapsort(int vector[], int n, int k, bool sin_repetidos);

/* Recibe un vector, su largo n, el estadístico de orden k, y un booleano indicando
 * si se consideran los elementos repetidos en el orden k.
 * Se usa un heap para almacenar los k elementos más chicos, intercambiándolos cuando
 * sea necesario. */
int heap_select(int vector[], int n, int k, bool sin_repetidos);

/* Recibe un vector, sus índices (primero y ultimo), el estadístico de orden k, y un 
 * booleano indicando si se consideran los elementos repetidos en el orden k.
 * Se utiliza una estrategia de división y conquista similar a la de quicksort pero 
 * descartando las divisiones que sabemos que no incluyen al k buscado. */
int quick_select(int vector[], int primero, int ultimo, int k, bool sin_repetidos);

#endif // _ESTADISTICO_H
