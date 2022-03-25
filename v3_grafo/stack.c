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

void push(PositionQueue *queue, Position *s)
{    
    if(!queue->top)
    {
        s->prev = NULL;
        queue->size = 1;
    }
    else
    {
        s->prev = queue->top;
        queue->size++;
    }
    queue->top = s;
}

Position *pop(PositionQueue *queue)
{
    if (!queue->top)
    {
        return NULL;
    }

    Position *aux = queue->top;
    queue->top = queue->top->prev;

    Position *r = NULL;

    free(aux);

    return r;
}