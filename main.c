#include <stdio.h>
#include <stdlib.h>

#include "main.h"

char **allocateMatrix(int rows, int cols)
{
    char **matrix = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (char *)malloc(cols * sizeof(char));
    }

    return matrix;
}

MapInfo *createMap(FILE *file)
{
    MapInfo *map = (MapInfo *)malloc(sizeof(MapInfo));
    fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->colors);
    map->map_grid = allocateMatrix(map->rows, map->cols);

    int ch;
    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            fscanf(file, "%c ", &map->map_grid[i][j]);
        }
    }

    return map;
}

void showMatrix(char **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%c ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[])
{
    FILE *map_file = stdin;
    if (map_file == NULL)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    MapInfo *map_info = createMap(map_file);

    showMatrix(map_info->map_grid, map_info->rows, map_info->cols);

    return 0;
}
