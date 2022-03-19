#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "stack.h"

Map *createMap(FILE *file)
{
    Map *map = (Map *)malloc(sizeof(Map));
    fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->colors);
    map->map = allocateMatrix(map->rows, map->cols);

    for (int i = 0; i < map->rows; i++)
    {
        for (int j = 0; j < map->cols; j++)
        {
            fscanf(file, "%d ", &map->map[i][j]);
        }
    }

    return map;
}

void showMatrix(int **matrix, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int isSolved(Map *m)
{
    // showMatrix(m, lin, col);
    int first_c = m->map[0][0];
    for (int i = 0; i < m->rows; ++i)
    {
        for (int j = 0; j < m->cols; ++j)
        {
            if (m->map[i][j] != first_c)
            {
                return 0;
            }
        }
    }

    return 1;    
}

void print_moves(int *moves)
{
    int cont = 0;
    for (int i = 0; i < 10; i++)
    {
        if (moves[i] == -1)
        {
            break;
        }
        cont++;
    }
    printf("%d\n", cont);

    int i;
    for (i = 0; i < 10 - 1; i++)
    {
        if (moves[i+1] == -1)
        {
            break;
        }
        printf("%d ", moves[i]);
    }
    printf("%d", moves[i]);
    printf("\n");    
}

void paintOneColor(Map **m, int lin, int col, int init_c, int color)
{
    (*m)->map[lin][col] = color;
    if ((*m)->rows - 1 > lin && (*m)->map[lin + 1][col] == init_c)
    {
        paintOneColor(m, lin + 1, col, init_c, color);
    }
    if ((*m)->cols - 1 > col && (*m)->map[lin][col + 1] == init_c)
    {
        paintOneColor(m, lin, col + 1, init_c, color);
    }
    if (lin > 0 && (*m)->map[lin - 1][col] == init_c)
    {
        paintOneColor(m, lin - 1, col, init_c, color);
    }
    if (col > 0 && (*m)->map[lin][col - 1] == init_c)
    {
        paintOneColor(m, lin, col - 1, init_c, color);
    }
}

void update_moves(int *v1, int *v2, int color)
{
    for (int i = 0; i < 10; ++i)
    {
        v1[i] = v2[i];
        if (v1[i] == -1)
        {
            v1[i] = color;
            break;
        }
    }
}

int main(int argc, char const *argv[])
{
    FILE *map_file = stdin;
    if (map_file == NULL)
    {
        fprintf(stderr, "File not found. Finishing program.\n");
        exit(1);
    }

    Map *map = createMap(map_file);

    // showMatrix(map->map, map->rows, map->cols);

    StateQueue *queue = (StateQueue *)malloc(sizeof(StateQueue));
    queue->top = NULL;

    int rows = map->rows;
    int cols = map->cols;
    State *new = create_state(map);
    copy_matrix(new->map, map);
    push(queue, new);

    while (queue->top)
    {
        State *current = pop(queue);

        int current_c = current->map->map[0][0];
        for (int i = 1; i <= map->colors; ++i)
        {
            if (i == current_c)
            {
                // printf("Nao pinte com a cor %d\n", i);
                continue;
            }
            State *next = create_state(current->map);
            copy_matrix(next->map, current->map);

            update_moves(next->moves, current->moves, i);

            printf("pintando com a cor %d\n", i);
            paintOneColor(&next->map, 0, 0, next->map->map[0][0], i);
            if (isSolved(next->map))
            {
                // printf("Solucionado :D\n");
                print_moves(next->moves);
                return 0;
            }
            showMatrix(next->map->map, next->map->rows, next->map->cols);
            sleep(1);

            push(queue, next);
        }
        printf("------------------------------\n");
    }

    // paintOneColor(&map, 0, 0, map->map[0][0], 1);

    // printf("Before painting with color 1...\n");
    // showMatrix(map->map, map->rows, map->cols);
    // paintOneColor(&map, 0, 0, map->map[0][0], 3);

    // printf("Before painting with color 3...\n");
    // showMatrix(map->map, map->rows, map->cols);
    // paintOneColor(&map, 0, 0, map->map[0][0], 1);

    // printf("Before painting with color 1...\n");
    // showMatrix(map->map, map->rows, map->cols);
    // paintOneColor(&map, 0, 0, map->map[0][0], 2);

    // printf("Before painting with color 2...\n");
    // showMatrix(map->map, map->rows, map->cols);
    // paintOneColor(&map, 0, 0, map->map[0][0], 1);

    // printf("Before painting with color 1...\n");
    // showMatrix(map->map, map->rows, map->cols);

    return 0;
}
