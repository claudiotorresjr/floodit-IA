/**
 * @file solver.c
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief File with solver.h implementation. All functions related to the heuristic is here.
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#include <stdlib.h>

#include "../include/graph.h"
#include "../include/solver.h"
#include "../include/doubleQueue.h"

/**
 * @brief Print the floodit solution
 * 
 * @param s The solution object
 */
void print_solution(Solution *s)
{
    printf("%d\n", s->steps);
    for (int i = 0; i < s->steps; i++)
    {
        printf("%d ", s->colors[i]);
    }
    printf("\n");
}

/**
 * @brief Check if graph is with same color (solved)
 * 
 * @param g The graph
 * @return int - 1 if is solved or 0 if not
 */
int is_solved(Graph *g)
{
    int color = g->array[0].head->color;
    for (int i = 1; i < g->num_v; ++i)
    {
        if (g->array[i].head)
        {
            if (g->array[i].head->color != color)
            {
                return 0;
            }
        }
    }

    return 1;
}

/**
 * @brief Check if color is in some region neighbors 
 * 
 * @param g The graph
 * @param v The vertice (region) to check its neighbors
 * @param base_color The color of the region who called this function
 * @param color The color to search in neighbors
 * @return int - 1 if color is in region neighbors or 0 if not
 */
int color_is_in_region(Graph *g, Vertice *v, int base_color, int color)
{    
    g->array[v->region].head->visited = 1;
    Vertice *aux = v->next;
    while (aux)
    {
        if(aux->color == color)
        {
            return 1;
        }

        if (g->array[aux->region].head->color == base_color && !g->array[aux->region].head->visited)
        {
            return color_is_in_region(g, g->array[aux->region].head, base_color, color);
        }

        aux = aux->next;
    }

    return 0;
}

/**
 * @brief This color is not present in graph anymore
 * 
 * @param g The graph
 * @param color The color to search in graph
 * @return int - 1 if color is not in graph or 0 if it is
 */
int no_color_found(Graph *g, int color)
{
    for (int i = 0; i < g->num_v; ++i)
    {
        if (g->array[i].head)
        {
            if (g->array[i].head->color == color)
            {
                return 0;
            }
        }
    }

    return 1;
}

/**
 * @brief Calculate from all distances calculated, the smaller one
 * 
 * @param size The max size of the distance
 * @param n_colors The number of different colors in graph 
 * @param colors The painted color array with all colors and its distance generated
 * @return int - the color with smaller distance
 */
int calculate_min_distance(int size, int n_colors, int *colors)
{
    int min_distance = size;
    int color = 0;
    for (int d = 1; d < n_colors+1; ++d)
    {
        if (colors[d] < min_distance && colors[d] != -1)
        {
            min_distance = colors[d];
            color = d;
        }
    }

    return color;
}

/**
 * @brief Calculate how many regions each color has 
 * 
 * @param g The graph
 * @param n_colors The number of different colors in graph 
 * @return int* - array with all colors and its regions amount
 */
int *remaining_nodes_by_color(Graph *g, int n_colors)
{

    int *colors_remaining = (int *)calloc(n_colors + 1, sizeof(int));
    for (int i = 0; i < g->num_v; ++i)
    {
        if (g->array[i].head)
        {
            colors_remaining[g->array[i].head->color]++;
        }
    }

    return colors_remaining;
}

/**
 * @brief Find te best color to paint the graph
 * 
 * @param g The graph
 * @param n_colors The number of different colors in graph 
 * @return int - the best color to paint
 */
int find_optimal_color(Graph *g, int n_colors)
{
    int *colors = (int *)calloc(n_colors + 1, sizeof(int));

    int optimal_color = 0;
    for (int c = 1; c <= n_colors; ++c)
    {
        //dont paint with color that is not in region 0 neighbors
        if (!color_is_in_region(g, g->array[0].head, g->array[0].head->color, c))
        {
            colors[c] = -1;
            reset_graph(&g);

            continue;
        }

        // printf("painting with color: %d\n", c, g->array[0].head->color);            
        reset_graph(&g);

        paint_graph(&g, g->array[0].head, g->array[0].head->color, c, 0);
        
        //TODO
        //if a color can be eliminated in one move
        // from the current position, that move is an optimal move and we can
        // simply use it
        // if(no_color_found(g, c))
        // {
        //     optimal_color = c;
        //     break;
        // }
        
        colors[c] = distance_between_nodes(g);
        //  printf("Color %d with distance == %d\n", c, colors[c]);

        for (int j = 0; j < g->num_v; ++j)
        {
            if (g->array[j].head)
            {
                g->array[j].head->color = g->array[j].head->first_color;
                g->array[j].head->visited = 0;
            }
        }
    }

    int color;
    if (!optimal_color)
    {
        color = calculate_min_distance(99999999, n_colors, colors);
    }
    else
    {
        color = optimal_color;
    }
    // printf(" --> Smaller distance for the color %d (distance == %d)\n", color, colors[color]);
    free(colors);

    return color;

    return 1;
}