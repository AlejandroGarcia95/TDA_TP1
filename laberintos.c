#include "lista.h"
#include "hash.h"
#include "grafo.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
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

/* Función heurística usada para los recorridos. Esta heurística
 devuelve la distancia hamiltoniana entre dos nodos */
int superheuristica(char *origen, char* destino){
	char* pc = &origen[1];
	int xi = (int) strtod(pc, &pc);
	pc += 2; 
	int yi = (int) strtod(pc, NULL);
	pc = &destino[1];
	int xf = (int) strtod(pc, &pc);
	pc += 2; 
	int yf = (int) strtod(pc, NULL);
	int delta_y = yf - yi;
	if(delta_y < 0)
		delta_y *= -1;
	int delta_x = xf - xi;
	if(delta_x < 0)
		delta_x *= -1;
	return (delta_x + delta_y);
}

/*		FUNCIONES AUXILIARES PARA GENERAR EL GRAFO/MAPA 	*/

/* Genera un grafo a modo de cuadrícula de tamaño anchoX por largoY.
Cada nodo del grafo tiene de clave el string "(x, y)" siendo esta la
coordenada x,y del nodo en la cuadrícula. Además, la función se encarga
de conectar cada nodo con sus cuatro nodos adyacentes en direcciones
arriba, abajo, izquierda y derecha y de forma ida y vuelta con peso 1*/
grafo_t* generar_cuadricula(int anchoX, int largoY){
	grafo_t *mapa = grafo_crear();
	if(!mapa) return NULL;
	// Creo primero todos los vértices
	char nodo[10];
	for(int i = 0; i < anchoX; i++)
		for(int j = 0; j < largoY; j++){
			sprintf(nodo, "(%d, %d)", i, j);
			grafo_crear_vertice(mapa, nodo);
			}

	// Ahora para todos los vértices, agrego sus cuatro adyacentes
	char adyAbajo[10], adyArriba[10], adyDer[10], adyIzq[10];
	for(int i = 0; i < anchoX; i++)
		for(int j = 0; j < largoY; j++){
			sprintf(nodo, "(%d, %d)", i, j);
			sprintf(adyAbajo, "(%d, %d)", i, j+1);
			sprintf(adyArriba, "(%d, %d)", i, j-1);
			sprintf(adyDer, "(%d, %d)", i+1, j);
			sprintf(adyIzq, "(%d, %d)", i-1, j);
			grafo_crear_arista(mapa, nodo, adyAbajo, FALSE, 1);
			grafo_crear_arista(mapa, nodo, adyDer, FALSE, 1);
			grafo_crear_arista(mapa, nodo, adyArriba, FALSE, 1);
			grafo_crear_arista(mapa, nodo, adyIzq, FALSE, 1);
		}
	return mapa;
}

/* Genera un grafo a modo de cuadrícula de tamaño anchoX por largoY.
Cada nodo del grafo tiene de clave el string "(x, y)" siendo esta la
coordenada x,y del nodo en la cuadrícula. La función conecta cada nodo
de 3 formas diferentes de acuerdo a los parámetros pesoN y pesoT:
	- Si pesoN < pesoT, todas las aristas tienen un peso aleatorio en
	el intervalo (pesoN, pesoT)
	- Si pesoN > pesoT, todas las aristas tienen un peso aleatorio en
	el intervalo (pesoT, pesoN)
	- Si pesoN = pesoT, todas las aristas tienen el mismo peso y
	es pesoN     */
grafo_t* generar_cuadricula_random(int anchoX, int largoY, int pesoN, int pesoT){
	srand (time(NULL));
	grafo_t *mapa = grafo_crear();
	if(!mapa) return NULL;
	// Creo primero todos los vértices
	char nodo[13];
	for(int i = 0; i < anchoX; i++)
		for(int j = 0; j < largoY; j++){
			sprintf(nodo, "(%d, %d)", i, j);
			grafo_crear_vertice(mapa, nodo);
			}

	// Ahora para todos los vértices, agrego sus cuatro adyacentes
	char adyAbajo[13], adyArriba[13], adyDer[13], adyIzq[13];
	for(int i = 0; i < anchoX; i++)		
		for(int j = 0; j < largoY; j++){
			// Armo los pesos de las aristas
			int pesos[4];
			for(int k = 0; k < 4; k++){
				if(pesoN == pesoT)
					pesos[k] = pesoN;
				else if(pesoN < pesoT)
					pesos[k] = (rand() % (pesoT - pesoN + 1)) + pesoN;
				else
					pesos[k] = (rand() % (pesoN - pesoT + 1)) + pesoT;
			}
			
			sprintf(nodo, "(%d, %d)", i, j);
			sprintf(adyAbajo, "(%d, %d)", i, j+1);
			sprintf(adyArriba, "(%d, %d)", i, j-1);
			sprintf(adyDer, "(%d, %d)", i+1, j);
			sprintf(adyIzq, "(%d, %d)", i-1, j);
			grafo_crear_arista(mapa, nodo, adyAbajo, FALSE, pesos[0]);
			grafo_crear_arista(mapa, nodo, adyDer, FALSE, pesos[1]);
			grafo_crear_arista(mapa, nodo, adyArriba, FALSE, pesos[2]);
			grafo_crear_arista(mapa, nodo, adyIzq, FALSE, pesos[3]);
			}
	return mapa;
}

/* Elimina un área rectangular del mapa cuadriculado, representada por
los vértices (origenX, origenY) hasta (finX, finY). NOTA: se asume que
origenX <= finX y que origenY <= finY. Así, se eliminan del grafo/mapa
todos los vértices que estén adentro del rectángulo de vértices
(origenX, origenY) hasta (finX, finY)*/
void eliminar_area(grafo_t* mapa, int origenX, int origenY, int finX, int finY){
	char nodo[10];
	for(int i = origenX; i <= finX; i++)
		for(int j = origenY; j <= finY; j++){
			sprintf(nodo, "(%d, %d)", i, j);
			grafo_borrar_vertice(mapa, nodo);
			}
}

/* Función auxiliar usada para el mapa_pokemon que convierte un nodo
en una celda de desfiladero (sólo se la puede atravesar de arriba
hacia abajo con peso 1 y no en ninguna dirección más)*/
void hacer_desfiladero(grafo_t* grafo, int x, int y){
	char nodo[10];
	char adyAbajo[10], adyArriba[10], adyDer[10], adyIzq[10];
	sprintf(nodo, "(%d, %d)", x, y);
	sprintf(adyAbajo, "(%d, %d)", x, y+1);
	sprintf(adyArriba, "(%d, %d)", x, y-1);
	sprintf(adyDer, "(%d, %d)", x+1, y);
	sprintf(adyIzq, "(%d, %d)", x-1, y);
	
	grafo_borrar_arista(grafo, nodo, adyDer);
	grafo_borrar_arista(grafo, nodo, adyIzq);
	grafo_borrar_arista(grafo, adyDer, nodo);
	grafo_borrar_arista(grafo, adyIzq, nodo);

	grafo_borrar_arista(grafo, nodo, adyArriba);
	grafo_borrar_arista(grafo, adyAbajo, nodo);
}

/* Función auxiliar usada para el mapa_pokemon que convierte un nodo
en una celda de hierba (sus aristas incidentes tienen peso 5)*/
void hacer_hierba(grafo_t* mapa, int x, int y){
	char nodo[10];
	char adyAbajo[10], adyArriba[10], adyDer[10], adyIzq[10];
	sprintf(nodo, "(%d, %d)", x, y);
	sprintf(adyAbajo, "(%d, %d)", x, y+1);
	sprintf(adyArriba, "(%d, %d)", x, y-1);
	sprintf(adyDer, "(%d, %d)", x+1, y);
	sprintf(adyIzq, "(%d, %d)", x-1, y);
	
	grafo_crear_arista(mapa, nodo, adyAbajo, TRUE, 5);
	grafo_crear_arista(mapa, nodo, adyDer, TRUE, 5);
	grafo_crear_arista(mapa, nodo, adyIzq, TRUE, 5);
	grafo_crear_arista(mapa, adyArriba, nodo, TRUE, 5);
	grafo_crear_arista(mapa, adyAbajo, nodo, TRUE, 5);
	grafo_crear_arista(mapa, adyDer, nodo, TRUE, 5);
	grafo_crear_arista(mapa, adyIzq, nodo, TRUE, 5);
	grafo_crear_arista(mapa, adyArriba, nodo, TRUE, 5);
}

/* Función auxiliar usada para el mapa_pokemon que convierte un área
rectangular en un área de hierba.*/
void hacer_area_hierba(grafo_t* mapa, int origenX, int origenY, int finX, int finY){
	for(int i = origenX; i <= finX; i++)
		for(int j = origenY; j <= finY; j++)
			hacer_hierba(mapa, i, j);
}

/*	FUNCIONES QUE GENERAN LOS TRES GRAFOS/MAPAS DE EJEMPLO 	*/

grafo_t* generar_mapa_pokemon(){
	grafo_t* mapa = generar_cuadricula(11, 14);

	eliminar_area(mapa, 9, 0, 10, 1);
	eliminar_area(mapa, 5, 12, 6, 13);
	eliminar_area(mapa, 4, 4, 5, 7);
	eliminar_area(mapa, 6, 5, 7, 6);
	
	for(int i = 0; i <= 3; i++){
		hacer_desfiladero(mapa, i, 4);
		hacer_desfiladero(mapa, i, 11);
		}
	
	hacer_area_hierba(mapa, 6, 0, 8, 0);
	hacer_area_hierba(mapa, 5, 1, 8, 3);
	hacer_area_hierba(mapa, 8, 2, 10, 3);	
	hacer_area_hierba(mapa, 7, 8, 8, 13);	
	hacer_area_hierba(mapa, 4, 8, 6, 10);
	hacer_area_hierba(mapa, 6, 7, 7, 7);
	hacer_area_hierba(mapa, 9, 9, 9, 11);
	hacer_area_hierba(mapa, 5, 11, 6, 11);		
	hacer_desfiladero(mapa, 4, 11);
	return mapa;
}

grafo_t* generar_mapa_dq3(){
	grafo_t* mapa = generar_cuadricula(11, 18);
		
	eliminar_area(mapa, 0, 0, 1, 0);
	eliminar_area(mapa, 8, 0, 10, 0);
	eliminar_area(mapa, 7, 1, 10, 1);
	eliminar_area(mapa, 5, 1, 5, 2);
	eliminar_area(mapa, 4, 2, 4, 3);
	eliminar_area(mapa, 10, 0, 10, 6);
	eliminar_area(mapa, 8, 2, 9, 2);
	eliminar_area(mapa, 9, 3, 9, 4);
	eliminar_area(mapa, 10, 16, 10, 17);
	eliminar_area(mapa, 4, 15, 7, 15);
	eliminar_area(mapa, 4, 13, 4, 14);
	eliminar_area(mapa, 2, 4, 3, 8);
	eliminar_area(mapa, 3, 3, 4, 3);
	eliminar_area(mapa, 4, 6, 7, 8);
	eliminar_area(mapa, 6, 9, 7, 14);
	eliminar_area(mapa, 9, 12, 9, 13);
	eliminar_area(mapa, 3, 13, 4, 13);
	eliminar_area(mapa, 8, 13, 9, 13);
	eliminar_area(mapa, 8, 8, 10, 8);
	eliminar_area(mapa, 1, 8, 1, 11);
	eliminar_area(mapa, 1, 11, 3, 11);
	
	return mapa;	
}

grafo_t* generar_mapa_pacman(){
	grafo_t* mapa = generar_cuadricula(17, 7);
		
	eliminar_area(mapa, 1, 1, 2, 1);
	eliminar_area(mapa, 1, 3, 2, 3);
	eliminar_area(mapa, 1, 4, 1, 5);
	eliminar_area(mapa, 4, 1, 4, 3);
	eliminar_area(mapa, 5, 3, 5, 4);
	eliminar_area(mapa, 3, 5, 4, 5);
	eliminar_area(mapa, 6, 5, 10, 5);
	eliminar_area(mapa, 8, 1, 8, 4);
	eliminar_area(mapa, 12, 5, 13, 5);
	eliminar_area(mapa, 14, 1, 15, 1);
	eliminar_area(mapa, 14, 3, 15, 3);
	eliminar_area(mapa, 15, 4, 15, 5);
	eliminar_area(mapa, 12, 1, 12, 2);
	eliminar_area(mapa, 10, 3, 12, 3);
	eliminar_area(mapa, 6, 0, 6, 1);
	eliminar_area(mapa, 10, 0, 10, 1);
	
	return mapa;
}

/* ----------------------------------------------------------------
 * 
 * 			PROGRAMA PRINCIPAAAAAALLL
 * 
 * ----------------------------------------------------------------*/

int test_analisis_posteriores(){
	grafo_t* mapa = generar_cuadricula_random(100, 100, 250, 275);
	path_finder_t* pf = path_finder_crear(superheuristica);
	
	path_finder_buscar_bfs(pf, mapa, "(0, 0)", "(99, 99)");
	printf("\nLONG FINAL DEL CAMINO PARA BFS: %d\n", path_finder_longitud_camino(pf));
	
	path_finder_buscar_heuristica(pf, mapa, "(0, 0)", "(99, 99)");
	printf("\nLONG FINAL DEL CAMINO PARA HEURISTICA: %d\n", path_finder_longitud_camino(pf));
	
	path_finder_buscar_dijkstra(pf, mapa, "(0, 0)", "(99, 99)");
	printf("\nLONG FINAL DEL CAMINO PARA DIJKSTRA: %d\n", path_finder_longitud_camino(pf));
	
	path_finder_buscar_a_star(pf, mapa, "(0, 0)", "(99, 99)");
	printf("\nLONG FINAL DEL CAMINO PARA A*: %d\n", path_finder_longitud_camino(pf));	
			
	path_finder_destruir(pf);
	grafo_destruir(mapa);
	return 0;
}

int test_mejores_peores_casos() {
	grafo_t* mapa = generar_mapa_pokemon();
	path_finder_t* pf = path_finder_crear(superheuristica);
	
	path_finder_buscar_a_star(pf, mapa, "(3, 10)", "(3, 0)");

	// Mostrar el camino final
	lista_t* vertices = path_finder_camino(pf);
	printf("CAMINO FINAL: \n");
	lista_iter_t* it = lista_iter_crear(vertices);
	while(!lista_iter_al_final(it)){
		printf("%s  ", (char*)lista_iter_ver_actual(it));
		lista_iter_avanzar(it);
		}
	lista_iter_destruir(it);
	
	printf("\nLONG FINAL DEL CAMINO: %d\n", path_finder_longitud_camino(pf));
	
	// Sección de visitados: Permite ver si un algoritmo visitó o no
	// alguno(s) vértice(s)
	print_test("Vértice (3, 5) fue visitado",  path_finder_fue_visitado(pf, "(3, 5)"));
	print_test("Vértice (10, 13) fue visitado",  path_finder_fue_visitado(pf, "(10, 13)"));
	print_test("Vértice (0, 10) fue visitado",  path_finder_fue_visitado(pf, "(0, 10)"));
	
	path_finder_destruir(pf);
	grafo_destruir(mapa);
	return 0;
}

int main(){
	return test_analisis_posteriores();

	return 0;
}
