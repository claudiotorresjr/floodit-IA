#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"
#include "main.h"
#include "doubleQueue.h"

void reset_graph(Graph *g)
{
    for (int j = 0; j < g->num_v; ++j)
    {
        g->array[j].head->visited = 0;
    }
}

int distance_between_nodes(Graph *g, int c)
{
    int total_distance = 0;
    for (int i = 1; i < g->num_v; ++i)
    {
        // printf("Buscando região %d\n", i);

        reset_graph(g);

        DoubleQueue *deque = dq_create();
        
        dq_insert_head(deque, g->array[0].head->region, g->array[0].head->color, 0);

        while(!dq_empty(deque))
        {
            State *current = dq_remove_head(deque);
            g->array[current->region].head->visited = 1;
            // printf("-----removi a regiao: %d\n", current->region);

            if (current->region == i)
            {   
                // printf(" --> distancia 0 ao %d == %d\n", i, current->distance);
                total_distance += current->distance;
                break;
            }

            Vertice *aux = g->array[current->region].head->next;
            // printf("vendo lista da regiao %d\n", current->region);
            int distance;
            int atual_color = g->array[current->region].head->color;
            while(aux != NULL)
            {
                distance;

                // printf("    item com reg %d (%d)\n", aux->region, g->array[aux->region].head->visited);
                if (g->array[aux->region].head->visited == 0)
                {
                    if(aux->parent == g->array[current->region].head->region && g->array[aux->region].head->color == atual_color)
                    {
                        distance = 0;
                    }
                    else
                    {
                        distance = 1 + current->distance;
                    }
                    
                    // printf("distancia pro %d == %d\n", aux->region, s->distance);

                    if (distance)
                    {
                        // printf("-> dq_insert_tail da reg %d\n", aux->region);
                        dq_insert_tail(deque, aux->region, aux->color, distance);
                    }
                    else
                    {
                        // printf("-> dq_insert_head da reg %d\n", aux->region);
                        dq_insert_head(deque, aux->region, aux->color, distance);
                    }
                    g->array[aux->region].head->visited = 2;
                }
                aux = aux->next;
            }
            free(current);
            free(aux);
            // dq_show_list(deque);
            // break;
        }
        free(deque);
    }

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
        vertice = create_vertice(size_a, color_a, a, a, pos);
        g->array[a].head = vertice;

        found = 1;
    }
    if (g->array[a].head)
    {
        // printf("criando vertice b = %d e colocando no a = %d\n", b, a);
        g->array[a].head->size = size_a;
        vertice = create_vertice(size_b, color_b, b, a, pos);
        Vertice *aux = g->array[a].head;
        while (aux->next)
        {
            aux = aux->next;
        }

        aux->next = vertice;
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
    }

    g->array->visiteds = 0;
    
    return g;
}

void free_vertices_list(AdjList *l)
{
    //libero todos os nos da lista encadeada
	if(!l->head)
	{
		return;
	}

	Vertice *aux = l->head;
	l->head = l->head->next;
	while(l->head)
	{	
		free(aux);
		aux = l->head;
		l->head = l->head->next;
	}
	free(aux);	
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
        printf("PAI %d: ", i);
        while(aux != NULL)
        {
            printf("region %d (pos = %d-%d, cor = %d, quantidade = %d)\n", aux->region, aux->pos[0], aux->pos[1], aux->color, aux->size);
            aux = aux->next;
        }
        printf("------------------------\n");
    }

}