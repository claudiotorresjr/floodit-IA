#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "graph.h"
#include "stack.h"
#include "doubleQueue.h"

int total;
int atual_region = 0;
int total_regions = 0;

int module(int a)
{
    if (a < 0)
    {
        return -a;
    }

    return a;
}

Map *create_map(FILE *file)
{
    Map *map = (Map *)malloc(sizeof(Map));
    fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->n_colors);
    map->map = allocate_matrix(map->rows, map->cols);

    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            map->map[i][j].region = -1;
            fscanf(file, "%d ", &map->map[i][j].color);
        }
    }

    return map;
}

void reset_map(Map **m)
{
    for (int i = 0; i < (*m)->rows; ++i)
    {
        for (int j = 0; j < (*m)->cols; ++j)
        {
            if ((*m)->map[i][j].color < 0)
            {
                (*m)->map[i][j].color = -(*m)->map[i][j].color;
            }
        }
    }
}

void show_matrix(Index **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j].color);
        }
        printf("\n");
    }
}

int isSolved(Index **m, int r, int c)
{
    int first_c = m[0][0].color;
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j].color != first_c)
            {
                return 0;
            }
        }
    }

    return 1;    
}

void paint_graph(Graph *g, Vertice *v, int base_color, int color)
{
    g->array[v->region].head->visited = 1;
    g->array[v->region].head->color = color;

    Vertice *aux = v->next;
    while (aux)
    {
        if (aux->color == base_color && !g->array[aux->region].head->visited)
        {
            paint_graph(g, g->array[aux->region].head, base_color, color);
        }

        aux = aux->next;
    }
    
}

void frontier(Map **m, int l, int c, int atual_color, int region, Graph *g)
{
    if ((*m)->map[l][c].color == atual_color)
    {
        total++;

        (*m)->map[l][c].color = -(*m)->map[l][c].color;
        if ((*m)->map[l][c].region == -1)
        {
            (*m)->map[l][c].region = region;
        }
    
        if ((*m)->rows - 1 > l)
            {
                frontier(m, l + 1, c, atual_color, region, g);
            }
        if ((*m)->cols - 1 > c)
        {
            frontier(m, l, c + 1, atual_color, region, g);
        }
        if (l > 0)
        {
            frontier(m, l - 1, c, atual_color, region, g);
        }
        if (c > 0)
        {
            frontier(m, l, c - 1, atual_color, region, g);
        }
    }
    else if ((*m)->map[l][c].color != -atual_color)
    {
        if (g)
        {
            if ((*m)->map[l][c].color > 0)
            {
                clock_t start, end;
                double cpu_time_used;
                int queue = 0;

                total_regions++;

                int save_region = region;

                if ((*m)->map[l][c].region > -1)
                {
                    region = (*m)->map[l][c].region;
                }
                else
                {
                    atual_region += 1;
                    region = atual_region;
                }

                int save_total = total;
                total = 0;
                // printf("achando a fronteira de %d-%d, regiao: %d\n", l, c, region);
                frontier(m, l, c, (*m)->map[l][c].color, region, NULL);

                int pos[2];
                pos[0] = l;
                pos[1] = c;

                // printf("Esse aqui vai ser o indice: %d\n", save_region);
                add_edge(
                    g,
                    save_region,
                    module(atual_color),
                    save_total,
                    region,
                    module((*m)->map[l][c].color),
                    total,
                    pos
                );

                total = save_total;
                region = save_region;
            }
        }
    }
}

void print_solution(Solution *s)
{
    printf("%d\n", s->steps);
    for (int i = 0; i < s->steps; i++)
    {
        printf("%d ", s->colors[i]);
    }
    printf("\n");
}

int calculate_min_distance(int size, int n_colors, int *colors)
{
    int min_distance = size;
    int color = 0;
    for (int d = 1; d < (n_colors)+1; ++d)
    {
        if (colors[d] < min_distance && colors[d] != -1)
        {
            min_distance = colors[d];
            color = d;
        }
    }

    return color;
}

int main(int argc, char const *argv[])
{
    clock_t start, end;
    double cpu_time_used;

    FILE *map_file = stdin;
    if (map_file == NULL)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    Map *map = create_map(map_file);

    Graph *g = create_graph(map->rows*map->cols);

    total = 0;
    int region = 0;
    frontier(&map, 0, 0, map->map[0][0].color, region, g);
    reset_map(&map);

    region = total_regions + 1;
    total_regions = 0;

    for (int i = 0; i < g->num_v; ++i)
    {
        // start = clock();
        Vertice *aux = g->array[i].head->next;
        // printf("indice da lista: %d\n", i);
        while(aux != NULL)
        {
            // printf("procurando regiao %d(%d, %d)\n", aux->region, aux->pos[0], aux->pos[1]);
            // printf("posicao sendo analizada: (%d, %d)\n", aux->pos[0], aux->pos[1]);
            total = 0;
            frontier(&map, aux->pos[0], aux->pos[1], aux->color, aux->region, g);
            reset_map(&map);
            // atual_region = total_regions + 1;
            region = total_regions + 1;
            total_regions = 0;

            aux = aux->next;
        }
        // end = clock();
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("fun() took %f seconds to execute \n", cpu_time_used);
    }
    // show_graph(g);
    // printf("------------------\n");
    // show_matrix(map->map, map->rows, map->cols);
    // printf("------------------\n");
    // printf("%d\n", g->num_v);


    Solution *solution = (Solution *)malloc(sizeof(Solution));
    solution->steps = 0;
    solution->colors = (int *)malloc(map->rows*map->cols*sizeof(int));

    g->array->visiteds++;

    int stop = 0;
    int anterior = 0;

    int *colors = (int *)calloc(((map->n_colors)+1), sizeof(int));

    // for (int c = 1; c <= map->n_colors; ++c)
    // {
    //     // printf("pintando com a cor: %d\n", c);

    //     if (c == g->array[0].head->first_color)
    //     {
    //         colors[c] = -1;
    //         continue;
    //     }

        
    //     colors[c] = distance_between_nodes(g, c);
    //     // printf("cor %d distancia == %d\n", c, colors[c]);
    // }

    // int color = calculate_min_distance(map->rows*map->cols, map->n_colors, colors);

    paint_graph(g, g->array[0].head, g->array[0].head->color, 1);
    paint_graph(g, g->array[0].head, g->array[0].head->color, 2);
    paint_graph(g, g->array[0].head, g->array[0].head->color, 1);
    paint_graph(g, g->array[0].head, g->array[0].head->color, 3);

    for (int i = 0; i < g->num_v; ++i)
    {
        printf("regiao %d cor == %d, ", g->array[i].head->region, g->array[i].head->color);
    }
    printf("\n");
    // printf("menor distancia para a cor %d\n", color);
    // solution->colors[solution->steps++] = color;

    print_solution(solution);

    return 0;
}
