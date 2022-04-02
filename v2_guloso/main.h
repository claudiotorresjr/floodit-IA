#ifndef _MAIN_H
#define _MAIN_H

typedef struct Map
{
    int **map;
    int rows;
    int cols;
    int colors;
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
void showMatrix(int **matrix, int rows, int cols);
Map *createMap(FILE *file);

#endif