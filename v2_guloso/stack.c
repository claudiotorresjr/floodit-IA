#include <stdlib.h>

#include "stack.h"
#include "main.h"

int **allocateMatrix(int rows, int cols)
{
    int **matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    return matrix;
}

State *create_state(int color)
{
    State *new = (State *)malloc(sizeof(State));
    new->color = color;
    new->prev = NULL;    

    return new;
}

int **copy_matrix(Map *m)
{
    int **matrix = allocateMatrix(m->rows, m->cols);
    for (int i = 0; i < m->rows; ++i)
    {
        for (int j = 0; j <  m->cols; ++j)
        {
            matrix[i][j] = m->map[i][j];
        }
    }

    return matrix;
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

    State *r = create_state(aux->color);

    free(aux);

    return r;
}