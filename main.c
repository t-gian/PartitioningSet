#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"
#define FILE_IN "g.txt"
#define FILE_IN2 "proposta.txt"

int checkPropostaRic(int *sol, int *val, int M, int V, Graph G);
int wrapperVerifica(Graph G, int V);
int checkPartizioneVertici(int **vettPartizioni, int m, int V, int *cardinalitaM);
int checkAdiacenzaVertici(int **vettPartizioni, int m, int *cardinalitaM, Graph G);
void partitioningWrapper(int n, int *maxN, Graph G);
void partitioning(int n, int m, int pos, int *sol, int *val, int *maxN, int *bestSol, Graph G);

int main()
{
    FILE *fin = fopen(FILE_IN, "r");
    if (fin == NULL)
        return -1;
    int V;
    fscanf(fin, "%d", &V);
    Graph G = graphLoad(fin, V);
    fclose(fin);
    if (wrapperVerifica(G, V))
        printf("Partizionamento valido con dominating sets validi \n");
    else
        printf("Partizionamento NON valido \n");
    int maxN = 0; // dumb var3 *maxN = 0 ma come si fa??
    partitioningWrapper(V, &maxN, G);
    graphFree(G);
    return 0;

    int wrapperVerifica(Graph G, int V)
    {
        FILE *fin2 = fopen(FILE_IN2, "r");
        if (fin2 == NULL)
            return -2;
        int m;
        fscanf(fin2, "%d", &m);
        int **vettPartizioni = malloc(m * sizeof(int *));
        int *cardinalitaM = calloc(m, sizeof(int));
        int n = 0;
        for (int i = 0; i < m; i++)
        {
            fscanf(fin2, "%d", &n);
            vettPartizioni[i] = malloc(n * sizeof(int));
            cardinalitaM[i] = n;
            for (int j = 0; j < n; j++)
                fscanf(fin2, "%d", &vettPartizioni[i][j]);
        }
        fclose(fin2);
        int result = (checkPartizioneVertici(vettPartizioni, m, V, cardinalitaM) && checkAdiacenzaVertici(vettPartizioni, m, cardinalitaM, G));
        free(cardinalitaM);
        for (int i = 0; i < m; i++)
            free(vettPartizioni[i]);
        free(vettPartizioni);
        return result;
    }

    int checkPartizioneVertici(int **vettPartizioni, int m, int V, int *cardinalitaM)
    {
        int *exclV = malloc(V * sizeof(int));
        for (int i = 0; i < V; i++)
            exclV[i] = -1;
        int check = 0; // dumb var0 ...
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < cardinalitaM[i]; j++)
            {
                if (exclV[vettPartizioni[i][j]] == -1)
                {
                    exclV[vettPartizioni[i][j]] = vettPartizioni[i][j];
                    check++;
                }
                else
                    return 0;
            }
        }
        free(exclV);
        if (check == V)
            return 1;
        return 0;
    }

    int checkAdiacenzaVertici(int **vettPartizioni, int m, int *cardinalitaM, Graph G)
    {
        int checkAdj;
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < cardinalitaM[i]; j++)
            {
                for (int l = i + 1; l < m; l++)
                {
                    checkAdj = 0; // var1 formattazione criminale di Exam
                    for (int s = 0; s < cardinalitaM[l]; s++)
                    {
                        if (checkMadj(G, vettPartizioni[l][s], vettPartizioni[i][j]))
                        {
                            checkAdj = 1;
                            break;
                        }
                    }
                    if (!checkAdj)
                        return 0;
                }
                // Aggiunta0 aggiunto check anche sulle partizioni precedenti (look back non solo look ahead)
                if (i > 0)
                {
                    for (int l = i - 1; l >= 0; l--)
                    {
                        checkAdj = 0;
                        for (int s = 0; s < cardinalitaM[l]; s++)
                        {
                            if (checkMadj(G, vettPartizioni[l][s], vettPartizioni[i][j]))
                            {
                                checkAdj = 1;
                                break;
                            }
                        }
                        if (!checkAdj)
                            return 0;
                    }
                }
            }
        }
        return 1;
    }
    int checkPropostaRic(int *sol, int *val, int M, int V, Graph G)
    {
        int i;
        int *cardM = calloc(M, sizeof(int));
        int *tmpcardM = calloc(M, sizeof(int));
        int **vettPart = malloc(M * sizeof(int *));
        for (i = 0; i < V; i++)
            cardM[sol[i]]++;
        for (i = 0; i < M; i++)
            vettPart[i] = malloc(cardM[i] * sizeof(int));
        for (i = 0; i < M; i++)
            tmpcardM[i] = cardM[i]; // var5
        for (i = 0; i < V; i++)
            vettPart[sol[i]][--tmpcardM[sol[i]]] = val[i];
        int result = (checkPartizioneVertici(vettPart, M, V, cardM) && checkAdiacenzaVertici(vettPart, M, cardM, G));
        for (i = 0; i < M; i++)
            free(vettPart[i]);
        free(vettPart);
        free(cardM);
        free(tmpcardM);
        return result;
    }

    void partitioningWrapper(int n, int *maxN, Graph G)
    {
        int *sol = calloc(n, sizeof(int));
        int *val = malloc(n * sizeof(int));
        for (int i = 0; i < n; i++)
            val[i] = i;
        int *bestSol = calloc(n, sizeof(int));
        partitioning(n, 1, 0, sol, val, maxN, bestSol, G);
        printf("Partizione a cardinalita %d massima \n", *maxN);
        for (int i = 0; i < n; i++)
            printf("%d ", bestSol[i]);
        free(sol);
        free(val);
        free(bestSol);
    }

    void partitioning(int n, int m, int pos, int *sol, int *val, int *maxN, int *bestSol, Graph G)
    {
        int i;
        if (pos >= n)
        {
            if (checkPropostaRic(sol, val, m, n, G))
            {
                if (m > *maxN)
                {
                    *maxN = m;
                    for (i = 0; i < n; i++)
                        bestSol[i] = sol[i];
                }
            }
            return;
        }
        for (i = 0; i < m; i++)
        {
            if (m <= *maxN)
                break; // pruning poichè basta solo una qualsiasi sol a card maggiore
            sol[pos] = i;
            partitioning(n, m, pos + 1, sol, val, maxN, bestSol, G);
        }
        sol[pos] = m;
        if (m + 1 <= n) // max partitions goes from 1 to n
            partitioning(n, m + 1, pos + 1, sol, val, maxN, bestSol, G);
    }
