#ifndef _MAIN_H
#define _MAIN_H

typedef struct Map
{
    int **map;
    int rows;
    int cols;
    int colors;
} Map;

int **createMatrix(int rows, int cols);
void showMatrix(int **matrix, int rows, int cols);
Map *createMap(FILE *file);
void paintOneColor(Map **m, int lin, int col, int init_c, int color);

#endif