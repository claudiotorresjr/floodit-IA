/**
 * @file doubleQueue.c
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief File with doubleQueue.h functions implementation. All functions related to the double-ended queue
 * are present here.
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#include "../include/doubleQueue.h"

/**
 * @brief Create a state object for the search
 * 
 * @param region The region of the state (the region of the color in the graph)
 * @param color The color of the state
 * @return State* - The new state object
 */
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

/**
 * @brief Create the double-ended queue 
 * 
 * @return DoubleQueue* - The double-ended queue
 */
DoubleQueue *dq_create()
{
    DoubleQueue *queue = (DoubleQueue *)malloc(sizeof(DoubleQueue));

    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

/**
 * @brief Show the double-ended queue
 * 
 * @param queue The double-ended queue
 */
void dq_show_list(DoubleQueue *queue)
{
    State *aux = queue->head;
    while (aux)
    {
        printf("(region: %d, color: %d) -> ", aux->region, aux->color);
        aux = aux->next;
    }
    printf("\n");    
}

/**
 * @brief Check if double-ended queue is empty
 * 
 * @param queue The double-ended queue
 * @return int - 1 if empty or 0 if not empty
 */
int dq_empty(DoubleQueue *queue)
{
    if (!queue->head && !queue->tail)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief Insert new states (nodes) at double-ended queue head
 * 
 * @param queue The double-ended queue
 * @param region The node region
 * @param color The node color
 * @param distance The node distance (region distance from root)
 */
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

/**
 * @brief Remove and return the first node from double-ended queue head
 * 
 * @param queue The double-ended queue
 * @return State* - first double-ended queue node
 */
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

/**
 * @brief Insert new states (nodes) at double-ended queue tail 
 * 
 * @param queue The double-ended queue
 * @param region The node region
 * @param color The node color
 * @param distance The node distance (region distance from root)
 */
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

/**
 * @brief Remove and return the last node from double-ended queue tail
 * 
 * @param queue The double-ended queue
 * @return State* - last double-ended queue node
 */
State *dq_remove_tail(DoubleQueue *queue)
{
    if (!queue->tail)
    {
        return NULL;
    }

    State *aux = queue->tail;
    queue->tail = aux->prev;

    if (queue->tail)
    {
        queue->tail->next = NULL;
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