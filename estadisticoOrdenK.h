#ifndef _ESTADISTICO_H
#define _ESTADISTICO_H

#include <stdio.h>
#include <stdlib.h>
#include "heap.h"



int fuerza_bruta(int vector[], int n, int k);
int k_selecciones(int vector[], int n, int k);
int ordenar_y_seleccionar(int vector[], int n, int k);
int k_heapsort(void *vector[], int n, int k);
int heap_select(int vector[], int n, int k);
int quick_select(int vector[], int primero, int ultimo, int k);

#endif // _ESTADISTICO_H
