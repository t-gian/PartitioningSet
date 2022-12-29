
#ifndef AP_06_22_GRAPH_H
#define AP_06_22_GRAPH_H
#include <stdio.h>
typedef struct graph *Graph;
typedef struct edge { int v; int w; int wt;} Edge;
Graph graphLoad(FILE *fin, int V);
int **madjAlloc (int V);
int checkMadj (Graph G,int r, int c);
void graphFree(Graph G);

#endif
