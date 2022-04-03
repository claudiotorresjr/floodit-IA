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
 * @brief Calculate the amount of each color 
 * 
 * @param g The graph
 * @param n_colors The number of different colors in graph 
 * @return int* - array with all colors and its amount
 */
int *total_colors_painted(Graph *g, int n_colors)
{

    int *colors_remaining = (int *)calloc(n_colors + 1, sizeof(int));
    for (int i = 0; i < g->num_v; ++i)
    {
        if (g->array[i].head)
        {
            colors_remaining[g->array[i].head->color] += g->array[g->array[i].head->region].head->size;
        }
    }

    return colors_remaining;
}

/**
 * @brief Find te best color to paint the graph
 * 
 * @param g The graph
 * @param n_colors The number of different colors in graph 
 * @return int - the best color to paint and the cost to it
 */
int *find_optimal_color(Graph *g, int n_colors, int cost)
{
    int optimal_color = 0;

    //first check if a color can be eliminated in one move. If yes, we can use it
    int colors_painted = 0;
    //int *last_colors_remaining, *colors_remaining, *future_colors;
    for (int c = 1; c <= n_colors; ++c)
    {
        //dont paint with color that is not in region 0 neighbors
        if (!color_is_in_region(g, g->array[0].head, g->array[0].head->color, c))
        {
            reset_graph(&g);

            continue;
        }

        reset_graph(&g);

        int *last_colors_remaining = total_colors_painted(g, n_colors);
        //paint with one color
        paint_graph(&g, g->array[0].head, g->array[0].head->color, c, 0);

        int *colors_remaining = total_colors_painted(g, n_colors);

        reset_graph(&g);
        //now, paint with 0 to see if the color c is eliminated
        paint_graph(&g, g->array[0].head, g->array[0].head->color, 0, 0);
        int *future_colors = total_colors_painted(g, n_colors);

        int total = colors_remaining[c];
        int atual = last_colors_remaining[c];
        int future_c = future_colors[c];
 
        free(last_colors_remaining);
        free(colors_remaining);
        free(future_colors);

        if (future_c == 0)
        {
            // printf("eliminated! painting with color: %d\n", c); 
            colors_painted = total - atual;
            optimal_color = c;

            break;
        }

        for (int j = 0; j < g->num_v; ++j)
        {
            if (g->array[j].head)
            {
                g->array[j].head->color = g->array[j].head->first_color;
            }
        }
    }

    if (optimal_color)
    {
        int *result = (int *)malloc(2*sizeof(int));
        result[0] = optimal_color;
        result[1] = colors_painted;

        return result;
    }

    //if the color cant't be eliminated in one move, check all distances and choose
    //the one that minimize the sum of all distances
    int *colors = (int *)calloc(n_colors + 1, sizeof(int));
    colors_painted = 0;
    for (int c = 1; c <= n_colors; ++c)
    {
        //dont paint with color that is not in region 0 neighbors
        if (!color_is_in_region(g, g->array[0].head, g->array[0].head->color, c))
        {
            colors[c] = -1;
            reset_graph(&g);

            continue;
        }

        // printf("painting with color: %d\n", c);              
        reset_graph(&g);

        int *last_colors_remaining = total_colors_painted(g, n_colors);
        paint_graph(&g, g->array[0].head, g->array[0].head->color, c, 0);
        
        colors[c] = distance_between_nodes(g);
        // printf("Color %d with distance == %d\n", c, colors[c]);
         
        int *colors_remaining = total_colors_painted(g, n_colors);

        //calculate the cost to reach the new state
        colors_painted = colors_remaining[c] - last_colors_remaining[c];
        colors[c] += colors_painted + cost;

        // for (int i = 1; i < n_colors+1; ++i)
        // {
        //     printf("(%d: %d) ", i, colors_remaining[i]);
        // }
        // printf("\n");


        for (int j = 0; j < g->num_v; ++j)
        {
            if (g->array[j].head)
            {
                g->array[j].head->color = g->array[j].head->first_color;
                g->array[j].head->visited = 0;
            }
        }
        free(last_colors_remaining);
        free(colors_remaining);
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

    int *result = (int *)malloc(2*sizeof(int));
    result[0] = color;
    result[1] = colors_painted;

    return result;
}