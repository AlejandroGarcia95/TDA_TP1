#include <stdbool.h> 
#include <stddef.h>
#include <stdlib.h>
#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <string.h>
#include "heap.h"
#include "path_finder.h"
#define TRUE 1
#define FALSE 0

/// El TDA path_finder utiliza cuatro primitivas de recorrido para
/// buscar el mínimo camino entre un vértice origen y un vértice
/// destino en un grafo: Dijkstra, BFS, heurística y A*.
/// Las primitivas de recorrido ejecutan la búsqueda de este camino,
/// comunicando si el resultado fue positivo o negativo.
/// Para tener acceso al camino resultante, o a otra información 
/// relevante del recorrido se utilizan las primitivas de resultados.

// Estructura del path_finder
struct _pathfinder{
	hash_t* nodos_visitados;
	hash_t* distancias;
	hash_t* padres;
	lista_t* camino_final;

	// Falta puntero a función heurística sdalfjhsaf
};

// FUNCIONES PRIVADAS

// Crea los campos de nodos_visitados, distancias, padres y
// camino_final del path_finder con memoria dinámica. Devuelve
// TRUE si la operación fue exitosa o FALSE en caso contrario.
// Pre: el path_finder fue creado.
bool inicializar_pathfinder(path_finder_t* pf){
	pf->distancias = hash_crear(free);
	if(!pf->distancias){
		free(pf);
		return FALSE;
		}
	pf->nodos_visitados = hash_crear(free);
	if(!pf->nodos_visitados){
		free(pf);
		hash_destruir(pf->distancias);
		return FALSE;
		}
	pf->padres = hash_crear(free);
	if(!pf->nodos_visitados){
		free(pf);
		hash_destruir(pf->distancias);
		hash_destruir(pf->nodos_visitados);
		return FALSE;
		}
	pf->camino_final = lista_crear();
	if(!pf->camino_final){
		free(pf);
		hash_destruir(pf->distancias);
		hash_destruir(pf->padres);
		hash_destruir(pf->nodos_visitados);
		return FALSE;
		}				
	return TRUE;
}

// Borra los campos de nodos_visitados, distancias, padres y
// camino_final del path_finder.
// Pre: el path_finder fue creado.
void vaciar_pathfinder(path_finder_t* pf){
		hash_destruir(pf->distancias);
		hash_destruir(pf->padres);
		hash_destruir(pf->nodos_visitados);
		lista_destruir(pf->camino_final, NULL);
}
// -------- PRIMITIVAS DEL PATHFINDER -------

// Crea un path_finder. Devuelve NULL si no fue posible.
// Post: el path_finder se creó o se devolvió NULL.
path_finder_t* path_finder_crear(){
	path_finder_t* pf = malloc(sizeof(path_finder_t));
	if(!pf)
		return NULL;
	
	if(inicializar_pathfinder(pf))
		return pf;

	return NULL;
}

// Destruye el path_finder recibido.
// Pre: el path_finder fue creado.
// Post: la memoria del path_finder se liberó.
void path_finder_destruir(path_finder_t* pf){
	vaciar_pathfinder(pf);
	free(pf);
}

// -------- PRIMITIVAS DE RECORRIDO ---------

// Busca el camino más corto del vertice origen al vértice destino del
// grafo recibido usando el algoritmo de Dijkstra. Devuelve FALSE si 
// no se pudo encontrar ningún camino o si alguno de los parámetros 
// recibidos era inválido, o TRUE si pudo
// encontrarlo.
// Pre: el grafo y el path_finder fueron creados, origen y destino son
// nodos válidos del grafo.
// Post: se encontró un camino válido y se devolvió TRUE o FALSE en caso
// contrario. Los campos de vértices visitados, padres y distancias
// fueron actualizados, y el camino encontrado también.
bool path_finder_buscar_dijkstra(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino){
	return FALSE;	
}

// Busca el camino más corto del vertice origen al vértice destino del
// grafo recibido usando el algoritmo de BFS. Devuelve FALSE si 
// no se pudo encontrar ningún camino o si alguno de los parámetros 
// recibidos era inválido, o TRUE si pudo
// encontrarlo.
// Pre: el grafo y el path_finder fueron creados, origen y destino son
// nodos válidos del grafo.
// Post: se encontró un camino válido y se devolvió TRUE o FALSE en caso
// contrario. Los campos de vértices visitados, padres y distancias
// fueron actualizados, y el camino encontrado también.
bool path_finder_buscar_bfs(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino){
	return FALSE;	
}

// Busca el camino más corto del vertice origen al vértice destino del
// grafo recibido usando la función heurística. Devuelve FALSE si 
// no se pudo encontrar ningún camino o si alguno de los parámetros 
// recibidos era inválido, o TRUE si pudo
// encontrarlo.
// Pre: el grafo y el path_finder fueron creados, origen y destino son
// nodos válidos del grafo.
// Post: se encontró un camino válido y se devolvió TRUE o FALSE en caso
// contrario. Los campos de vértices visitados, padres y distancias
// fueron actualizados, y el camino encontrado también.
bool path_finder_buscar_heuristica(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino){
	return FALSE;	
}

// Busca el camino más corto del vertice origen al vértice destino del
// grafo recibido usando el algoritmo A*. Devuelve FALSE si 
// no se pudo encontrar ningún camino o si alguno de los parámetros 
// recibidos era inválido, o TRUE si pudo
// encontrarlo.
// Pre: el grafo y el path_finder fueron creados, origen y destino son
// nodos válidos del grafo.
// Post: se encontró un camino válido y se devolvió TRUE o FALSE en caso
// contrario. Los campos de vértices visitados, padres y distancias
// fueron actualizados, y el camino encontrado también.
bool path_finder_buscar_a_star(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino){
	return FALSE;	
}

// -------- PRIMITIVAS DE RESULTADO ---------

// Devuelve TRUE si el vértice recibido fue visitado en la búsqueda
// del último camino mínimo, o FALSE en caso contrario. Si el
// path_finder aún no realizó ninguna búsqueda, devuelve FALSE por
// defecto.
// Pre: el path_finder fue creado.
// Post: se devolvió TRUE o FALSE según corresponda.
bool path_finder_fue_visitado(path_finder_t* pf, char* vertice){
	return FALSE;
}

// Devuelve la distancia hacia el vértice recibido según la última
// búsqueda realizada. Si el path_finder aún no realizó ninguna búsqueda
// o si no se visitó el vértice, devuelve INFINITO.
// Pre: el path_finder fue creado.
// Post: se devolvió la distancia hacia el vértice recibido o INFINITO
// según corresponda.
int path_finder_distancia(path_finder_t* pf, char* vertice){
	return INFINITO;
}

// Devuelve una lista con las claves en orden de los vértices visitados
// durante el último camino, comenzando por el vértice origen y como
// último vértice en la lista al destino. Devuelve NULL si el
// path_finder aún no realizó ninguna búsqueda.
// Pre: el path_finder fue creado.
// Post: se devolvió la lista con claves de vértices en orden o NULL
// según corresponda
// NOTA: la lista resultante es de sólo consulta y no debe destruirse
// dinámicamente.
lista_t* path_finder_camino(path_finder_t* pf){
	return NULL;
}

