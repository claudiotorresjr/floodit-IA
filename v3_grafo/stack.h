#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Index
{
    int region;
    int color;
} Index;

typedef struct Position
{
    int l;
    int c;
    int v;
    struct Position *prev;
} Position;

typedef struct State
{
    int region;
    int color;
    int distance;
    struct State *prev;
    struct State *next;
} State;

typedef struct StateQueue
{
    int size;
    struct State *top;
} StateQueue;

Index **allocate_matrix(int rows, int cols);
State *create_state(int region, int color);
void push(StateQueue *queue, State *s);
State *pop(StateQueue *queue);

#endif