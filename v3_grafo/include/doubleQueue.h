#ifndef _DOUBLEQUEUE_H
#define _DOUBLEQUEUE_H

#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

typedef struct State
{
    int region;
    int color;
    int distance;
    struct State *prev;
    struct State *next;
} State;

typedef struct DoubleQueue
{
    int size;
    struct State *head;
    struct State *tail;
} DoubleQueue;

State *create_state(int region, int color);
DoubleQueue *dq_create();
void dq_show_list(DoubleQueue *queue);
int dq_empty(DoubleQueue *queue);
void dq_insert_head(DoubleQueue *queue, int region, int color, int distance);
State *dq_remove_head(DoubleQueue *queue);
void dq_insert_tail(DoubleQueue *queue, int region, int color, int distance);
State *dq_remove_tail(DoubleQueue *queue);

#endif