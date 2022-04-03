/**
 * @file solver.h
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief All structures and functions used in solver.c
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#ifndef _SOLVER_H
#define _SOLVER_H

#include "graph.h"
#include "map.h"

/**
 * @brief Struct that represents a solution
 * 
 */
typedef struct Solution
{
    int steps; //amount of solution steps
    int *colors; //array with all colors for the solution
} Solution;

void print_solution(Solution *s);
int is_solved(Graph *g);
int color_is_in_region(Graph *g, Vertice *v, int base_color, int color);
int no_color_found(Graph *g, int color);
int calculate_min_distance(int size, int n_colors, int *colors);
int *total_colors_painted(Graph *g, int n_colors);
int *find_optimal_color(Graph *g, int n_colors, int cost);

#endif