/**
 * @file floodit.c
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief File with floodit.h implementation. The main function for floodit solver.
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/map.h"
#include "../include/graph.h"
#include "../include/solver.h"
#include "../include/doubleQueue.h"

/**
 * @brief The floodit main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char const *argv[])
{
    FILE *map_file = stdin;
    if (!map_file)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    Map *map = create_map(map_file);

    //check if map is already solved
    if (map_is_solved(map->map, map->rows, map->cols))
    {
        printf("0\n\n");
        return 0;
    }

    Graph *g = map_to_graph(map);

    //create the solution array
    Solution *solution = (Solution *)malloc(sizeof(Solution));
    solution->steps = 0;
    solution->colors = (int *)malloc(map->rows*map->cols*sizeof(int));

    free_map(map);

    DoubleQueue *deque = dq_create();

    dq_insert_head(deque, g->array[0].head->region, g->array[0].head->color, 0);

    while (deque->head)
    {
        State *current = dq_remove_head(deque);
        
        int *result = find_optimal_color(g, map->n_colors, current->distance);

        int color = result[0];
        int cost = result[1];

        paint_graph(&g, g->array[0].head, g->array[0].head->color, color, 1);
        reset_graph(&g);

        merge_nodes(&g, 0, g->array[0].head->color);
        reset_graph(&g);

        solution->colors[solution->steps++] = color;

        free(result);
        free(current);

        if(is_solved(g))
        {
            break;
        }

        // printf("%d\n", cost);

        dq_insert_head(deque, g->array[0].head->region, g->array[0].head->color, cost);
    }

    print_solution(solution);

    fclose(map_file);
    free_graph(g);
    free(deque);
    free(solution->colors);
    free(solution);
    free(map);

    return 0;
}
