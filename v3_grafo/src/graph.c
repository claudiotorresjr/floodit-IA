/**
 * @file graph.c
 * @author GRR20176143 Cláudio Torres Júnior
 * @author GRR20173546 Lucas José Ribeiro
 * @brief File with graph.h implementation. All functions related with graph manipulation is here.
 * @version 1.0
 * @date 2022-04-03
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/graph.h"
#include "../include/map.h"
#include "../include/doubleQueue.h"

/**
 * @brief Given a color, paint the root node and all of its neighbors recursively
 * 
 * @param g The graph
 * @param v The vertice beeing painted
 * @param base_color The root color before beeing painted
 * @param color The color to paint the node
 * @param change_color If the color is the final one, update the color forever
 */
void paint_graph(Graph **g, Vertice *v, int base_color, int color, int change_color)
{
    // printf("painting the region: %d with color %d\n", v->region, color);
    (*g)->array[v->region].head->visited = 1;
    (*g)->array[v->region].head->color = color;

    if (change_color)
    {
        (*g)->array[v->region].head->first_color = color;
    }

    Vertice *aux = v->next;
    while (aux)
    {
        // printf("---- checking reion: %d (%d) - %d == base_color %d\n", aux->region, aux->color, (*g)->array[aux->region].head->visited, base_color);
        if ((*g)->array[aux->region].head->color == base_color && !(*g)->array[aux->region].head->visited)
        {
            // printf("->> has color %d\n", base_color);
            paint_graph(g, (*g)->array[aux->region].head, base_color, color, change_color);
        }

        aux = aux->next;
    }
}

/**
 * @brief Remove the given node by its region from adjacency list
 * 
 * @param head The adjacency list first element
 * @param region The region to remove from list
 */
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

/**
 * @brief Given a node region and its color, merge all nodes (region) with same color recursively.
 * 
 * @param g The graph
 * @param region The node to start the nodes (region) merge 
 * @param color The color of the nodes to be merged
 */
void merge_nodes(Graph **g, int region, int color)
{
    // printf("-> starting with region %d\n", region);
    int new_size = (*g)->array[region].head->size;

    //set the region as visited
    (*g)->array[region].head->visited = 1;
    Vertice *aux = (*g)->array[region].head->next;
    while (aux)
    {
        // printf(" --> verify the region with color %d\n", aux->region);
        if(aux->color == color && !(*g)->array[aux->region].head->visited)
        {
            (*g)->array[aux->region].head->visited = 1;
            // printf("    --> is the same\n");
            merge_nodes(g, aux->region, color);
            // printf("  (out of recursion) im in region %d\n", aux->region);
            Vertice *child = (*g)->array[aux->region].head->next;

            //update the size of the node beeing merged
            new_size += (*g)->array[aux->region].head->size;
            // printf("        --> seeing childs of region %d\n", aux->region);
            while (child)
            {
                if(child->region != region && (*g)->array[child->region].head->color != color && !(*g)->array[child->region].head->visited)
                {
                    add_edge(
                        g,
                        region,
                        color,
                        new_size,
                        (*g)->array[child->region].head->region,
                        (*g)->array[child->region].head->color,
                        (*g)->array[child->region].head->size,
                        (*g)->array[child->region].head->pos
                    );

                    // printf("Removin from region %d the node %d\n", (*g)->array[child->region].head->region, aux->region);
                    remove_node_from_list((*g)->array[child->region].head, aux->region);
                }

                //Update aux->region adjacency list removing the child from it
                (*g)->array[aux->region].head->next = child->next;

                if ((*g)->array[aux->region].head->next)
                {
                    (*g)->array[aux->region].head->next->prev = (*g)->array[aux->region].head->next;
                }
                else
                {
                    (*g)->array[aux->region].tail = (*g)->array[aux->region].head;
                }

                free(child);

                child = (*g)->array[aux->region].head->next;
            }

            //Update the merged adjacency list removing the aux from it
            aux->prev->next = aux->next;

            if (aux->prev->next)
            {
                 aux->next->prev = aux->prev;
            }
            else
            {
                (*g)->array[region].tail = aux->prev;
            }

            //remove the aux->region adjacency list -> the list the was merged. We dont need it anymore
            free_vertices_list(&(*g)->array[aux->region]);

            (*g)->array[aux->region].head = NULL;
            (*g)->array[aux->region].tail = NULL;

            free(aux);

            //receive the new second element from adjacency list
            aux = (*g)->array[region].head->next;
        }
        else
        {
            aux = aux->next;
        }
    }
}

/**
 * @brief Set all nodes as not visited
 * 
 * @param g The graph
 */
void reset_graph(Graph **g)
{
    for (int i = 0; i < (*g)->num_v; ++i)
    {
        if ((*g)->array[i].head)
        {
            (*g)->array[i].head->visited = 0;
        }
    }
}

/**
 * @brief Calculate the distance between root and all nodes
 * 
 * @param g The graph
 * @return int - The sum of all distances
 */
int distance_between_nodes(Graph *g)
{
    int total_distance = 0;
    reset_graph(&g);

    DoubleQueue *deque = dq_create();

    dq_insert_head(deque, g->array[0].head->region, g->array[0].head->color, 0);

    while(!dq_empty(deque))
    {
        State *current = dq_remove_head(deque);
        g->array[current->region].head->visited = 1;

        int current_region = g->array[current->region].head->region;

        Vertice *aux = g->array[current->region].head->next;

        // printf("The region %d list\n", current->region);
        int distance;
        int atual_color = g->array[current->region].head->color;

        while(aux)
        {
            //get distance only if not visited
            if (g->array[aux->region].head->visited == 0)
            {
                if(aux->parent == current_region && g->array[aux->region].head->color == atual_color)
                {
                    distance = current->distance;
                }
                else
                {
                    distance = 1 + current->distance;
                }

                //if distance is not zero, insert at tail, because we want the regions with less distance
                //at the beggining of the double-ended queue
                if (distance)
                {
                    dq_insert_tail(deque, aux->region, aux->color, distance);
                }
                else
                {
                    dq_insert_head(deque, aux->region, aux->color, distance);
                }

                total_distance += distance;
                // printf(" --> distance from 0 to %d == %d\n", aux->region, distance);

                //set the node as present in the double-ended queue
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

/**
 * @brief Create a vertice object
 * 
 * @param size The amount of pos with same color in this vertice (region)
 * @param color The color of this node (region)
 * @param region The node region value
 * @param parent The node parent region
 * @param pos The region first position in matrix
 * @return Vertice* - new vertice object
 */
Vertice *create_vertice(int size, int color, int region, int parent, int pos[2])
{
    Vertice *v = (Vertice *)malloc(sizeof(Vertice));

    v->size = size;
    v->color = color;
    //first color is to save the color between colorr changes in loop
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

/**
 * @brief Check if graph already has the vertice from a to b (is the same from b to a)
 * 
 * @param g The graph
 * @param a The vertice head of the adjacency list
 * @param b The vertice present in 'a' adjacency list
 * @return int - 1 if present or 0 if not
 */
int has_edge(Graph *g, int a, int b)
{
    Vertice *aux;

    if (g->array[a].head)
    {
        aux = g->array[a].head->next;
        while (aux != NULL)
        {
            if (aux->region == b)
            {
                return 1;
            }
            aux = aux->next;
        }
    }

    return 0;
}

/**
 * @brief Add edge from vertice a to b and from b to a
 * 
 * @param g The graph
 * @param a The a region
 * @param color_a The a region region color
 * @param size_a The a region size
 * @param b The b region
 * @param color_b The b region color
 * @param size_b The b region size
 * @param pos The b region position
 */
void add_edge(Graph **g, int a, int color_a, int size_a, int b, int color_b, int size_b, int pos[2])
{
    Vertice *vertice;
    int found = 0;

    if (has_edge(*g, a, b))
    {
        return;
    }

    //creating vertice a adjacency list
    if (!(*g)->array[a].head)
    {
        (*g)->num_v++;
        vertice = create_vertice(size_a, color_a, a, a, pos);
        (*g)->array[a].head = vertice;
        (*g)->array[a].tail = vertice;

        found = 1;
    }
    //adding vertice b in a list
    if ((*g)->array[a].head)
    {
        (*g)->array[a].head->size = size_a;
        vertice = create_vertice(size_b, color_b, b, a, pos);

        Vertice *aux = (*g)->array[a].tail;
        vertice->prev = aux;
        aux->next = vertice;

        (*g)->array[a].tail = vertice;
        found = 1;
    }

    //creating vertice b adjacency list
    if (!(*g)->array[b].head)
    {
        (*g)->num_v++;
        vertice = create_vertice(size_b, color_b, b, b, pos);
        (*g)->array[b].head = vertice;
        (*g)->array[b].tail = vertice;

        found = 1;
    }
    //adding vertice b to a list
    if ((*g)->array[b].head)
    {
        (*g)->array[b].head->size = size_b;
        vertice = create_vertice(size_a, color_a, a, b, pos);

        Vertice *aux = (*g)->array[b].tail;
        vertice->prev = aux;
        aux->next = vertice;

        (*g)->array[b].tail = vertice;
        found = 1;
    }

    //update edges count
    if (found)
    {
        (*g)->num_e++;
    }
}

/**
 * @brief Create a graph object
 * 
 * @param size The adjacency list size
 * @return Graph* 
 */
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

    return g;
}

/**
 * @brief Free the adjacency list
 * 
 * @param l The adjacency list to free
 */
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

/**
 * @brief Free the graph
 * 
 * @param g The graph
 */
void free_graph(Graph *g)
{
    for (int i = 0; i < g->num_v; ++i)
    {
        free_vertices_list(&g->array[i]);
    }
    free(g->array);
    free(g);
}

/**
 * @brief Print all graph adjacency list
 * 
 * @param g 
 */
void show_graph(Graph *g)
{
    printf("---- Adjacency list ----\n");
    for (int i = 0; i < g->num_v; ++i)
    {

        Vertice *aux = g->array[i].head;
        if (!aux)
        {
            continue;
        }
        printf("HEAD %d: ", i);
        while(aux != NULL)
        {
            printf("region %d (pos = %d-%d, color = %d, count = %d)\n", aux->region, aux->pos[0], aux->pos[1], aux->color, aux->size);
            aux = aux->next;
        }
        printf("------------------------\n");
    }

}