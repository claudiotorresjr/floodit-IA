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
            printf("%d ", matrix[i][j].region);
        }
        printf("\n");
    }
}

int isSolved(int **m, int r, int c)
{
    int first_c = m[0][0];
    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j < c; ++j)
        {
            if (m[i][j] != first_c)
            {
                return 0;
            }
        }
    }

    return 1;    
}

void paintOneColor(int ***m, int rows, int cols, int r, int c, int init_c, int color)
{
    (*m)[r][c] = color;
    if (rows - 1 > r && (*m)[r + 1][c] == init_c)
    {
        paintOneColor(m, rows, cols, r + 1, c, init_c, color);
    }
    if (cols - 1 > c && (*m)[r][c + 1] == init_c)
    {
        paintOneColor(m, rows, cols, r, c + 1, init_c, color);
    }
    if (r > 0 && (*m)[r - 1][c] == init_c)
    {
        paintOneColor(m, rows, cols, r - 1, c, init_c, color);
    }
    if (c > 0 && (*m)[r][c - 1] == init_c)
    {
        paintOneColor(m, rows, cols, r, c - 1, init_c, color);
    }
}

void frontier(Map **m, int l, int c, int atual_color, int region, Graph *g)
{
    if ((*m)->map[l][c].color == atual_color)
    {
        total++;

        // Position *p = (Position *)malloc(sizeof(Position));
        // p->l = l;
        // p->c = c;
        // p->v = module((*m)->map[l][c].color);
        // push(region, p);

        (*m)->map[l][c].color = -(*m)->map[l][c].color;
        if ((*m)->map[l][c].region == -1)
        {
            (*m)->map[l][c].region = region;
        }
        if ((*m)->rows - 1 > l)
            frontier(m, l + 1, c, atual_color, region, g);
        if ((*m)->cols - 1 > c)
            frontier(m, l, c + 1, atual_color, region, g);
        if (l > 0)
            frontier(m, l - 1, c, atual_color, region, g);
        if (c > 0)
            frontier(m, l, c - 1, atual_color, region, g);
    }
    else if ((*m)->map[l][c].color != -atual_color)
    {
        if (g)
        {
            if ((*m)->map[l][c].color > 0)
            {
                clock_t start, end;
                double cpu_time_used;
                // printf("COR %d regiao %d,%d:\n", (*m)->map[l][c].color, l, c);
                int queue = 0;

                int save_total = total;
                total = 0;

                if ((*m)->map[l][c].region > -1)
                    return;

                    total_regions++;

                int save_region = region;
                region = total_regions;
                frontier(m, l, c, (*m)->map[l][c].color, region, NULL);

                int pos[2];
                pos[0] = l;
                pos[1] = c;

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
    PositionQueue *region_queue = (PositionQueue *)malloc(sizeof(PositionQueue));
    region_queue->top = NULL;

    Graph *g = create_graph(map->rows*map->cols);

    // int reg = 0;
    // for (int i = 0; i < map->rows; ++i)
    // {
    //     for (int j = 0; j < map->cols; ++j)
    //     {
    //         //if (map->map[i][j].region == atual_region + 1)
    //         {   
    //             printf("%d %d\n", i, j);
    //             total = 0;
    //             frontier(&map, i, j, map->map[i][j].color, region_queue, g);
    //             reset_map(&map);
    //             show_matrix(map->map, map->rows, map->cols);
    //             printf("------------------\n");

    //             atual_region = total_regions + 1;
    //             total_regions = 0;
    //         }
    //     }
    // }

    total = 0;
    frontier(&map, 0, 0, map->map[0][0].color, 0, g);
    reset_map(&map);

    int region = 1;
    for (int i = 0; i < g->num_v; ++i)
    {
        // start = clock();
    
        Vertice *aux = g->array[i].head->next;
        // printf("%d\n", i);
        while(aux != NULL)
        {
            // PositionQueue *region_queue = (PositionQueue *)malloc(sizeof(PositionQueue));
            // region_queue->top = NULL;

            // printf("procurando regiao %d\n", region);
            // if (aux->region == i)
            //     continue;
            frontier(&map, aux->pos[0], aux->pos[1], aux->color, region, g);
            reset_map(&map);
            region = total_regions + 1;
            // total_regions = 0;

            aux = aux->next;
        }
        if (i == 5)
        break;
        // end = clock();
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("fun() took %f seconds to execute \n", cpu_time_used);
    }
    show_graph(g);
    show_matrix(map->map, map->rows, map->cols);
    // printf("------------------\n");
    printf("%d\n", g->num_v);


    // Solution *solution = (Solution *)malloc(sizeof(Solution));
    // solution->steps = 0;
    // solution->colors = (int *)malloc(map->rows*map->cols*sizeof(int));

    // PositionQueue *queue = (PositionQueue *)malloc(sizeof(PositionQueue));
    // queue->top = NULL;

    // State *new = create_state(map->map[0][0]);
    // push(queue, new);

    // while (queue->top)
    // {
    //     State *current = pop(queue);

    //     int current_c = current->color;
    //     State **frontier = (State **)malloc((map->n_colors - 1)*sizeof(State *));
    //     int f_pos = 0;
    //     for (int i = 1; i <= map->n_colors; ++i)
    //     {
    //         if (i == current_c)
    //         {
    //             // printf("Nao pinte com a cor %paintOneColor
    //         }
    //         int **m_aux = copy_matrix(map);
    //         total = 0;
    //         paintOneColor(&m_aux, map->rows, map->cols, 0, 0, m_aux[0][0], i);
    //         frontier(&m_aux, map->rows, map->cols, 0, 0, m_aux[0][0]);
    //         if (isSolved(m_aux, map->rows, map->cols))
    //         {
    //             solution->colors[solution->steps++] = i;
    //             print_solution(solution);

    //             return 0;
    //         }
    //         // frontier[f_pos]->distance = count_color_region(m_aux, map->rows, map->cols);
    //         frontier[f_pos]->distance = total;
            
    //         f_pos++;
    //     }
    //     printf("-----------------------\n");
    //     int max = frontier[0]->distance;
    //     int color = frontier[0]->color;
    //     for (int i = 1; i < map->n_colors - 1; i++)
    //     {
    //         if (frontier[i]->distance > max)
    //         {
    //             max = frontier[i]->distance;
    //             color = frontier[i]->color;
    //         }
    //     }

    //     State *new = create_state(color);
    //     push(queue, new);

    //     paintOneColor(&map->map, map->rows, map->cols, 0, 0, map->map[0][0], color);
    //     solution->colors[solution->steps++] = color;
    // }
    // print_solution(solution);

    return 0;
}
