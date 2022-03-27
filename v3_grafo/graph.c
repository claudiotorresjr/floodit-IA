#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"
#include "main.h"

Vertice *create_vertice(int size, int color, int region, int pos[2])
{
    Vertice *v = (Vertice *)malloc(sizeof(Vertice));

    v->size = size;
    v->color = color;
    v->region = region;
    v->pos[0] = pos[0];
    v->pos[1] = pos[1];
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
    // while (g->array[i].head != NULL)
    // {
    //     //head da lista eh igual ao vertice a
    //     if (present_in_queue(g->array[i].head, a, a))
    //     {
    //         aux = g->array[i].head->next;
    //         while (aux != NULL)
    //         {
    //             //se vertice b ta na lista de a, retorna true (tem aresta)
    //             if (present_in_queue(aux, b, b))
    //             {
    //                 return 1;
    //             }
    //             aux = aux->next;
    //         }
    //         return 0;
    //     }
    //     ++i;
    // }
    return 0;
}

void add_edge(Graph *g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2])
{
    Vertice *vertice;
    int i = 0;
    int found = 0;

    // printf("tentando add no a = %d o b = %d\n", a, b);

    //verifica se tem uma aresta entre a e b
    if (has_edge(g, a, b))
    {
        return;
    }
    
    if (!g->array[a].head)
    {
        g->num_v++;
        vertice = create_vertice(size_a, color_a, a, pos);
        g->array[a].head = vertice;

        found = 1;
    }
    if (g->array[a].head)
    {
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
    //inserindo vertice b na lista do vertice a
    // while (!found)
    // {
    //     //se a lista ta vazia, insere o a primeiro
    //     if (!g->array[i].head)
    //     {
    //         g->num_v++;
    //         vertice = create_vertice(size_a, color_a, a);
    //         g->array[i].head = vertice;
    //         g->array[i].head->grau = 0;

    //         found = 1;
    //     }

    //     //se o a tem lista, adiciona o b nela
    //     if (present_in_queue(g->array[i].head, a, a))
    //     {
    //         g->array[i].head->size = size_a;
    //         vertice = create_vertice(size_b, color_b, b);
    //         Vertice *aux = g->array[i].head;
    //         while (aux->next)
    //         {
    //             aux = aux->next;
    //         }

    //         aux->next = vertice;

    //         g->array[i].head->grau++;
    //         found = 1;
    //     }
    //     ++i;
    // }

    if (found == 1)
    {
        g->num_e++;
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
            printf("region %d (cor = %d, quantidade = %d)\n", aux->region, aux->color, aux->size);
            aux = aux->next;
        }
        printf("------------------------\n");
    }

}