/**
 * @file graph.h
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief All structures and functions used in graph.c
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Struct that represents a graph vertice (region)
 * 
 */
typedef struct Vertice
{
    int size; //vertice (region) size
    int color; //vertice (region) color
    int first_color; //vertice (region) first_color (to save color in loops)
    int region; //vertice (region) region value
    int visited; //vertice (region) status: 0 - not visited | 1 - visited | 2 - already in queue
    int parent; //vertice (region) parent
    int pos[2]; //vertice (region) first position found
    struct Vertice *prev; //vertice (region) previous vertice
    struct Vertice *next; //vertice (region) next vertice
} Vertice;

/**
 * @brief Struct that represents an adjacency list
 * 
 */
typedef struct AdjList
{
	Vertice *head; //vertice (region) previous vertice
	Vertice *tail; //vertice (region) next vertice

} AdjList;

/**
 * @brief Struct that represents a graph
 * 
 */
typedef struct Graph
{
    int num_v; //number of vertices
    int num_e; //number of edges
    AdjList *array; //adjacency list
} Graph;

void paint_graph(Graph **g, Vertice *v, int base_color, int color, int change_color);
void remove_node_from_list(Vertice *head, int region);
void merge_nodes(Graph **g, int region, int color);
void reset_graph(Graph **g);
int distance_between_nodes(Graph *g);
Vertice *create_vertice(int size, int color, int region, int parent, int pos[2]);
int has_edge(Graph *g, int a, int b);
void add_edge(Graph **g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2]);
Graph *create_graph(int size);
void free_vertices_list(AdjList *l);
void free_graph(Graph *g);
void show_graph(Graph *g);

#endif