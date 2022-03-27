#include <stdlib.h>

#include "stack.h"
#include "main.h"

Index **allocate_matrix(int rows, int cols)
{
    Index **matrix = (Index **)malloc(rows * sizeof(Index *));
    for (int i = 0; i < rows; ++i)
    {
        matrix[i] = (Index *)malloc(cols * sizeof(Index));
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