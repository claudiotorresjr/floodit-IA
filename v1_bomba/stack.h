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

typedef struct PositionQueue
{
    struct State *top;
} PositionQueue;

int **allocateMatrix(int rows, int cols);
State *create_state(Map *m);
void copy_matrix(Map *m1, Map *m2);
void push(PositionQueue *queue, State *s);
State *pop(PositionQueue *queue);

#endif