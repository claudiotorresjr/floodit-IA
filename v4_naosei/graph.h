#ifndef _SGRAPH_H
#define _SGRAPH_H

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"

typedef struct Vertice
{
    int size;
    int color;
    int region;
    int visited;
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

int distance_between_nodes(Graph *g, int from, int to);
void initialize_array(int **colors, int size, int v);
int *verify_max_color_count(Graph *g, State *current, int t_colors);
Vertice *create_vertice(int size, int color, int region, int pos[2]);
int has_edge(Graph *g, int a, int b);
void add_edge(Graph *g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2]);
Graph *create_graph(int size);
void show_graph(Graph *g);

#endif