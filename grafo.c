#include <stdbool.h> 
#include <stddef.h>
#include <stdlib.h>
#include "lista.h"
#include "hash.h"
#include <string.h>
#include <stdio.h>
#include "heap.h"
#include "grafo.h"



// Wrapper del destructor del hash
void destructor_hash(void* hash) {
	hash_destruir((hash_t*) hash);
}

// ESTRUCTURA DEL GRAFO
struct _grafo {
	hash_t *nodos;
	size_t cantidad;
};

// -------- PRIMITIVAS DEL GRAFO -------

// Crea un grafo. Devuelve NULL si no fue posible.
// Post: el grafo se creo o se devolvio NULL.
grafo_t *grafo_crear() {
	grafo_t *migrafo = malloc(sizeof(grafo_t));
	if(!migrafo)
		return NULL;
	migrafo->nodos = hash_crear(destructor_hash);
	if(!migrafo->nodos) {
		free(migrafo);
		return NULL;
		}
	migrafo->cantidad = 0;
	return migrafo;
}

// Destruye el grafo recibido.
// Pre: el grafo fue creado.
// Post: la memoria del grafo se libero.
void grafo_destruir(grafo_t *grafo) {
	hash_destruir(grafo->nodos);
	free(grafo);
}

// Agrega un vertice al grafo con la clave recibida, e inicializa
// su lista de caminos de forma vacia. Devuelve true o false si
// pudo o no llevarse a cabo respectivamente.
// Pre: el grafo fue creado.
// Post: se agrego el vertice con la clave, y se devolvio true
// o false en caso contrario.
bool grafo_crear_vertice(grafo_t *grafo, char *clave) {
	// No quiero que se sobrescriba el nodo si ya existia
	if (hash_pertenece(grafo->nodos, clave))
		return false;
	// Creo el minihash para el vertice, con la funcion free, ya que lo
	// que voy a guardar son enteros con los pesos
	hash_t* hash_destinos = hash_crear(free);
	if (!hash_destinos)
		return false;
	if (!hash_guardar(grafo->nodos, clave, hash_destinos)) {
		hash_destruir(hash_destinos);
		return false;
		}
	
	grafo->cantidad++;
	return true;
}

// Crea una arista uniendo dos vertices origen -> destino. Si
// idayvuelta es true, ademas crea la arista en el otro
// sentido (origen <- destino). Devuelve true o false segun
// la funcion haya funcionado o no.
// Pre: el grafo fue creado.
// Post: se creo el camino y se devolvio true o false.
// IMPORTANTE: si ya existía una arista entre ambos nodos se sobrescribe
bool grafo_crear_arista(grafo_t *grafo, char *origen, char *destino, bool idayvuelta, int peso){
	if(!grafo_pertenece(grafo, origen) || (!grafo_pertenece(grafo, destino)))
		return false;//si los nodos no existen, no se puede crear camino
	hash_t* hash_destinos = (hash_t*) hash_obtener(grafo->nodos, origen); 
	// Voy a guardar el destino en el hash_destinos de origen, si ya
	// existia un camino entre ambos nodos se sobrescribe
	// pido dinamicamente memoria para el peso
	int* peso_camino = malloc(sizeof(int));
	*peso_camino = peso;
	hash_guardar(hash_destinos, destino, peso_camino);
	// Hasta aca agregue el camino origen->destino. Me falta ver
	// si tengo que agregar el camino destino->origen
	if(idayvuelta)
		return grafo_crear_arista(grafo, destino, origen, false, peso); 
	return true;
}

// Devuelve true si existe un vertice con la clave recibida.
// Pre: el grafo fue creado, la clave es valida.
// Post: se devolvio true o false segun corresponda.
bool grafo_pertenece(grafo_t *grafo, char* clave) {
	return hash_pertenece((const hash_t*) grafo->nodos, (const char*) clave);
}

// Devuelve true si hay una arista desde nodo origen a destino.
// Pre: el grafo fue creado.
// Post: se devolvio true o false segund corresponda.
// NOTA: un nodo origen es adyacente a un destino, si existe una arista
// que sale de origen y va a destino. Si la arista no existe, la
// funcion devuelve false incluso aunque exista una arista de destino
// a origen.
bool grafo_son_adyacentes(grafo_t *grafo, char *origen, char *destino) {
	if(!grafo_pertenece(grafo, origen) || (!grafo_pertenece(grafo, destino)))
		return false; // Si alguno no existe, no pueden estar conectados
	hash_t* caminos_origen = (hash_t*) hash_obtener(grafo->nodos, origen);
	return hash_pertenece(caminos_origen, destino);
}

// Devuelve el peso de una arista que una origen con destino
// Pre: origen y destino estan conectados
// Post: se devolvio el peso de dicha conexion
int grafo_devolver_peso_arista(grafo_t *grafo, char *origen, char *destino) {
	if(!grafo_son_adyacentes(grafo, origen, destino))
		return -1; // Si no estan conectados, devuelvo -1 por defecto
	hash_t* caminos_origen = (hash_t*) hash_obtener(grafo->nodos, origen);
	return *(int*)hash_obtener(caminos_origen, destino);	
}

// Devuelve true si el grafo esta vacio o false en caso contrario.
// Pre: el grafo fue creado.
bool grafo_esta_vacio(grafo_t *grafo) {
	return !grafo_cantidad(grafo);
}

// Devuelve la cantidad de elementos guardados en el grafo.
// Pre: el grafo fue creado.
// Post: se devolvio la cant. de elem. del grafo.
size_t grafo_cantidad(grafo_t *grafo) {
	return grafo->cantidad;
}

// Devuelve todos los vertices que son adyacentes al
// vertice con la clave recibida.
// Pre: el grafo fue creado.
// Post: se devolvio un hash con todos los adyacentes.
hash_t *grafo_devolver_adyacentes(grafo_t *grafo, char *clave) {
	if(!hash_pertenece((const hash_t*) grafo->nodos, (const char*) clave))
		return NULL;
	return (hash_t*) hash_obtener((const hash_t*) grafo->nodos, (const char*) clave);
}

// Borra un vertice del grafo con todos sus caminos asociados.
// Pre: el grafo fue creado.
// Post: se borro el vertice que tenia a clave.
void grafo_borrar_vertice(grafo_t *grafo, char *clave) {
	if(!grafo_pertenece(grafo, clave))
		return; // Si no existe, no hay nada que borrar

	hash_destruir((hash_t*)hash_borrar(grafo->nodos, clave));
	hash_iter_t* iter_hash = hash_iter_crear(grafo->nodos);
	if (!iter_hash)
		return;
	while (!hash_iter_al_final(iter_hash)) {
		const char* nodo_actual = hash_iter_ver_actual(iter_hash);
		hash_t* hash_caminos = (hash_t*)hash_obtener(grafo->nodos, nodo_actual);
		if (hash_pertenece(hash_caminos, clave));
			free((int*)hash_borrar(hash_caminos, clave));
		hash_iter_avanzar(iter_hash);
	}
	hash_iter_destruir(iter_hash);
	grafo->cantidad--;
}

// Borra la arista que va de origen a destino.
// Pre: el grafo fue creado.
// Post: se borro el camino.
// NOTA: solo se borra la arista que va de origen a destino, si existe
// y no el que va de destino a origen
void grafo_borrar_arista(grafo_t *grafo, char *origen, char *destino) {
	if(!grafo_pertenece(grafo, origen) || (!grafo_pertenece(grafo, destino)))
		return; // Si alguno no existe, no puede haber camino que borrar
	hash_t* caminos_origen = (hash_t*) hash_obtener(grafo->nodos, origen);
	if (hash_pertenece(caminos_origen, destino)) {
		free((int*)hash_borrar(caminos_origen, destino));
	}
}

// Devuelve el hash con todas las conexiones del grafo.
// Pre: el grafo fue creado.
// Post: se devolvio el hash de todas las conexiones del grafo.
hash_t* grafo_devolver_conexiones(grafo_t* grafo) {
	return grafo->nodos;
}

// Imprime el grafo.
// Pre: el grafo fue creado.
void grafo_imprimir(grafo_t *grafo) {
	
	printf("GRAFO CANT: %zu\n", grafo_cantidad(grafo));
	
	hash_iter_t* iter_hash = hash_iter_crear(grafo->nodos);
	if (!iter_hash) return;

	// Primero itero sobre cada clave del hash... cada vertice
	while (!hash_iter_al_final(iter_hash)) {
		char* clave_vertice = (char*)hash_iter_ver_actual(iter_hash);
		printf("Vertice %s. Conectado a:\n",clave_vertice);
		hash_t* caminos_origen = (hash_t*)hash_obtener(grafo->nodos, clave_vertice);
		hash_iter_t* iter_minihash = hash_iter_crear(caminos_origen);
		if (!iter_minihash) {
			hash_iter_destruir(iter_hash);
			return;
		}
		// Creo un iterador en cada minihash e imprimo todas las aritas
		while (!hash_iter_al_final(iter_minihash)) {
			const char* clave_destino = hash_iter_ver_actual(iter_minihash);
			int* peso_destino = (int*)hash_obtener(caminos_origen, clave_destino);
			printf("\t\t\t%s(%d)\n ", clave_destino, *peso_destino);
			hash_iter_avanzar(iter_minihash);
		}
		printf("\n");
		hash_iter_destruir(iter_minihash);
		hash_iter_avanzar(iter_hash);
	}
	hash_iter_destruir(iter_hash);
}