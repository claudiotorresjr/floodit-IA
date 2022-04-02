#include <stdlib.h>

#include "../include/graph.h"
#include "../include/solver.h"
#include "../include/doubleQueue.h"

void print_solution(Solution *s)
{
    printf("%d\n", s->steps);
    for (int i = 0; i < s->steps; i++)
    {
        printf("%d ", s->colors[i]);
    }
    printf("\n");
}

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

int color_is_in_region(Graph *g, Vertice *v, int base_color, int color)
{    
    g->array[v->region].head->visited = 1;
    Vertice *aux = v->next;
    while (aux)
    {
        if(aux->color == color)
        {
            // printf("tem o %d\n", aux->color);
            return 1;
        }

        if (g->array[aux->region].head->color == base_color && !g->array[aux->region].head->visited)
        {
            return color_is_in_region(g, g->array[aux->region].head, base_color, color);
        }

        aux = aux->next;
    }

    // printf("Nao tem \n");
    return 0;
}

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

State *create_state(int region, int color)
{
    State *s = (State *)malloc(sizeof(State));

    s->region = region;
    s->distance = 0;
    s->color = color;
    s->prev = NULL;
    s->next = NULL;

    return s;
}

int solve_floodit(Graph *g, int n_colors)
{
    int *colors = (int *)calloc(n_colors + 1, sizeof(int));

    int optimal_color = 0;
    for (int c = 1; c <= n_colors; ++c)
    {
        if (!color_is_in_region(g, g->array[0].head, g->array[0].head->color, c))
        {
            colors[c] = -1;
            reset_graph(g);

            continue;
        }
        // printf("pintando com a cor: %d, o %d (%d)\n", c, g->array[0].head->color, g->array[0].head->first_color);            
        reset_graph(g);

        paint_graph(&g, g->array[0].head, g->array[0].head->color, c, 0);

        if(no_color_found(g, c))
        {
            optimal_color = c;
            break;
        }
        
        colors[c] = distance_between_nodes(g, c);
        //  printf("cor %d distancia == %d\n", c, colors[c]);

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
        color = calculate_min_distance(999999, n_colors, colors);
    }
    else
    {
        color = optimal_color;
    }
    // printf(" --> menor distancia para a cor %d (%d)\n", color, colors[color]);
    free(colors);

    return color;

    return 1;
}