#include <stdlib.h>

#include "stack.h"
#include "main.h"

int **allocate_matrix(int rows, int cols)
{
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    return matrix;
}

State *create_state(Map *m)
{
    State *new = (State *)malloc(sizeof(State));
    new->map = (Map *)malloc(sizeof(Map));
    new->map->map = allocate_matrix(m->rows, m->cols);
    new->map->rows = m->rows;
    new->map->cols = m->cols;
    new->map->n_colors = m->n_colors;
    new->prev = NULL;

    for (int i = 0; i < 100; i++)
    {
        new->moves[i] = -1;
    }
    

    return new;
}

void copy_matrix(Map *m1, Map *m2)
{
    for (int i = 0; i < m1->rows; ++i)
    {
        for (int j = 0; j <  m1->cols; ++j)
        {
            m1->map[i][j] = m2->map[i][j];
        }
    }
}

void push(StateQueue *queue, State *s)
{    
    if(!queue->top)
    {
        s->prev = NULL;
    }
    else
    {
        s->prev = queue->top;
    }
    queue->top = s;
}

State *pop(StateQueue *queue)
{
    if (!queue->top)
    {
        return NULL;
    }

    State *aux = queue->top;
    queue->top = queue->top->prev;

    State *r = create_state(aux->map);

    copy_matrix(r->map, aux->map);
    for (int i = 0; i < 10; i++)
    {
        r->moves[i] = aux->moves[i];
    }

    r->distance = aux->distance;

    free(aux);

    return r;
}