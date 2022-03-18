#ifndef _MAIN_H
#define _MAIN_H

typedef struct MapInfo
{
    char **map_grid;
    int rows;
    int cols;
    int colors;
} MapInfo;

int **createMatrix(int rows, int cols);
void showMatrix(char **matrix, int rows, int cols);
MapInfo *createMap(FILE *file);

#endif