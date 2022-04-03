/**
 * @file doubleQueue.h
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief All structures and functions used in doubleQueue.c
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#ifndef _DOUBLEQUEUE_H
#define _DOUBLEQUEUE_H

#include <stdio.h>
#include <stdlib.h>

#include "solver.h"

/**
 * @brief Struct that represents a state
 * 
 */
typedef struct State
{
    int region; //region value
    int color; //region color
    int distance; //region distance from root
    struct State *prev; //state previous node
    struct State *next; //state next node
} State;

/**
 * @brief Struct that represents the double-ended queue
 * 
 */
typedef struct DoubleQueue
{
    int size; //double-ended queue size
    struct State *head; //double-ended queue head
    struct State *tail; //double-ended queue tail
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