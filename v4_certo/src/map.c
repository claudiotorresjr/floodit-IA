#include <stdlib.h>

#include "../include/map.h"

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

Index **allocate_matrix(int rows, int cols)
{
    Index **matrix = (Index **)malloc(rows * sizeof(Index *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (Index *)malloc(cols * sizeof(Index));
    }

    return matrix;
}

Map *create_map(FILE *file)
{
    Map *map = (Map *)malloc(sizeof(Map));

    if (!fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->n_colors))
    {
        fprintf(stderr, "Error reading file header.\n");
        exit(1);
    }
    map->map = allocate_matrix(map->rows, map->cols);

    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            map->map[i][j].region = -1;
            if (!fscanf(file, "%d ", &map->map[i][j].color))
            {
                fprintf(stderr, "Error reading map colors.\n");
                exit(1);
            }
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

Graph *map_to_graph( Map *map)
{
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

    return g;
}