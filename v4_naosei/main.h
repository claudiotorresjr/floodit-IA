#ifndef _MAIN_H
#define _MAIN_H

#include "stack.h"

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
void paintOneColor(Map **m, int rows, int cols, int r, int c, int init_c, int color);

#endif