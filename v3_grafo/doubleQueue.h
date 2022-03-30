#ifndef _DOUBLEQUEUE_H
#define _DOUBLEQUEUE_H

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


typedef struct DoubleQueue
{
    int size;
    struct State *head;
    struct State *tail;
} DoubleQueue;

DoubleQueue *dq_create();
void dq_show_list(DoubleQueue *queue);
int dq_empty(DoubleQueue *queue);
void dq_insert_head(DoubleQueue *queue, State *s);
State *dq_remove_front(DoubleQueue *queue);
void dq_insert_tail(DoubleQueue *queue, State *s);
State *dq_remove_tail(DoubleQueue *queue);

#endif