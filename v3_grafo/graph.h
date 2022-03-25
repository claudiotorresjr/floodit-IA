#ifndef _SGRAPH_H
#define _SGRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Vertice
{
    int state; //visitado ou nao
    int size; //quantas cores
    int color;
    int pos[2]; //a primeira posicao da regiao
    int grau;
    struct Vertice *next;
    struct Vertice *parent;
} Vertice;

typedef struct AdjList
{
	Vertice *head;
	//struct _vertice *tail;

} AdjList;

typedef struct Graph
{
    int num_v;
    int num_e;
    AdjList *array;
} Graph;

Vertice *create_vertice(int size, int color, int pos[2]);
int has_edge(Graph *g, int a[2], int b[2]);
void add_edge(Graph *g, int a[2], int color_a, int size_a, int b[2], int color_b, int size_b);
Graph *create_graph(int size);

#endif