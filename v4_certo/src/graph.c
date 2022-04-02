#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/graph.h"
#include "../include/stack.h"
#include "../include/map.h"
#include "../include/doubleQueue.h"

void paint_graph(Graph **g, Vertice *v, int base_color, int color, int change_color)
{
    // printf("pintando a reg: %d com cor %d\n", v->region, color);
    (*g)->array[v->region].head->visited = 1;
    (*g)->array[v->region].head->color = color;

    if (change_color)
    {
        (*g)->array[v->region].head->first_color = color;
    }

    Vertice *aux = v->next;
    while (aux)
    {
        // printf("---- verificando a reg: %d (%d) - %d == cor base %d\n", aux->region, aux->color, (*g)->array[aux->region].head->visited, base_color);
        if ((*g)->array[aux->region].head->color == base_color && !(*g)->array[aux->region].head->visited)
        {
            // printf("->> tem cor %d\n", base_color);
            paint_graph(g, (*g)->array[aux->region].head, base_color, color, change_color);
        }

        aux = aux->next;
    }
    // free(aux);
}

void remove_node_from_list(Vertice *head, int region)
{
    Vertice *v = head->next;

    while(v)
    {
        if (v->region == region)
        {
            if (v->next)
            {
                v->next->prev = v->prev;
            }
            v->prev->next = v->next;

            free(v);

            break;
        }

        v = v->next;
    }
}

void merge_nodes(Graph *g, int region, int color)
{
    // printf("-> iniciando com a regiao %d\n", region);
    int new_size = g->array[region].head->size;

    g->array[region].head->visited = 1;
    Vertice *aux = g->array[region].head->next;
    while (aux)
    {
        // printf(" --> verificando cor da regiao %d\n", aux->region);
        if(aux->color == color && !g->array[aux->region].head->visited)
        {
            g->array[aux->region].head->visited = 1;
            // printf("    --> eh igual\n");
            merge_nodes(g, aux->region, color);
            // printf("  (sai da recursao)\n");
            Vertice *child = g->array[aux->region].head->next;

            new_size += g->array[aux->region].head->size;
            // printf("        --> vendo filhos da regiao %d\n", aux->region);
            while (child)
            {
                if(g->array[child->region].head->color != color && !g->array[child->region].head->visited)
                {
                    add_edge(
                        g,
                        region,
                        color,
                        new_size,
                        g->array[child->region].head->region,
                        g->array[child->region].head->color,
                        g->array[child->region].head->size,
                        g->array[child->region].head->pos
                    );

                    add_edge(
                        g,
                        g->array[child->region].head->region,
                        g->array[child->region].head->color,
                        g->array[child->region].head->size,
                        region,
                        color,
                        g->array[region].head->size,
                        g->array[child->region].head->pos
                    );

                    // printf("Removendo da reg %d o nodo %d\n", g->array[child->region].head->region, aux->region);
                    remove_node_from_list(g->array[child->region].head, aux->region);
                }

                g->array[aux->region].head->next = child->next;

                if (g->array[aux->region].head->next)
                {
                    g->array[aux->region].head->prev = g->array[aux->region].head;
                }
                else
                {
                    g->array[aux->region].tail = g->array[aux->region].head;
                }

                free(child);

                child = g->array[aux->region].head->next;
            }

            aux->prev->next = aux->next;

            if (aux->prev->next)
            {
                 aux->prev->next->prev = aux->prev;
            }
            else
            {
                aux->prev = NULL;
                g->array[region].tail = aux->prev;
            }

            // printf("free na regiao %d, q eh %d\n", aux->region, g->array[aux->region].head->region);
            free_vertices_list(&g->array[aux->region]);

            g->array[aux->region].head = NULL;
            g->array[aux->region].tail = NULL;

            free(aux);

            aux = g->array[region].head->next;
        }
        else
        {
            aux = aux->next;
        }
    }
}

void reset_graph(Graph *g)
{
    for (int j = 0; j < g->num_v; ++j)
    {
        if (g->array[j].head)
        {
            g->array[j].head->visited = 0;
        }
    }
}

int distance_between_nodes(Graph *g, int c)
{
    int total_distance = 0;
    reset_graph(g);

    DoubleQueue *deque = dq_create();

    dq_insert_head(deque, g->array[0].head->region, g->array[0].head->color, 0);

    while(!dq_empty(deque))
    {
        State *current = dq_remove_head(deque);
        g->array[current->region].head->visited = 1;

        int current_region = g->array[current->region].head->region;

        // printf("-----removi a regiao: %d\n", current->region);

        Vertice *aux = g->array[current->region].head->next;
        // printf("vendo lista da regiao %d\n", current->region);
        int distance;
        int atual_color = g->array[current->region].head->color;
        while(aux != NULL)
        {
            // printf("    item com reg %d (%d)\n", aux->region, g->array[aux->region].head->visited);
            if (g->array[aux->region].head->visited == 0)
            {
                if(aux->parent == current_region && g->array[aux->region].head->color == atual_color)
                {
                    distance = 0;
                }
                else
                {
                    distance = 1 + current->distance;
                }

                if (distance)
                {
                    dq_insert_tail(deque, aux->region, aux->color, distance);
                }
                else
                {
                    dq_insert_head(deque, aux->region, aux->color, distance);
                }
                total_distance += distance;
                // printf(" --> distancia 0 ao %d == %d\n", aux->region, distance);
                g->array[aux->region].head->visited = 2;
            }
            aux = aux->next;
        }


        free(current);
        free(aux);
    }

    free(deque);

    return total_distance;
}

Vertice *create_vertice(int size, int color, int region, int parent, int pos[2])
{
    Vertice *v = (Vertice *)malloc(sizeof(Vertice));

    v->size = size;
    v->color = color;
    v->first_color = color;
    v->region = region;
    v->visited = 0;
    v->parent = parent;
    v->pos[0] = pos[0];
    v->pos[1] = pos[1];
    v->next = NULL;
    v->prev = NULL;

    return v;
}

int has_edge(Graph *g, int a, int b)
{
    Vertice *aux;

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
        vertice = create_vertice(size_a, color_a, a, a, pos);
        g->array[a].head = vertice;
        g->array[a].tail = vertice;

        found = 1;
    }
    if (g->array[a].head)
    {
        // printf("criando vertice b = %d e colocando no a = %d\n", b, a);
        g->array[a].head->size = size_a;
        vertice = create_vertice(size_b, color_b, b, a, pos);

        Vertice *aux = g->array[a].tail;
        vertice->prev = aux;
        aux->next = vertice;

        g->array[a].tail = vertice;
        found = 1;
    }

    if (found)
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
        g->array[i].tail = NULL;
    }

    g->array->visiteds = 0;

    return g;
}

void free_vertices_list(AdjList *l)
{
	if(!l->head)
	{
		return;
	}

	Vertice *aux = l->head;
    while (aux)
    {
        l->head = aux->next;

        if (l->head)
        {
            l->head->prev = NULL;
        }
        else
        {
            l->tail = NULL;
        }
        free(aux);
        aux = l->head;
    }
}

void free_graph(Graph *g)
{
    for (int i = 0; i < g->num_v; ++i)
    {
        free_vertices_list(&g->array[i]);
    }
    free(g->array);
    free(g);
}

void show_graph(Graph *g)
{
    printf("---- lista adj----\n");
    for (int i = 0; i < g->num_v; ++i)
    {

        Vertice *aux = g->array[i].head;
        if (!aux)
        {
            continue;
        }
        printf("PAI %d: ", i);
        while(aux != NULL)
        {
            printf("region %d (pos = %d-%d, cor = %d, quantidade = %d)\n", aux->region, aux->pos[0], aux->pos[1], aux->color, aux->size);
            aux = aux->next;
        }
        printf("------------------------\n");
    }

}