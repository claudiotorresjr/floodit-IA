#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "../include/map.h"
#include "../include/graph.h"
#include "../include/solver.h"
#include "../include/doubleQueue.h"

int main(int argc, char const *argv[])
{
    FILE *map_file = stdin;
    if (map_file == NULL)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    Map *map = create_map(map_file);

    Graph *g = map_to_graph(map);

    Solution *solution = (Solution *)malloc(sizeof(Solution));
    solution->steps = 0;
    solution->colors = (int *)malloc(map->rows*map->cols*sizeof(int));

    g->array->visiteds++;

    free_map(map);

    while (!is_solved(g))
    {
        int color = solve_floodit(g, map->n_colors);

        paint_graph(&g, g->array[0].head, g->array[0].head->color, color, 1);
        // reset_graph(g);
        
        merge_nodes(g, 0, g->array[0].head->color);
        reset_graph(g);

        // show_graph(g);
        // printf("#################################################\n");

        solution->colors[solution->steps++] = color;
        // break;
    }

    print_solution(solution);

    free_graph(g);
    free(solution->colors);
    free(solution);
    free(map);

    return 0;
}
