#ifndef _MAIN_H
#define _MAIN_H

#include "stack.h"
#include "graph.h"

typedef struct Map
{
    Index **map;
    int rows;
    int cols;
    int n_colors;
} Map;

int module(int a);
Index **allocate_matrix(int rows, int cols);
Map *create_map(FILE *file);
void reset_map(Map **m);
void show_matrix(Index **matrix, int rows, int cols);
void free_map(Map *m);
void free_map(Map *m);
void frontier(Map **m, int l, int c, int atual_color, int region, Graph *g);
Graph *map_to_graph(Map *map);

#endif