#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"
#include "main.h"

Vertice *create_vertice(int size, int color, int pos[2])
{
    Vertice *v = (Vertice *)malloc(sizeof(Vertice));

    v->state = 0;
    v->size = size;
    v->color = color;
    v->pos[0] = pos[0];
    v->pos[1] = pos[1];
    v->next = NULL;
    v->parent = NULL;

    return v;
}

//verifica se a tem aresta com b
int has_edge(Graph *g, int a[2], int b[2])
{
    Vertice *aux;

    int i = 0;
    while (g->array[i].head != NULL)
    {
        //head da lista eh igual ao vertice a
        if (g->array[i].head->pos[0] == a[0] && g->array[i].head->pos[1] == a[1])
        {
            aux = g->array[i].head->next;
            while (aux != NULL)
            {
                //se vertice b ta na lista de a, retorna true (tem aresta)
                if (aux->pos[0] == b[0] && aux->pos[1] == b[1])
                {
                    return 1;
                }
                aux = aux->next;
            }
            return 0;
        }
        ++i;
    }
    return 0;
}

void add_edge(Graph *g, int a[2], int color_a, int size_a, int b[2], int color_b, int size_b)
{
    Vertice *vertice;
    int i = 0;
    int found = 0;

    //verifica se tem uma aresta entre a e b
    if (has_edge(g, a, b))
    {
        return;
    }

    //inserindo vertice b na lista do vertice a
    while (!found)
    {
        //se a lista ta vazia, insere o a primeiro
        if (!g->array[i].head)
        {
            g->num_v++;
            vertice = create_vertice(size_a, color_a, a);
            g->array[i].head = vertice;
            g->array[i].head->grau = 0;

            found = 1;
        }

        //se o a tem lista, adiciona o b nela
        if (g->array[i].head->pos[0] == a[0] && g->array[i].head->pos[1] == a[1])
        {
            vertice = create_vertice(size_b, color_b, b);
            Vertice *aux = g->array[i].head;
            while (aux->next)
            {
                aux = aux->next;
            }

            aux->next = vertice;

            g->array[i].head->grau++;
            found = 1;
        }
        ++i;
    }

    //adicionar o pai ou nao??

    //inserindo vertice a na lista do vertice b
    // i = 0;
    // found = 0;
    // while (!found)
    // {
    //     //se a lista ta vazia, insere o b primeiro
    //     if (!g->array[i].head)
    //     {
    //         g->num_v++;
    //         vertice = create_vertice(size_b, color_b, b);
    //         g->array[i].head = vertice;
    //         g->array[i].head->grau = 0;

    //         found = 1;
    //     }

    //     //se o b tem lista, adiciona o a nela
    //     if (g->array[i].head->pos[0] == b[0] && g->array[i].head->pos[1] == b[1])
    //     {
    //         vertice = create_vertice(size_a, color_a, a);
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