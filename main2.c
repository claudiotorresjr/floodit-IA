#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "main.h"
#include "stack.h"

Map *createMap(FILE *file)
{
    Map *map = (Map *)malloc(sizeof(Map));
    fscanf(file, "%d %d %d\n", &map->rows, &map->cols, &map->n_colors);
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

int compara_pos(int l1, int c1, int l2, int c2)
{
    if (l1 < l2)
        return 1;
    if (l1 > l2)
        return 0;
    if (c1 >= c2)
        return (c1 > c2);
    return 1;
}

void limpa_mapa(Map **m)
{
    for (int i = 0; i < (*m)->rows; ++i)
    {
        for (int j = 0; j < (*m)->cols; ++j)
        {
            if ((*m)->map[i][j] < 0)
            {
                (*m)->map[i][j] = -(*m)->map[i][j];
            }
        }
    }
}

void fronteira(Map **m, int l, int c, int fundo, Frontier *f)
{
    if ((*m)->map[l][c] == fundo)
    {
        (*m)->map[l][c] = -(*m)->map[l][c];
        if ( (*m)->rows - 1 > l )
            fronteira(m, l + 1, c, fundo, f);
        if ( (*m)->cols - 1 > c )
            fronteira(m, l, c + 1, fundo, f);
        if ( l > 0 )
            fronteira(m, l - 1, c, fundo, f);
        if ( c > 0 )
            fronteira(m, l, c - 1, fundo, f);
    }
    else if ( (*m)->map[l][c] != -fundo )
    {
        //printf("insere fronteira l: %d, c: %d, 0\n", l, c);
        f->pos[f->size].l = l;
        f->pos[f->size].c = c;
        f->pos[f->size].v = 0;
        ++f->size;
    }
}

void fronteira_mapa(Map **m, Frontier *f)
{
    f->size = 0;
    fronteira(m, 0, 0, (*m)->map[0][0], f);
    limpa_mapa(m);    
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
    
    //Criando ponteiro com as fronteiras
    Frontier *f = (Frontier *)malloc(sizeof(Frontier));
    f->size = 0;
    f->pos = (Position *)malloc(map->rows*map->cols*sizeof(Position));

    Solution *solution = (Solution *)malloc(sizeof(Solution));
    solution->steps = 0;
    solution->colors = (int *)malloc(map->rows*map->cols*sizeof(int));

    int *ncorfront = (int *)malloc((map->n_colors+1)*sizeof(int));
    int cor = map->map[0][0];

    //fronteira do mapa
    fronteira_mapa(&map, f);
    
    int ncor;
    while (f->size > 0)
    {
        for (int i = 1; map->n_colors >= i; ++i)
        {
            ncorfront[i] = 0;
        }

        //percorre todas as posicoes salvas na fronteira
        for (int ia = 0; f->size > ia; ++ia)
        {
            ncorfront[map->map[f->pos[ia].l][f->pos[ia].c]] += 1;
        }
        ncor = 0;
        
        for (int ib = 1; map->n_colors >= ib; ++ib)
        {
            if (ncorfront[ib] > ncor)
            {
                ncor = ncorfront[ib];
                cor = ib;
            }
        }
        paintOneColor(&map, 0, 0, map->map[0][0], cor);
        solution->colors[solution->steps++] = cor;
        fronteira_mapa(&map, f);
    }

    printf("%d\n", solution->steps);
    for (int i = 0; i < solution->steps; i++)
    {
        printf("%d ", solution->colors[i]);
    }
    printf("\n");

    return 0;
}