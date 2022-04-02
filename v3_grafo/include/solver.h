#ifndef _SOLVER_H
#define _SOLVER_H

#include "graph.h"

typedef struct Solution
{
    int steps;
    int *colors;
} Solution;

void print_solution(Solution *s);
int is_solved(Graph *g);
int color_is_in_region(Graph *g, Vertice *v, int base_color, int color);
int no_color_found(Graph *g, int color);
int calculate_min_distance(int size, int n_colors, int *colors);
int solve_floodit(Graph *g, int n_colors);


#endif