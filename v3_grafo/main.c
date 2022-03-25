#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "graph.h"
#include "stack.h"

int total;

int module(int a)
{
    if (a < 0)
    {
        return -a;
    }

    return a;
}

Map *createMap(FILE *file)
{
    Map *map = (Map *)malloc(sizeof(Map));
    fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->n_colors);
    map->map = allocateMatrix(map->rows, map->cols);

    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            fscanf(file, "%d ", &map->map[i][j]);
        }
    }

    return map;
}

void showMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
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

int count_color_region(int **m, int rows, int cols)
{
    int first_c = m[0][0];
    int total = 0;

    int i, j;
    for (i = 0; i < rows; ++i)
    {
        for (j = 0; (m[i][j] == first_c && j < cols); ++j)
        {
            total++;
        }
        if (i + 1 < rows && m[i+1][0] != first_c && m[i+1][1] != first_c)
        {
            break;
        }
    }

    return total;
}

void fronteira(Map **m, int l, int c, int fundo, PositionQueue *ngb_queue)
{
    if ((*m)->map[l][c] == fundo)
    {
        total++;
        // printf("%d ", (*m)->map[l][c]);
        (*m)->map[l][c] = -(*m)->map[l][c];
        if ( (*m)->rows - 1 > l )
            fronteira(m, l + 1, c, fundo, ngb_queue);
        if ( (*m)->cols - 1 > c )
            fronteira(m, l, c + 1, fundo, ngb_queue);
        if ( l > 0 )
            fronteira(m, l - 1, c, fundo, ngb_queue);
        if ( c > 0 )
            fronteira(m, l, c - 1, fundo, ngb_queue);
    }
    else if ((*m)->map[l][c] != -fundo)
    {
        //posicoes que sao vizinhas da regiao encontrada no if
        // printf("else: (%d, %d) ", l, c);
        if (ngb_queue)
        {
            Position *p = (Position *)malloc(sizeof(Position));
            p->l = l;
            p->c = c;
            p->v = (*m)->map[l][c];
            push(ngb_queue, p);
        }
    }
}

void find_regions(Graph *g, PositionQueue *queue, int size_a, int r, int c, Map *m)
{
    PositionQueue *p = (PositionQueue *)malloc(sizeof(PositionQueue));

    Position *aux = queue->top;
    int a[2];
    a[0] = r;
    a[1] = c;
    int color_a = module(m->map[r][c]);

    int b[2];
    int color_b;
    while (aux)
    {
        total = 0;
        fronteira(&m, aux->l, aux->c, m->map[aux->l][aux->c], NULL);

        b[0] = aux->l;
        b[1] = aux->c;
        color_b = module(m->map[aux->l][aux->c]);

        add_edge(g, a, color_a, size_a, b, color_b, total);
        aux = aux->prev;
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
    FILE *map_file = stdin;
    if (map_file == NULL)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    Map *map = createMap(map_file);
    printf("-- pos 0,0 com cor 2... vizinhos:\n");
    PositionQueue *queue = (PositionQueue *)malloc(sizeof(PositionQueue));
    queue->top = NULL;

    //acha a fronteira da posicao 0,0
    total = 0;
    fronteira(&map, 0, 0, map->map[0][0], queue);


    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------

    Graph *g = create_graph(map->rows*map->cols);
    //regioes da fronteira anterior
    find_regions(g, queue, total, 0, 0, map);

    Position *aux = queue->top;
    int a[2] = {0, 0};
    int color_a = module(map->map[0][0]);

    // int b[2];
    // while (aux)
    // {
    //     b[0] = aux->l;
    //     b[1] = aux->c;
    //     add_edge(g, a, color_a, queue->size, b, 0, 0);
    //     aux = aux->prev;
    // }
    // printf("\n");

    printf("---- lista adj----\n");
    printf("---- (x, y) - [color, color_count]----\n");
    for (int i = 0; i < g->num_v; ++i)
    {
    
        Vertice *aux = g->array[i].head;
        while(aux != NULL)
        {
            printf("(%d, %d) - [%d - %d] --> ", aux->pos[0], aux->pos[1], aux->color, aux->size);
            aux = aux->next;
        }
        printf("\n");
    }


    // showMatrix(map->map, map->rows, map->cols);

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
    //         fronteira(&m_aux, map->rows, map->cols, 0, 0, m_aux[0][0]);
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
