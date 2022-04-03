/**
 * @file map.h
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief All structures and functions used in map.c
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#ifndef _MAIN_H
#define _MAIN_H

#include "graph.h"

/**
 * @brief Struct that represents an index  in matrix(position)
 * 
 */
typedef struct Index
{
    int region; //index region value
    int color; //index color
} Index;

/**
 * @brief Struct that represents a map information
 * 
 */
typedef struct Map
{
    Index **map; //array of type index (represents the color matrix)
    int rows; //amount of rows
    int cols; //amount of cols
    int n_colors; //amount of different colors
} Map;

int module(int a);
int map_is_solved(Index **m, int rows, int cols);
Index **allocate_matrix(int rows, int cols);
Map *create_map(FILE *file);
void reset_map(Map **m);
void show_matrix(Index **matrix, int rows, int cols);
void free_map(Map *m);
void free_map(Map *m);
void frontier(Map **m, int r, int c, int atual_color, int region, Graph *g);
Graph *map_to_graph(Map *map);

#endif