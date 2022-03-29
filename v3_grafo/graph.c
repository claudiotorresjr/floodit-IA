#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"
#include "main.h"

int distance_between_nodes(Graph *g, int from, int to)
{
        
}

void initialize_array(int **colors, int size, int v)
{
    for (int i = 0; i < size; ++i)
    {
        (*colors)[i] = v;
    }
}

Vertice *create_vertice(int size, int color, int region, int pos[2])
{
    Vertice *v = (Vertice *)malloc(sizeof(Vertice));

    v->size = size;
    v->color = color;
    v->region = region;
    v->visited = 0;
    v->pos[0] = pos[0];
    v->pos[1] = pos[1];
    // printf("criando vertice na pos %d-%d\n", v->pos[0], v->pos[1]);
    v->next = NULL;

    return v;
}

//verifica se a tem aresta com b
int has_edge(Graph *g, int a, int b)
{
    Vertice *aux;

    int i = 0;
    if (g->array[a].head)
    {
        aux = g->array[a].head->next;
        while (aux != NULL)
        {
            //se vertice b ta na lista de a, retorna true (tem aresta)
            if (aux->region == b)
            {
                return 1;
            }
            aux = aux->next;
        }
    }

    return 0;
}

void add_edge(Graph *g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2])
{
    Vertice *vertice;
    int i = 0;
    int found = 0;

    // printf("tentando add a pos %d %d\n", pos[0], pos[1]);

    //verifica se tem uma aresta entre a e b
    if (has_edge(g, a, b))
    {
        return;
    }
    
    if (!g->array[a].head)
    {
        // printf("criando vertice a = %d\n", a);
        g->num_v++;
        vertice = create_vertice(size_a, color_a, a, pos);
        g->array[a].head = vertice;

        found = 1;
    }
    if (g->array[a].head)
    {
        // printf("criando vertice b = %d e colocando no a = %d\n", b, a);
        g->array[a].head->size = size_a;
        vertice = create_vertice(size_b, color_b, b, pos);
        Vertice *aux = g->array[a].head;
        while (aux->next)
        {
            aux = aux->next;
        }

        aux->next = vertice;
        found = 1;
    }
}

Graph *create_graph(int size)
{
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->num_v = 0;
    g->num_e = 0;

    g->array = (AdjList *)malloc(size*sizeof(AdjList));
    for (int i = 0; i < size; ++i)
    {
        g->array[i].head = NULL;
    }

    g->array->visiteds = 0;
    
    return g;
}

void show_graph(Graph *g)
{    
    printf("---- lista adj----\n");
    for (int i = 0; i < g->num_v; ++i)
    {
    
        Vertice *aux = g->array[i].head;
        printf("PAI %d: ", i);
        while(aux != NULL)
        {
            printf("region %d (pos = %d-%d, cor = %d, quantidade = %d)\n", aux->region, aux->pos[0], aux->pos[1], aux->color, aux->size);
            aux = aux->next;
        }
        printf("------------------------\n");
    }

}