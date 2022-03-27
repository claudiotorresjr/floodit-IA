#ifndef _MAIN_H
#define _MAIN_H

typedef struct Map
{
    int **map;
    int rows;
    int cols;
    int n_colors;
} Map;

typedef struct Position
{
    int l;
    int c;
    int v;
} Position;

typedef struct Frontier
{
    int size;
    Position *pos;
} Frontier;

typedef struct Solution
{
    int steps;
    int *colors;
} Solution;

int **createMatrix(int rows, int cols);
void show_matrix(int **matrix, int rows, int cols);
Map *create_map(FILE *file);
void paintOneColor(Map **m, int lin, int col, int init_c, int color);

#endif