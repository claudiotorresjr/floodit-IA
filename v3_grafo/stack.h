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

typedef struct PositionQueue
{
    int size;
    struct Position *top;
} PositionQueue;

Index **allocate_matrix(int rows, int cols);
void push(PositionQueue *queue, Position *s);
Position *pop(PositionQueue *queue);

#endif