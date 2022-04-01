#ifndef _GRAPH_H
#define _GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

typedef struct Vertice
{
    int size;
    int color;
    int first_color;
    int region;
    int visited;
    int parent;
    int pos[2];
    struct Vertice *next;
} Vertice;

typedef struct AdjList
{
    int visiteds;
	Vertice *head;
	//struct _vertice *tail;

} AdjList;

typedef struct Graph
{
    int num_v;
    int num_e;
    AdjList *array;
} Graph;

void reset_graph(Graph *g);
int distance_between_nodes(Graph *g, int c);
void initialize_array(int **colors, int size);
int *verify_max_color_count(Graph *g, State *current, int t_colors);
Vertice *create_vertice(int size, int color, int region, int parent, int pos[2]);
int has_edge(Graph *g, int a, int b);
void add_edge(Graph *g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2]);
Graph *create_graph(int size);
void free_vertices_list(AdjList *l);
void free_graph(Graph *g);
void show_graph(Graph *g);

#endif