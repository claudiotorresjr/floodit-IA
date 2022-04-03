/**
 * @file map.c
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief File with map.h implementation. All functions for file and matrix manipulation is here.
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#include <stdlib.h>

#include "../include/map.h"

//global vars to control some recursive functions
int total;
int atual_region = 0;
int total_regions = 0;

/**
 * @brief The module from a
 * 
 * @param a The value to get the module
 * @return int - a module
 */
int module(int a)
{
    if (a < 0)
    {
        return -a;
    }

    return a;
}

/**
 * @brief Check if map matrix colors is the same
 * 
 * @param m The map object
 * @param rows The map matrix amount of rows
 * @param cols The map matrix amount of cols
 * @return int - 1 if is solved or 0 if not
 */
int map_is_solved(Index **m, int rows, int cols)
{
    int color = m[0][0].color;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            //return false if some color is not the same of the first color
            if (m[i][j].color != color)
            {
                return 0;
            }
        }
    }

    return 1;
}

/**
 * @brief Create a matrix
 * 
 * @param rows The map matrix amount of rows
 * @param cols The map matrix amount of cols
 * @return Index** - A matrix
 */
Index **allocate_matrix(int rows, int cols)
{
    Index **matrix = (Index **)malloc(rows * sizeof(Index *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (Index *)malloc(cols * sizeof(Index));
    }

    return matrix;
}

/**
 * @brief Create a map object
 * 
 * @param file The file with map information
 * @return Map* 
 */
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

/**
 * @brief Reset all map values to its original value
 * 
 * @param m The map object
 */
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

/**
 * @brief Show all matrix colors
 * 
 * @param matrix The matrix
 * @param rows The map matrix amount of rows
 * @param cols The map matrix amount of cols
 */
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

/**
 * @brief Free the map matrix
 * 
 * @param m map object
 */
void free_map(Map *m)
{
    for(int i = 0; i < m->rows; ++i)
    {
        free(m->map[i]);
    }
    free(m->map);
}

/**
 * @brief Find the frontier from some region with some color
 * 
 * @param m The map object
 * @param l The region row
 * @param c The region col
 * @param atual_color The region color
 * @param region The region value
 * @param g The graph
 */
void frontier(Map **m, int r, int c, int atual_color, int region, Graph *g)
{
    //if we find some position with color as atual_color, this is inside a region
    if ((*m)->map[r][c].color == atual_color)
    {
        total++;

        //change region color to its negative form to we know that we visited it
        (*m)->map[r][c].color = -(*m)->map[r][c].color;
        //if the position element region is -1, we not visited it yet, so this element region will be region
        if ((*m)->map[r][c].region == -1)
        {
            (*m)->map[r][c].region = region;
        }

        //recursively find all others regions
        if ((*m)->rows > r + 1)
        {
            frontier(m, r + 1, c, atual_color, region, g);
        }
        if ((*m)->cols > c + 1)
        {
            frontier(m, r, c + 1, atual_color, region, g);
        }
        if (r > 0)
        {
            frontier(m, r - 1, c, atual_color, region, g);
        }
        if (c > 0)
        {
            frontier(m, r, c - 1, atual_color, region, g);
        }
    }
    //if the color is not the atual color and not equal its negative, we found another region
    else if ((*m)->map[r][c].color != -atual_color)
    {
        //if the graph parameter was passed, is time to add this new region as a new graph vertice,
        //and add it to the adjacency list of the first region that called this function (and vice versa)
        if (g)
        {
            if ((*m)->map[r][c].color > 0)
            {
                total_regions++;

                int save_region = region;

                //if this position is in a region yet, dont change it
                if ((*m)->map[r][c].region > -1)
                {
                    region = (*m)->map[r][c].region;
                }
                //if not, update it
                else
                {
                    atual_region += 1;
                    region = atual_region;
                }

                int save_total = total;
                total = 0;
                
                frontier(m, r, c, (*m)->map[r][c].color, region, NULL);

                int pos[2];
                pos[0] = r;
                pos[1] = c;

                add_edge(
                    &g,
                    save_region,
                    module(atual_color),
                    save_total,
                    region,
                    module((*m)->map[r][c].color),
                    total,
                    pos
                );

                total = save_total;
                region = save_region;
            }
        }
    }
}

/**
 * @brief Transform a 2D matrix to a graph
 * 
 * @param map The map object
 * @return Graph* 
 */
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
        Vertice *aux = g->array[i].head->next;
        while(aux != NULL)
        {
            total = 0;
            frontier(&map, aux->pos[0], aux->pos[1], aux->color, aux->region, g);
            reset_map(&map);

            region = total_regions + 1;
            total_regions = 0;

            aux = aux->next;
        }
    }
    // show_graph(g);
    // printf("------------------\n");
    // show_matrix(map->map, map->rows, map->cols);
    // printf("------------------\n");
    //printf("%d\n", g->num_v);

    return g;
}