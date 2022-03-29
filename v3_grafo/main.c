#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "main.h"
#include "graph.h"
#include "stack.h"

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

void paintOneColor(Map **m, int rows, int cols, int r, int c, int init_c, int color)
{
    (*m)->map[r][c].color = color;
    if (rows - 1 > r && (*m)->map[r + 1][c].color == init_c)
    {
        paintOneColor(m, rows, cols, r + 1, c, init_c, color);
    }
    if (cols - 1 > c && (*m)->map[r][c + 1].color == init_c)
    {
        paintOneColor(m, rows, cols, r, c + 1, init_c, color);
    }
    if (r > 0 && (*m)->map[r - 1][c].color == init_c)
    {
        paintOneColor(m, rows, cols, r - 1, c, init_c, color);
    }
    if (c > 0 && (*m)->map[r][c - 1].color == init_c)
    {
        paintOneColor(m, rows, cols, r, c - 1, init_c, color);
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

    // int *regions = (int *)malloc((1)*sizeof(int));
    // regions[0] = 0;
    // State *first = create_state(0, 1, regions);

    // StateQueue *queue = (StateQueue *)malloc(sizeof(StateQueue));
    // queue->top = NULL;

    // push(queue, first);
    // g->array[0].head->visited = 1;
    g->array->visiteds++;

    int stop = 0;
    int anterior = 0;

    // distance_between_nodes(g, 0, 1);

    int *colors = (int *)calloc(((map->n_colors)+1), sizeof(int));

    for (int c = 1; c <= map->n_colors; ++c)
    {
        printf("pintando com a cor: %d\n", c);

        if (c == g->array[0].head->color)
        {
            continue;
        }

        int total_distance = 0;
        for (int i = 1; i < g->num_v; ++i)
        {
            printf("Buscando região %d\n", i);
            int atual_v_distance = g->num_e;
            
            for (int j = 0; j < g->num_v; ++j)
            {
                g->array[j].head->visited = 0;
            }

            StateQueue *lifo = (StateQueue *)malloc(sizeof(StateQueue));
            lifo->top = NULL;

            State *s = create_state(g->array[0].head->region, 0, g->array[0].head->color);
            g->array[0].head->color = c;
            s->distance = 0;
            // g->array[0].head->visited = 1;

            push(lifo, s);

            while(lifo->top)
            {
                State *current = pop(lifo);
                g->array[current->region].head->visited = 2;
                printf("-----pop na regiao: %d\n", current->region);

                if (current->region == i)
                {   
                    if (i == 4)
                        printf("distancia 0 ao %d == %d\n", i, current->distance);
                    if (current->distance < atual_v_distance)
                    {
                        atual_v_distance = current->distance;
                    }
                }

                Vertice *aux = g->array[current->region].head->next;
                // printf("vendo lista da regiao %d\n", current->region);
                int distance;
                int atual_color = g->array[current->region].head->color;
                while(aux != NULL)
                {
                    distance = 0;
                    if (g->array[aux->region].head->visited == 0)
                    {
                        printf("-> push em %d\n", aux->region);
                        if(aux->color != atual_color)
                        {
                            distance = 1 + current->distance;
                        }
                        
                        State *s = create_state(aux->region, 0, aux->color);
                        g->array[aux->region].head->visited = 2;
                        s->distance = distance;

                        push(lifo, s);
                    }
                    aux = aux->next;
                }
                // g->array[current->region].head->visited = 0;
            }
            // printf("distancia 0 ao %d == %d\n", i, atual_v_distance);
        }
        colors[c] = total_distance;
        break;
    }

    // int min_distance = map->rows*map->cols;
    // int color = 0;
    // for (int d = 1; d < (map->n_colors)+1; ++d)
    // {
    //     if (colors[d] < min_distance)
    //     {
    //         min_distance = colors[d];
    //         color = d;
    //     }
    // }

    // printf("para cor %d: %d\n", color, min_distance);
    // solution->colors[solution->steps++] = color;

    // print_solution(solution);

    return 0;
}
