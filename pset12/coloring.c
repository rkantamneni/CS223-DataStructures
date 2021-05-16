#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <string.h>

struct graph {
    size_t n;              /* number of vertices */
    size_t m;              /* number of edges */
    size_t graphDegree;              /* max degree of the graph */
    struct successors {
        size_t d;          /* number of successors */
        size_t len;        /* number of slots in array */
        size_t finalColor;
        size_t isColored;
        size_t list[];    /* actual list of successors starts here */
    } *alist[];
};
typedef struct graph *Graph;


Graph graphCreate(size_t n){
    //check if n is, return NULL
    Graph g;
    size_t i;

    g = malloc(sizeof(struct graph) + sizeof(struct successors *) * n);

    g->n = n;
    g->m = 0;
    g->graphDegree = 0;

    for(i = 0; i < n; i++) {
        g->alist[i] = malloc(sizeof(struct successors));

        g->alist[i]->d = 0;
        g->alist[i]->len = 0;
        g->alist[i]->isColored = 0;
    } 
    return g;
}

void graphDestroy(Graph g){
    if (g != NULL){
        size_t i;
        for(i = 0; i < g->n; i++) free(g->alist[i]);
        free(g);
    }
    
}

void graphAddEdge(Graph g, size_t u, size_t v){
    assert(u >= 0); assert(u < g->n); assert(v >= 0); assert(v < g->n);

    /* do we need to grow the list? */
    while(g->alist[u]->d >= g->alist[u]->len) {
        g->alist[u]->len = g->alist[u]->len * 2 + 1;   /* +1 because it might have been 0 */
        g->alist[u] =
            realloc(g->alist[u], 
                sizeof(struct successors) + sizeof(size_t) * g->alist[u]->len);
    }

    /* now add the new sink */
    g->alist[u]->list[g->alist[u]->d++] = v;

    //Biggest Graph Degree, Needed?
    if (g->alist[u]->d > g->graphDegree){
        g->graphDegree = g->alist[u]->d;
    }
}
void printOutput(Graph g){
    for (size_t i = 0; i < g->n; i++){
        printf("%zu %zu\n", i, g->alist[i]->finalColor);
        //printf("%zu\n", i);
    }  
}

void graphAssignColor(Graph g){
    size_t k; //max color number

    k = g->graphDegree * (g->graphDegree - 1);

    for (size_t i = 0; i < g->n; i++){ //Goes down adjacency lists (Graph Representation)
        int *myArray = (int*)calloc(k, sizeof(int));

        for (size_t j = 0; j < g->alist[i]->d; j++){ //Iterating through nodes adjacent

            for (size_t k = 0; k < g->alist[  g->alist[i]->list[j]  ]->d; k++){ //Iterating through neighbor 2
                if (g->alist[  g->alist[i]->list[j]  ] -> list[k] != i){
                    //printf("%zu, Neighbor: %d, Neighbor-2: %d\n", i,  g->alist[i]->list[j], g->alist[  g->alist[i]->list[j]  ] -> list[k] );
                    if (g->alist[ g->alist[  g->alist[i]->list[j]  ] -> list[k] ]-> isColored == 1){ //vu and uv
                        myArray[ g->alist[ g->alist[  g->alist[i]->list[j]  ] -> list[k] ]-> finalColor ] = 1;
                    }
                }
            }     
        }
        size_t h;
        h = 0;
        while (myArray[h] == 1){
            h++;
        } 
        g->alist[i]->finalColor = h;
        g->alist[i]->isColored = 1;
        free(myArray);
    }
}


int main(int argc, char **argv) {
    size_t n; //number of vertices/nodes  0 -> n-1
    scanf("%lu\n", &n);
    Graph g;
    g = graphCreate(n);

    size_t vertex1;
    size_t vertex2;
    while(scanf("%lu %lu\n", &vertex1, &vertex2) == 2) {
        graphAddEdge(g, vertex1, vertex2);
        graphAddEdge(g, vertex2, vertex1);
        g->m+=2;
    }
    graphAssignColor(g);
    printOutput(g);
    graphDestroy(g);
    return 0;
}
//Edge Cases
//every vertex has a vertex to every other vertex
    //latice graph, grid graph