#include "doubleQueue.h"

DoubleQueue *dq_create()
{
    DoubleQueue *queue = (DoubleQueue *)malloc(sizeof(DoubleQueue));

    queue->size = 0;
    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void dq_insert_front(DoubleQueue * queue, State *s)
{
    if (!queue->first && !queue->last)
    {
        queue->first = s;
        queue->last = s;
    }
    else{
        State *aux = queue->first;

        aux->prev = queue->first->prev;
        
    }

}