#include <stdlib.h>

#include "../include/stack.h"
#include "../include/map.h"

State *create_state(int region, int color)
{
    State *s = (State *)malloc(sizeof(State));

    s->region = region;
    s->distance = 0;
    s->color = color;
    s->prev = NULL;
    s->next = NULL;

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

    State *r = create_state(aux->region, aux->color);

    free(aux);

    return r;
}