#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

typedef struct State
{
    Map *map;
    int distance;
    int moves[100];
    struct State *prev;
} State;

typedef struct StateQueue
{
    struct State *top;
} StateQueue;

int **allocate_matrix(int rows, int cols);
State *create_state(Map *m);
void copy_matrix(Map *m1, Map *m2);
void push(StateQueue *queue, State *s);
State *pop(StateQueue *queue);

#endif