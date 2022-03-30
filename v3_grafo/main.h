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

typedef struct Solution
{
    int steps;
    int *colors;
} Solution;

int **createMatrix(int rows, int cols);
void show_matrix(Index **matrix, int rows, int cols);
Map *create_map(FILE *file);
void paint_graph(Graph *g, Vertice *v, int base_color, int color);

#endif