#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

typedef struct State
{
    int color;
    int distance;
    struct State *prev;
} State;

typedef struct PositionQueue
{
    struct State *top;
} PositionQueue;

int **allocate_matrix(int rows, int cols);
State *create_state(int color);
int **copy_matrix(Map *m);
void push(PositionQueue *queue, State *s);
State *pop(PositionQueue *queue);

#endif