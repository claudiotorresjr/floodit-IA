#ifndef _MAIN_H
#define _MAIN_H

typedef struct Map
{
    int **map;
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
void show_matrix(int **matrix, int rows, int cols);
Map *create_map(FILE *file);
void paintOneColor(int ***m, int rows, int cols, int r, int c, int init_c, int color);

#endif