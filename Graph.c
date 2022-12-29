
#include "Graph.h"
#include <stdlib.h>
struct graph { int V; int E; int **madj};

static void insertE (Graph G, Edge e) {
    int v = e.v, w = e.w, wt = e.wt;
    if (G->madj[v][w] == 0){
        G->E++;
        G->madj[v][w] = wt;
        G->madj[w][v] = wt;
    }
}
static Edge EDGEcreate (int v, int w, int wt){
    Edge e;
    e.v=v; e.w=w; e.wt=wt;
    return e;
}
void GRAPhinsertE(Graph G, int id1, int id2, int wt){
    insertE(G,EDGEcreate(id1,id2,wt));
}

Graph graphLoad(FILE *fin,int V) {
    int id1, id2;
    Graph G = malloc(sizeof *G);
    G->V=V;
    G->E = 0;
    G->madj = madjAlloc(G->V);
    while (fscanf(fin, "%d %d",&id1,&id2) == 2) {
        if (id1 >= 0 && id2 >= 0)
            GRAPhinsertE(G, id1, id2, 1);
    }
    return G;
}

int **madjAlloc (int V){
    int i,j;
    int **t = malloc (V*sizeof(int *));
    for (i=0;i<V;i++) t[i]=malloc(V*sizeof(int));
    for (i=0;i<V;i++)
        for (j=0;j<V;j++)
            t[i][j]=0;
    return t;
}
int checkMadj (Graph G,int r, int c){
    return (G->madj[r][c]==1);
}
void graphFree(Graph G){
    for (int i=0;i<G->V;i++)
        free(G->madj[i]);
    free(G->madj);
    free(G);
}
