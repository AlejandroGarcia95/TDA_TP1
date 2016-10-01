#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "heap.h"
#include "path_finder.h"

#define FALSE 0
#define TRUE 0


/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result) {
	printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void destruir_arreglo_claves(char** arr, size_t cantidad) {
	for (int i = 0; i < cantidad; i++) {
		free(arr[i]);
	}
	free(arr);
}

char** generar_claves_rnd(size_t cantidad, size_t longitud) {
	char** arr_claves = malloc(sizeof(char*) * cantidad);
	if (!arr_claves) return NULL;
	srand(time(NULL));
	for (int i = 0; i < cantidad; i++) {
		char* cadenita = malloc(sizeof(char) * (longitud+1));
		if (!cadenita){
			destruir_arreglo_claves(arr_claves, cantidad);
			return NULL;
		}
		for (int j = 0; j < longitud; j++) {
			cadenita[j] = 'A' + rand()%32;
		}
		cadenita[longitud] = '\0';
		arr_claves[i] = cadenita;		
	}
	return arr_claves;
}


void prueba_volumen(int cantidad) {
	char** claves = generar_claves_rnd(cantidad, 8);
	if (!claves) return;
	srand(time(NULL));
	bool ok = true;
	size_t pos1 = 0;
	size_t pos2 = 0;
	grafo_t *ng = grafo_crear();
	print_test("Se creo el grafo", ng);
	while(ok && grafo_cantidad(ng) < cantidad) {
		pos1 = rand()%cantidad;
		if ((!grafo_pertenece(ng, claves[pos1]) && (!grafo_crear_vertice(ng, claves[pos1]))))
			ok = false;
	}
	print_test("Cantidad es correcta", grafo_cantidad(ng) && ok);
	for(int i=0; i < (cantidad*2); i++) {
		pos1 = rand()%cantidad;		
		pos2 = rand()%cantidad;
		size_t peso = 1;//rand()%100;		
		grafo_crear_arista(ng, claves[pos1], claves[pos2], true, peso);
	}
	grafo_imprimir(ng);



	grafo_destruir(ng);
	destruir_arreglo_claves(claves, cantidad);
}


/* Test del digrafo siguiendo el ejemplo de la imagen GrafoEjemplo.png*/
void prueba_std() {
	grafo_t *nuestro_grafo = grafo_crear();
	print_test("Se creo el grafo", nuestro_grafo);
	print_test("El grafo esta vacio", grafo_esta_vacio(nuestro_grafo));
	
	// Creo los vértices del grafo
	grafo_crear_vertice(nuestro_grafo, "1");
	grafo_crear_vertice(nuestro_grafo, "2");
	grafo_crear_vertice(nuestro_grafo, "3");
	grafo_crear_vertice(nuestro_grafo, "4");
	grafo_crear_vertice(nuestro_grafo, "5");
	grafo_crear_vertice(nuestro_grafo, "6");
	grafo_crear_vertice(nuestro_grafo, "7");
	grafo_crear_vertice(nuestro_grafo, "8");
	grafo_crear_vertice(nuestro_grafo, "9");
	grafo_crear_vertice(nuestro_grafo, "10");
	
	print_test("Cantidad de vertices correcta", grafo_cantidad(nuestro_grafo) == 10);
	print_test("Vertice correcto pertenece", grafo_pertenece(nuestro_grafo, "6"));
	print_test("Vertice correcto pertenece", grafo_pertenece(nuestro_grafo, "2"));
	print_test("Vertice incorrecto no pertenece", !grafo_pertenece(nuestro_grafo, "19"));
	print_test("Vertice incorrecto no pertenece", !grafo_pertenece(nuestro_grafo, "F"));
	
	// Creo las aristas 
	grafo_crear_arista(nuestro_grafo, "1", "4", FALSE, 20);
	grafo_crear_arista(nuestro_grafo, "1", "5", FALSE, 20);
	grafo_crear_arista(nuestro_grafo, "1", "6", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "1", "7", FALSE, 15);
	grafo_crear_arista(nuestro_grafo, "2", "4", FALSE, 10);
	grafo_crear_arista(nuestro_grafo, "2", "3", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "3", "2", FALSE, 15);
	grafo_crear_arista(nuestro_grafo, "1", "2", FALSE, 10);
	grafo_crear_arista(nuestro_grafo, "8", "1", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "8", "2", FALSE, 20);
	grafo_crear_arista(nuestro_grafo, "8", "7", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "7", "6", FALSE, 10);
	grafo_crear_arista(nuestro_grafo, "5", "6", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "4", "5", FALSE, 10);
	grafo_crear_arista(nuestro_grafo, "3", "4", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "10", "3", FALSE, 15);
	grafo_crear_arista(nuestro_grafo, "10", "2", FALSE, 5);
	grafo_crear_arista(nuestro_grafo, "9", "2", FALSE, 15);
	grafo_crear_arista(nuestro_grafo, "9", "10", FALSE, 10);
	grafo_crear_arista(nuestro_grafo, "9", "8", FALSE, 20);

	print_test("Peso correcto de arista",  grafo_devolver_peso_arista(nuestro_grafo, "1", "5")==20);
	print_test("Peso correcto de arista",  grafo_devolver_peso_arista(nuestro_grafo, "2", "3")==5);
	print_test("Peso correcto de arista",  grafo_devolver_peso_arista(nuestro_grafo, "3", "2")==15);
	print_test("Peso correcto de arista",  grafo_devolver_peso_arista(nuestro_grafo, "8", "7")==5);
	print_test("Peso correcto de arista inexistente",  grafo_devolver_peso_arista(nuestro_grafo, "2", "6")==-1);
	
	print_test("Vertices adyacentes lo son", grafo_son_adyacentes(nuestro_grafo, "1", "6"));
	print_test("Vertices adyacentes lo son", grafo_son_adyacentes(nuestro_grafo, "4", "5"));
	print_test("Vertices adyacentes lo son", grafo_son_adyacentes(nuestro_grafo, "10", "2"));
	print_test("Vertices no adyacentes no lo son", !grafo_son_adyacentes(nuestro_grafo, "1", "8"));
	print_test("Vertices no adyacentes no lo son", !grafo_son_adyacentes(nuestro_grafo, "4", "6"));

	hash_t* ady = grafo_devolver_adyacentes(nuestro_grafo, "1");
	print_test("Cantidad de adyacentes de vertice es correcta", hash_cantidad(ady) == 5);
	ady = grafo_devolver_adyacentes(nuestro_grafo, "5");
	print_test("Cantidad de adyacentes de vertice es correcta", hash_cantidad(ady) == 1);
	ady = grafo_devolver_adyacentes(nuestro_grafo, "6");
	print_test("Cantidad de adyacentes de vertice es correcta", hash_cantidad(ady) == 0);
/*
	print_test("Vertices adyacentes antes de borrado", grafo_son_adyacentes(nuestro_grafo, "1", "4"));
	grafo_borrar_arista(nuestro_grafo, "1", "4");
	print_test("Vertices no adyacentes antes de borrado", !grafo_son_adyacentes(nuestro_grafo, "1", "4"));
	ady = grafo_devolver_adyacentes(nuestro_grafo, "1");
	print_test("Cantidad de adyacentes post borrado es correcta", hash_cantidad(ady) == 4);
	
	grafo_borrar_vertice(nuestro_grafo, "1");
	print_test("Cantidad de vertices correcta", grafo_cantidad(nuestro_grafo) == 9);
	print_test("Vertice correcto pertenece", grafo_pertenece(nuestro_grafo, "6"));
	print_test("Vertice correcto pertenece", !grafo_pertenece(nuestro_grafo, "1"));
	print_test("Vertices no adyacentes no lo son", !grafo_son_adyacentes(nuestro_grafo, "1", "6"));
	*/
	

	
	
	path_finder_t* pf = path_finder_crear();
	
	


	path_finder_buscar_dijkstra(pf, nuestro_grafo, "9", "6");


	lista_t* vertices = path_finder_camino(pf);
	printf("CAMINO FINAL: \n");
	lista_iter_t* it = lista_iter_crear(vertices);
	while(!lista_iter_al_final(it)){
		printf("%s\n", (char*)lista_iter_ver_actual(it));
		lista_iter_avanzar(it);
		}
	lista_iter_destruir(it);
	
	
	path_finder_destruir(pf);
	
	grafo_destruir(nuestro_grafo);
}


int compararP(const void *a, const void *b){
	int real_a = ((int)a);
	int real_b = ((int)b);
	return real_b-real_a;
}

int main() {
	prueba_std();
//	prueba_volumen(100);
	
	// Intentamos crear heap
	
	return 0;
}
