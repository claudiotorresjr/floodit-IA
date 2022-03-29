#ifndef _DOUBLEQUEUE_H
#define _DOUBLEQUEUE_H

#include <stdio.h>
#include <stdlib.h>

#include "stack.h"


typedef struct DoubleQueue
{
    int size;
    struct State *first;
    struct State *last;
} DoubleQueue;


#endif