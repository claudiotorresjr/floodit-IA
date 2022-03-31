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

void free_map(Map *m)
{
    for(int i = 0; i < m->rows; ++i)
    {
        free(m->map[i]);
    }
    free(m->map);
    // free(m);
}

int is_solved(Graph *g)
{
    int color = g->array[0].head->color;
    for (int i = 1; i < g->num_v; ++i)
    {
        if (g->array[i].head->color != color)
        {
            return 0;
        }
    }

    return 1;
}

void paint_graph(Graph **g, Vertice *v, int base_color, int color, int change_color)
{
    // printf("pintando a reg: %d com cor %d\n", v->region, color);
    (*g)->array[v->region].head->visited = 1;
    (*g)->array[v->region].head->color = color;

    if (change_color)
    {
        (*g)->array[v->region].head->first_color = color;
    }

    Vertice *aux = v->next;
    while (aux)
    {
        // printf("---- verificando a reg: %d (%d) - %d == cor base %d\n", aux->region, aux->color, (*g)->array[aux->region].head->visited, base_color);
        if ((*g)->array[aux->region].head->color == base_color && !(*g)->array[aux->region].head->visited)
        {
            // printf("->> tem cor %d\n", base_color);
            paint_graph(g, (*g)->array[aux->region].head, base_color, color, change_color);
        }

        aux = aux->next;
    }
    // free(aux);
}

int color_is_in_region(Graph *g, Vertice *v, int base_color, int color)
{
    g->array[v->region].head->visited = 1;
    Vertice *aux = v->next;
    while (aux)
    {
        if(aux->color == color)
        {
            printf("tem o %d\n", aux->color);
            return 1;
        }

        if (g->array[aux->region].head->color == base_color && !g->array[aux->region].head->visited)
        {
            return color_is_in_region(g, g->array[aux->region].head, base_color, color);
        }

        aux = aux->next;
    }

    printf("Nao tem \n");
    return 0;
    
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

    free_map(map);

    // paint_graph(&g, g->array[0].head, g->array[0].head->color, 15, 1);
    // paint_graph(&g, g->array[0].head, g->array[0].head->color, 16, 1);
    // paint_graph(&g, g->array[0].head, g->array[0].head->color, 17, 1);
    // paint_graph(&g, g->array[0].head, g->array[0].head->color, 1, 1);

    // color_is_in_region(g, g->array[0].head, g->array[0].head->color, 1);

    while (!is_solved(g))
    {
    
        int *colors = (int *)calloc(((map->n_colors)+1), sizeof(int));

        for (int c = 1; c <= map->n_colors; ++c)
        {
            printf("pintando com a cor: %d, o %d\n", c, g->array[0].head->color);

            if (!color_is_in_region(g, g->array[0].head, g->array[0].head->color, c))
            {
                colors[c] = -1;
                continue;
            }
            reset_graph(g);

            paint_graph(&g, g->array[0].head, g->array[0].head->color, c, 0);
            colors[c] = distance_between_nodes(g, c);

            printf("cor %d distancia == %d\n", c, colors[c]);

            for (int j = 0; j < g->num_v; ++j)
            {
                g->array[j].head->color = g->array[j].head->first_color;
                g->array[j].head->visited = 0;
            }
        }

        int color = calculate_min_distance(999999, map->n_colors, colors);
        printf(" --> menor distancia para a cor %d (%d)\n", color, colors[color]);
        free(colors);

        paint_graph(&g, g->array[0].head, g->array[0].head->color, color, 1);
        // g->array[0].head->first_color = g->array[0].head->color;

        // for (int j = 0; j < g->num_v; ++j)
        // {
        //    printf("(%d - %d (%d)) ", g->array[j].head->region, g->array[j].head->color, g->array[j].head->first_color);
        // }
        // printf("\n");
        reset_graph(g);

        solution->colors[solution->steps++] = color;

        if (stop == 8)
        {
            // break;
        }
        stop++;
        // printf("%d\n", stop);
    }

    print_solution(solution);

    free_graph(g);
    free(solution->colors);
    free(solution);
    free(map);

    return 0;
}
