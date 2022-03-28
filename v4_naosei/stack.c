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

State *create_state(int parent, int size, int *regions)
{
    State *s = (State *)malloc(sizeof(State));

    s->parent = parent;
    s->size = size;
    s->regions = regions;

    return s;
}

void push(StateQueue *queue, State *s)
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

State *pop(StateQueue *queue)
{
    if (!queue->top)
    {
        return NULL;
    }

    State *aux = queue->top;
    queue->top = queue->top->prev;

    State *r = create_state(aux->parent, aux->size, aux->regions);

    free(aux);

    return r;
}