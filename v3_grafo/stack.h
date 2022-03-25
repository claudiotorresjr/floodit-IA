#ifndef _STACK_H
#define _STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "main.h"

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

int **allocateMatrix(int rows, int cols);
int **copy_matrix(Map *m);
void push(PositionQueue *queue, Position *s);
Position *pop(PositionQueue *queue);

#endif