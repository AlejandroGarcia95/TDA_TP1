#ifndef _PATHFINDER_H
#define _PATHFINDER_H

#include <stdbool.h> 
#include <stddef.h>
#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <string.h>
#include "heap.h"

#define INFINITO 65500

/// El TDA path_finder utiliza cuatro primitivas de recorrido para
/// buscar el mínimo camino entre un vértice origen y un vértice
/// destino en un grafo: Dijkstra, BFS, heurística y A*.
/// Las primitivas de recorrido ejecutan la búsqueda de este camino,
/// comunicando si el resultado fue positivo o negativo.
/// Para tener acceso al camino resultante, o a otra información 
/// relevante del recorrido se utilizan las primitivas de resultados.

// Estructura del path_finder
typedef struct _pathfinder path_finder_t;

typedef int (*heur_f) (char *origen, char *destino);

// -------- PRIMITIVAS DEL PATHFINDER -------

// Crea un path_finder. Devuelve NULL si no fue posible.
// Post: el path_finder se creó o se devolvió NULL.
path_finder_t* path_finder_crear(heur_f heuristica);

// Destruye el path_finder recibido.
// Pre: el path_finder fue creado.
// Post: la memoria del path_finder se liberó.
void path_finder_destruir(path_finder_t* pf);

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
bool path_finder_buscar_dijkstra(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino);

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
bool path_finder_buscar_bfs(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino);

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
bool path_finder_buscar_heuristica(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino);

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
bool path_finder_buscar_a_star(path_finder_t* pf, grafo_t* grafo, char *origen, char *destino);

// -------- PRIMITIVAS DE RESULTADO ---------

// Devuelve TRUE si el vértice recibido fue visitado en la búsqueda
// del último camino mínimo, o FALSE en caso contrario. Si el
// path_finder aún no realizó ninguna búsqueda, devuelve FALSE por
// defecto.
// Pre: el path_finder fue creado.
// Post: se devolvió TRUE o FALSE según corresponda.
bool path_finder_fue_visitado(path_finder_t* pf, char* vertice);

// Devuelve la distancia hacia el vértice recibido según la última
// búsqueda realizada. Si el path_finder aún no realizó ninguna búsqueda
// o si no se visitó el vértice, devuelve INFINITO.
// Pre: el path_finder fue creado.
// Post: se devolvió la distancia hacia el vértice recibido o INFINITO
// según corresponda.
int path_finder_distancia(path_finder_t* pf, char* vertice);

// Devuelve una lista con las claves en orden de los vértices visitados
// durante el último camino, comenzando por el vértice origen y como
// último vértice en la lista al destino. Devuelve NULL si el
// path_finder aún no realizó ninguna búsqueda.
// Pre: el path_finder fue creado.
// Post: se devolvió la lista con claves de vértices en orden o NULL
// según corresponda
// NOTA: la lista resultante es de sólo consulta y no debe destruirse
// dinámicamente.
lista_t* path_finder_camino(path_finder_t* pf);

// Devuelve la longitud del camino encontrado por el último recorrido.
// Si no se encontró ningún camino, devuelve INFINITO.
// Pre: el path_finder fue creado.
// Post: se devolvió la longitud del último camino, calculada como
// la suma de los pesos de todas las aristas involucradas en el mismo.
int path_finder_longitud_camino(path_finder_t* pf);

#endif // _PATHFINDER_H
