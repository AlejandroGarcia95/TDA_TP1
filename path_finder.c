#include <stdbool.h> 
#include <stddef.h>
#include <stdlib.h>
#include "lista.h"
#include "cola.h"
#include "hash.h"
#include "grafo.h"
#include <string.h>
#include "heap.h"
#include "path_finder.h"
#define TRUE 1
#define FALSE 0


#include <stdio.h>

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
	int longitud_final;
	heur_f h;
	// Falta puntero a función heurística sdalfjhsaf
};


// Estructura auxiliar para guardar información de los vértices en
// cada uno de los recorridos
struct vert_data{
	char* vertice;
	int distancia;
	char* padre;
};

// FUNCIONES PRIVADAS

// Destructor de vert_data adaptado para el heap
void vert_data_destruir(void *e){
	struct vert_data* vActual = (struct vert_data*) e;
	if(vActual->padre)
		free(vActual->padre);
	free(vActual);
}

struct vert_data* vert_data_crear(char* vert, int dist, char* padre){
	struct vert_data* vd = malloc(sizeof(struct vert_data));
	if(!vd)
		return NULL;
	vd->vertice = vert;
	vd->distancia = dist;
	vd->padre = (char*)malloc(sizeof(char)*(strlen(padre)+1));
	if(!vd->padre){
		free(vd);
		return NULL;
		}
	strcpy(vd->padre, padre);
	return vd;
}

/* Prototipo de función de comparación que se le pasa como parámetro a las
 * diversas funciones del heap.
 * Debe recibir dos punteros del tipo de dato utilizado en el heap, y
 * debe devolver:
 *   menor a 0  si  a < b
 *       0      si  a == b
 *   mayor a 0  si  a > b */
int comparar(const void *a, const void *b){
	struct vert_data* real_a = (struct vert_data*)a;
	struct vert_data* real_b = (struct vert_data*)b;
	return real_b->distancia -real_a->distancia;
}

// Crea los campos de nodos_visitados, distancias, padres y
// camino_final del path_finder con memoria dinámica. Devuelve
// TRUE si la operación fue exitosa o FALSE en caso contrario.
// Pre: el path_finder fue creado.
bool inicializar_pathfinder(path_finder_t* pf){
	pf->distancias = hash_crear(NULL);
	if(!pf->distancias){
		free(pf);
		return FALSE;
		}
	pf->nodos_visitados = hash_crear(NULL);
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
	pf->longitud_final = INFINITO;				
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

// Inicializa las distancias, padres y visitados de todos los
// vértices de un grafo antes de un determinado recorrido.
// Los recorridos que la usen deben invocarla antes de iniciar
// Pre: la lista de vértices posee a todos los vértices del grafo
// Post: se inicializaron para todos los vértices los campos de
// visitado, padre y distancia en FALSE, NULL, INFINITO en las
// estructuras internas del path_finder
bool inicializar_recorrido(path_finder_t* pf, lista_t* vertices){
	lista_iter_t *l_it = lista_iter_crear(vertices);
	if(!l_it)	return FALSE;
	while(!lista_iter_al_final(l_it)){
		char* v = (char*) lista_iter_ver_actual(l_it);			
		hash_guardar(pf->nodos_visitados, v, (void*)FALSE);
		hash_guardar(pf->distancias, v, (void*)INFINITO);
		hash_guardar(pf->padres, v, NULL);
		lista_iter_avanzar(l_it);
		}
	lista_iter_destruir(l_it);
	return TRUE;
}

// Genera la lista del camino final encontrado por la última búsqueda
// de recorrido, a partir de un par de vértices destino y origen.
// Devuelve TRUE si fue posible generar esta lista o FALSE en caso
// contrario
// Pre: el pf fue creado, origen y destino son nodos válidos
// Post: se guardo en pf->camino_final una lista de los vértices en
// el orden del recorrido correcto
bool generar_camino_final(path_finder_t* pf, grafo_t* grafo, char* origen, char* destino){
	char* vHijo = destino;
	int longitud = 0;
	while(strcmp(vHijo, origen)){
		lista_insertar_primero(pf->camino_final, (void*) vHijo);
		char* vPadre = (char*)hash_obtener(pf->padres, vHijo);
		if(!vPadre){
			lista_borrar_primero(pf->camino_final);
			return FALSE;
			}	
		else{
			longitud += grafo_devolver_peso_arista(grafo, vPadre, vHijo);
			vHijo = vPadre;
			}
		}
	pf->longitud_final = longitud;
	lista_insertar_primero(pf->camino_final, (void*) origen);
	return TRUE;
}

// Función encargada de realizar un recorrido en el grafo desde el
// nodo origen al nodo destino, buscando el mínimo camino. Para ello,
// se itera, y en cada iteración se visita al nodo adyacente al conjunto
// de vértices ya visitados con menor distancia. 
// Para poder generar los recorridos según el algoritmo de Dijkstra, BFS,
// A* ó con función heurística, este algoritmo parametriza la distancia
// de cada nodo de la siguiente forma:
// dist(v) = dist(w) + A*peso(w, v) + B*heuristica(v, destino) + C
// donde w es el padre de v, y A, B, C son ctes recibidas por parámetro.
// De esta forma, se observan los algoritmos mencionados como casos 
// particulares de esta parametrización:
// Dijkstra: A = 1, B = C = 0
// BFS: A = B = 0, C = 1
// Heurística: A = C = 0, B = 1
// A*: A = B = 1, C = 0
// Pre: el grafo y el path_finder fueron creados, origen y destino son
// nodos válidos del grafo.
// Post: se encontró un camino válido y se devolvió TRUE o FALSE en caso
// contrario. Los campos de vértices visitados, padres y distancias
// fueron actualizados, y el camino encontrado también. 
bool ejecutar_recorrido(path_finder_t* pf, grafo_t* grafo, char *origen, 
						char *destino, float a, float b, float c){
	vaciar_pathfinder(pf);
	inicializar_pathfinder(pf);
	// Inicializo distancias, padres y visitados
	lista_t* vertices = grafo_vertices(grafo);
	if(!vertices) return FALSE;
	if(!inicializar_recorrido(pf, vertices)){
		lista_destruir(vertices, NULL);
		return FALSE;
		}
	// Inicializo la distancia y el visitado para el origen
	hash_guardar(pf->distancias, origen, 0);
	//hash_guardar(pf->nodos_visitados, origen, (void*)TRUE);
	
	int cantVisitados = 0;
	heap_t* q = heap_crear(comparar);
	if(!q){
		lista_destruir(vertices, NULL);
		return FALSE;
		}
	struct vert_data* inicial = vert_data_crear(origen, 0, origen);
	if(!inicial){
		lista_destruir(vertices, NULL);
		heap_destruir(q, NULL);
		return FALSE;
		}
	// Encolo nodo inicial
	heap_encolar(q, (void*) inicial);
	
	while((!heap_esta_vacio(q)) && (cantVisitados < lista_largo(vertices)) && (!path_finder_fue_visitado(pf, destino))){
		struct vert_data* vActual = heap_desencolar(q);
		if(!path_finder_fue_visitado(pf, vActual->vertice)){
			cantVisitados++;
			hash_guardar(pf->distancias, vActual->vertice, (void*)vActual->distancia);
			char* cadenita = (char*)malloc(sizeof(char)*(strlen(vActual->padre)+1));
			strcpy(cadenita, vActual->padre);
			hash_guardar(pf->padres, vActual->vertice, (void*) cadenita);
			hash_guardar(pf->nodos_visitados, vActual->vertice, (void*)TRUE);
			hash_t* ady = grafo_devolver_adyacentes(grafo, vActual->vertice);
			hash_iter_t* h_it = hash_iter_crear(ady);
			if(!h_it){
				heap_destruir(q, vert_data_destruir);
				lista_destruir(vertices, NULL);
				return FALSE;
				}
			while(!hash_iter_al_final(h_it)){ // for w adyacente a v
				char* w = (char*) hash_iter_ver_actual(h_it); 
				if(!path_finder_fue_visitado(pf, w)){
						int ld = vActual->distancia + c;
						ld += a * grafo_devolver_peso_arista(grafo, vActual->vertice, w);
						//ld += b * pf->h(vActual->vertice, w);
						ld += b * pf->h(w, destino);
						struct vert_data* wAdy = vert_data_crear(w, ld, vActual->vertice);
						if(!wAdy){
							lista_destruir(vertices, NULL);
							heap_destruir(q, free);
							hash_iter_destruir(h_it);
							return FALSE;
							}					
						// Encolo w adyacente
						heap_encolar(q, (void*) wAdy);
					}
				hash_iter_avanzar(h_it);
				}
			hash_iter_destruir(h_it);
			}
		vert_data_destruir((void*)vActual);
		}
	
	// Acá encontré un camino => Lo guardo en la lista de camino
	if(!generar_camino_final(pf, grafo, origen, destino)){
		heap_destruir(q, vert_data_destruir);
		lista_destruir(vertices, NULL);
		return FALSE;
		}	
			
	heap_destruir(q, vert_data_destruir);
	lista_destruir(vertices, NULL);
	
	
	return TRUE;
}

// -------- PRIMITIVAS DEL PATHFINDER -------

// Crea un path_finder. Devuelve NULL si no fue posible.
// Post: el path_finder se creó o se devolvió NULL.
path_finder_t* path_finder_crear(heur_f heuristica){
	path_finder_t* pf = malloc(sizeof(path_finder_t));
	if(!pf)
		return NULL;
	if(inicializar_pathfinder(pf)){
		pf->h = heuristica;
		return pf;
		}
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
	return ejecutar_recorrido(pf, grafo, origen, destino, 1, 0, 0);
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
	vaciar_pathfinder(pf);
	inicializar_pathfinder(pf);
	// Inicializo distancias, padres y visitados
	lista_t* vertices = grafo_vertices(grafo);
	if(!vertices) return FALSE;
	if(!inicializar_recorrido(pf, vertices)){
		lista_destruir(vertices, NULL);
		return FALSE;
		}
	// Inicializo la distancia y el visitado para el origen
	hash_guardar(pf->distancias, origen, 0);
	hash_guardar(pf->nodos_visitados, origen, (void*)TRUE);
	
	int cantVisitados = 0;
	cola_t* q = cola_crear();
	if(!q) {
		lista_destruir(vertices, NULL);
		return FALSE;
		}
		
	struct vert_data* inicial = vert_data_crear(origen, 0, origen);
	if(!inicial){
		lista_destruir(vertices, NULL);
		cola_destruir(q, NULL);
		return FALSE;
		}
	cola_encolar(q, (void*)inicial);
		
	while((!cola_esta_vacia(q)) && (cantVisitados < lista_largo(vertices))){
		struct vert_data* vActual = cola_desencolar(q);
		cantVisitados++;
		hash_guardar(pf->distancias, vActual->vertice, (void*)vActual->distancia);
		char* cadenita = (char*)malloc(sizeof(char)*(strlen(vActual->padre)+1));
		strcpy(cadenita, vActual->padre);
		hash_guardar(pf->padres, vActual->vertice, (void*) cadenita);
		
		if(!path_finder_fue_visitado(pf, destino)){
			hash_t* ady = grafo_devolver_adyacentes(grafo, vActual->vertice);
			hash_iter_t* h_it = hash_iter_crear(ady);
			if(!h_it){
				cola_destruir(q, vert_data_destruir);
				lista_destruir(vertices, NULL);
				return FALSE;
				}
			while(!hash_iter_al_final(h_it)){ // for w adyacente a v
				char* w = (char*) hash_iter_ver_actual(h_it); 
				if(!path_finder_fue_visitado(pf, w)){
						hash_guardar(pf->nodos_visitados, w, (void*)TRUE);
						int ld = vActual->distancia + 1;
						struct vert_data* wAdy = vert_data_crear(w, ld, vActual->vertice);
						if(!wAdy){
							lista_destruir(vertices, NULL);
							cola_destruir(q, free);
							hash_iter_destruir(h_it);
							return FALSE;
							}					
						// Encolo w adyacente
						cola_encolar(q, (void*) wAdy);
					}
				hash_iter_avanzar(h_it);
				}
			hash_iter_destruir(h_it);
			}
		vert_data_destruir((void*)vActual);
		}	
	// Acá encontré un camino => Lo guardo en la lista de camino
	if(!generar_camino_final(pf, grafo, origen, destino)){
		cola_destruir(q, vert_data_destruir);
		lista_destruir(vertices, NULL);
		return FALSE;
		}	
	
	cola_destruir(q, vert_data_destruir);
	lista_destruir(vertices, NULL);
	return TRUE;
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
	return ejecutar_recorrido(pf, grafo, origen, destino, 0, 1, 0);
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
	return ejecutar_recorrido(pf, grafo, origen, destino, 1, 1, 0);
}

// -------- PRIMITIVAS DE RESULTADO ---------

// Devuelve TRUE si el vértice recibido fue visitado en la búsqueda
// del último camino mínimo, o FALSE en caso contrario. Si el
// path_finder aún no realizó ninguna búsqueda, devuelve FALSE por
// defecto.
// Pre: el path_finder fue creado.
// Post: se devolvió TRUE o FALSE según corresponda.
bool path_finder_fue_visitado(path_finder_t* pf, char* vertice){
	if(!hash_pertenece(pf->nodos_visitados, vertice))
		return FALSE;
	return hash_obtener(pf->nodos_visitados, vertice);
}

// Devuelve la distancia hacia el vértice recibido según la última
// búsqueda realizada. Si el path_finder aún no realizó ninguna búsqueda
// o si no se visitó el vértice, devuelve INFINITO.
// Pre: el path_finder fue creado.
// Post: se devolvió la distancia hacia el vértice recibido o INFINITO
// según corresponda.
int path_finder_distancia(path_finder_t* pf, char* vertice){
	if(!hash_pertenece(pf->distancias, vertice))
		return INFINITO;
	return (int)hash_obtener(pf->distancias, vertice);
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
	return pf->camino_final;
}

// Devuelve la longitud del camino encontrado por el último recorrido.
// Si no se encontró ningún camino, devuelve INFINITO.
// Pre: el path_finder fue creado.
// Post: se devolvió la longitud del último camino, calculada como
// la suma de los pesos de todas las aristas involucradas en el mismo.
int path_finder_longitud_camino(path_finder_t* pf){
	return pf->longitud_final;
}
