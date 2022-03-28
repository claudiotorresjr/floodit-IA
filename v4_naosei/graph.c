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

int *verify_max_color_count(Graph *g, State *current, int t_colors)
{
    // 

    int *colors = (int *)malloc((t_colors+1)*sizeof(int));
    initialize_array(&colors, t_colors+1, 0);

    int *eq_regions = (int *)malloc((t_colors+1)*sizeof(int));
    initialize_array(&eq_regions, t_colors+1, 0);

    int *regions = (int *)malloc((g->num_v)*sizeof(int));
    initialize_array(&regions, g->num_v, 0);

    for (int i = 0; i < current->size; ++i)
    {
        int v = current->regions[i];
        Vertice *aux = g->array[v].head->next;

        // printf("verif o ");
        while(aux != NULL)
        {
            // printf("%d ", aux->region);
            if (!g->array[aux->region].head->visited && regions[aux->region] == 0)
            {
                // printf("%d ", aux->region);
                // regions[aux->region] = 1;
                colors[aux->color] += aux->size;
            }
            
            // if (regions[aux->region] > 0)
            {
                eq_regions[aux->color]++;
            }
            aux = aux->next;
        }
        // printf("\n");
    }

    int max = -1;
    int c = -1;
    for (int i = 1; i < t_colors+1; ++i)
    {
        // printf("%d: %d\n", i, colors[i]);
        if (colors[i] > max)
        {
            max = colors[i];
            c = i;
        }
    }

    // printf("--------------------------\n");
    // for (int i = 1; i < t_colors+1; ++i)
    // {
    //     printf("%d ", colors[i]);
    // }
    // printf("\n");
    // for (int i = 1; i < t_colors+1; ++i)
    // {
    //     printf("%d ", eq_regions[i]);
    // }
    // printf("\n");

    //check_draw
    int bigger_region = 0;
    for (int i = 1; i < t_colors+1; ++i)
    {
        // printf("vendo se a cor %d == %d\n", colors[i], max);
        if (colors[i] == max)
        {
            
            // printf("   cor %d (%d) > %d\n", eq_regions[i], i, bigger_region);
            if (eq_regions[i] > bigger_region)
            {
                bigger_region = eq_regions[i];
                c = i;
            }
        }
    } 
    // printf("maior == %d --------------------------\n", bigger_region);

    int *color = (int *)malloc(2*sizeof(int));
    color[0] = c;
    color[1] = max;

    return color;
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
    // while (g->array[i].head != NULL)
    // {
    //     //head da lista eh igual ao vertice a
    //     if (g->array[i].head->region == a)
    //     {
    //         aux = g->array[i].head->next;
    //         while (aux != NULL)
    //         {
    //             //se vertice b ta na lista de a, retorna true (tem aresta)
    //             if (aux->region == b)
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
    //inserindo vertice b na lista do vertice a
    // while (!found)
    // {
    //     //se a lista ta vazia, insere o a primeiro
    //     if (!g->array[i].head)
    //     {
    //         g->num_v++;
    //         vertice = create_vertice(size_a, color_a, a, pos);
    //         g->array[i].head = vertice;

    //         found = 1;
    //     }

    //     //se o a tem lista, adiciona o b nela
    //     if (g->array[i].head->region == a)
    //     {
    //         g->array[i].head->size = size_a;
    //         vertice = create_vertice(size_b, color_b, b, pos);
    //         Vertice *aux = g->array[i].head;
    //         while (aux->next)
    //         {
    //             aux = aux->next;
    //         }

    //         aux->next = vertice;
    //         found = 1;
    //     }
    //     ++i;
    // }

    // if (found == 1)
    // {
    //     g->num_e++;
    // }
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