#include "doubleQueue.h"

DoubleQueue *dq_create()
{
    DoubleQueue *queue = (DoubleQueue *)malloc(sizeof(DoubleQueue));

    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void dq_show_list(DoubleQueue *queue)
{
    State *aux = queue->head;
    while (aux)
    {
        printf("(reg: %d, cor: %d) -> ", aux->region, aux->color);
        aux = aux->next;
    }
    printf("\n");    
}

int dq_empty(DoubleQueue *queue)
{
    if (!queue->head && !queue->tail)
    {
        return 1;
    }

    return 0;
}

void dq_insert_head(DoubleQueue *queue, int region, int color, int distance)
{
    State *s = create_state(region, color);
    s->distance = distance;

    if (dq_empty(queue))
    {
        queue->head = s;
        queue->tail = s;
    }
    else{
        State *aux = queue->head;

        s->next = aux;
        aux->prev = s;

        queue->head = s;
    }

    queue->size++;
}

State *dq_remove_head(DoubleQueue *queue)
{
    if (!queue->head)
    {
        return NULL;
    }

    State *aux = queue->head;
    queue->head = aux->next;

    if (queue->head)
    {
        queue->head->prev = NULL;
    }
    else
    {
        queue->tail = NULL;
    }

    queue->size--;

    aux->next = NULL;
    aux->prev = NULL;

    return aux;
    
}

void dq_insert_tail(DoubleQueue *queue, int region, int color, int distance)
{
    State *s = create_state(region, color);
    s->distance = distance;

    if (dq_empty(queue))
    {
        queue->head = s;
        queue->tail = s;
    }
    else{
        State *aux = queue->tail;

        s->prev = aux;
        aux->next = s;

        queue->tail = s;
    }

    queue->size++;
}

State *dq_remove_tail(DoubleQueue *queue)
{
    if (dq_empty(queue))
    {
        return NULL;
    }

    State *aux = queue->tail;
    queue->tail = aux->prev;
    queue->tail->next = NULL;

    queue->size--;

    aux->next = NULL;
    aux->prev = NULL;

    return aux;
}