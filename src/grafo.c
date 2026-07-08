#include "../include/grafo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 64
#define MAX_CEP 64
#define MAX_NOME 128
#define CAP_INICIAL 32

typedef struct Aresta {
    int destino;
    char ldir[MAX_CEP];
    char lesq[MAX_CEP];
    double cmp;
    double vm;
    int expandida;
    char nome[MAX_NOME];
    struct Aresta* prox;
} Aresta;

typedef struct Vertice {
    char id[MAX_ID];
    double x;
    double y;
    Aresta* adj;
} Vertice;

struct Grafo {
    Vertice* vertices;
    int qtd;
    int cap;
};


static int buscarIndiceVertice(Grafo* grafo, const char* id) {
    int i;

    if (grafo == NULL || id == NULL) {
        return -1;
    }

    for (i = 0; i < grafo->qtd; i++) {
        if (strcmp(grafo->vertices[i].id, id) == 0) {
            return i;
        }
    }

    return -1;
}

Grafo* criarGrafo(void) {
    Grafo* grafo;

    grafo = malloc(sizeof(Grafo));

    if (grafo == NULL) {
        return NULL;
    }

    grafo->qtd = 0;
    grafo->cap = CAP_INICIAL;
    grafo->vertices = calloc(grafo->cap, sizeof(Vertice));

    if (grafo->vertices == NULL) {
        free(grafo);
        return NULL;
    }

    return grafo;
}

void destruirGrafo(Grafo* grafo) {
    int i;
    Aresta* atual;
    Aresta* prox;

    if (grafo == NULL) {
        return;
    }

    for (i = 0; i < grafo->qtd; i++) {
        atual = grafo->vertices[i].adj;

        while (atual != NULL) {
            prox = atual->prox;
            free(atual);
            atual = prox;
        }
    }

    free(grafo->vertices);
    free(grafo);
}

void inserirVertice(Grafo* grafo, const char* id, double x, double y) {
    Vertice* novoVetor;

    if (grafo == NULL || id == NULL) {
        return;
    }

    if (buscarIndiceVertice(grafo, id) != -1) {
        return;
    }

    if (grafo->qtd >= grafo->cap) {
        novoVetor = realloc(grafo->vertices, grafo->cap * 2 * sizeof(Vertice));

        if (novoVetor == NULL) {
            return;
        }

        grafo->vertices = novoVetor;
        memset(&grafo->vertices[grafo->cap], 0, grafo->cap * sizeof(Vertice));
        grafo->cap *= 2;
    }

    strncpy(grafo->vertices[grafo->qtd].id, id, MAX_ID - 1);
    grafo->vertices[grafo->qtd].id[MAX_ID - 1] = '\0';

    grafo->vertices[grafo->qtd].x = x;
    grafo->vertices[grafo->qtd].y = y;
    grafo->vertices[grafo->qtd].adj = NULL;

    grafo->qtd++;
}

void inserirAresta(
    Grafo* grafo,
    const char* origem,
    const char* destino,
    const char* ldir,
    const char* lesq,
    double cmp,
    double vm,
    const char* nome
) {
    int idxOrigem;
    int idxDestino;
    Aresta* nova;

    if (grafo == NULL || origem == NULL || destino == NULL) {
        return;
    }

    idxOrigem = buscarIndiceVertice(grafo, origem);
    idxDestino = buscarIndiceVertice(grafo, destino);

    if (idxOrigem == -1 || idxDestino == -1) {
        return;
    }

    nova = calloc(1, sizeof(Aresta));

    if (nova == NULL) {
        return;
    }

    nova->destino = idxDestino;
    nova->cmp = cmp;
    nova->vm = vm;

    if (ldir != NULL) {
        strncpy(nova->ldir, ldir, MAX_CEP - 1);
        nova->ldir[MAX_CEP - 1] = '\0';
    }

    if (lesq != NULL) {
        strncpy(nova->lesq, lesq, MAX_CEP - 1);
        nova->lesq[MAX_CEP - 1] = '\0';
    }

    if (nome != NULL) {
        strncpy(nova->nome, nome, MAX_NOME - 1);
        nova->nome[MAX_NOME - 1] = '\0';
    }

    nova->prox = grafo->vertices[idxOrigem].adj;
    grafo->vertices[idxOrigem].adj = nova;
}

void desenharGrafoSvg(Grafo* grafo, FILE* svg) {
    int i;
    int destino;
    Aresta* atual;
    Vertice* origemV;
    Vertice* destinoV;

    if (grafo == NULL || svg == NULL) {
        return;
    }

    fprintf(svg, "\n<g id=\"sistema-viario\">\n");

    for (i = 0; i < grafo->qtd; i++) {
        origemV = &grafo->vertices[i];
        atual = origemV->adj;

        while (atual != NULL) {
            destino = atual->destino;

            if (destino >= 0 && destino < grafo->qtd) {
                destinoV = &grafo->vertices[destino];

                fprintf(svg, "<line x1=\"%.2lf\" y1=\"%.2lf\" x2=\"%.2lf\" y2=\"%.2lf\" " "stroke=\"gray\" stroke-width=\"1\" />\n", origemV->x, origemV->y, destinoV->x, destinoV->y);
            }
            atual = atual->prox;
        }
    }
    fprintf(svg, "</g>\n");
}

int contarVerticesGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return 0;
    }

    return grafo->qtd;
}

int contarArestasGrafo(Grafo* grafo) {
    int i;
    int total = 0;
    Aresta* atual;

    if (grafo == NULL) {
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        atual = grafo->vertices[i].adj;

        while (atual != NULL) {
            total++;
            atual = atual->prox;
        }
    }

    return total;
}

static int pontoDentroRegiao(double px, double py, double x, double y, double w, double h) {
    double x1 = x;
    double y1 = y;
    double x2 = x + w;
    double y2 = y + h;
    double aux;

    if (x2 < x1) {
        aux = x1;
        x1 = x2;
        x2 = aux;
    }

    if (y2 < y1) {
        aux = y1;
        y1 = y2;
        y2 = aux;
    }

    return px >= x1 && px <= x2 && py >= y1 && py <= y2;
}

int atualizarVelocidadeRegiao(Grafo* grafo, double vmNova, double x, double y, double w, double h) {
    int i;
    int qtd = 0;
    Aresta* atual;
    Vertice* origemV;
    Vertice* destinoV;

    if (grafo == NULL) {
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        origemV = &grafo->vertices[i];
        atual = origemV->adj;

        while (atual != NULL) {
            destinoV = &grafo->vertices[atual->destino];

            if (
                pontoDentroRegiao(origemV->x, origemV->y, x, y, w, h) &&
                pontoDentroRegiao(destinoV->x, destinoV->y, x, y, w, h)
            ) {
                atual->vm = vmNova;
                qtd++;
            }
            atual = atual->prox;
        }
    }

    return qtd;
}

double obterVelocidadeAresta(Grafo* grafo, const char* origem, const char* destino) {
    int idxOrigem;
    int idxDestino;
    Aresta* atual;

    if (grafo == NULL || origem == NULL || destino == NULL) {
        return -1.0;
    }
    idxOrigem = buscarIndiceVertice(grafo, origem);
    idxDestino = buscarIndiceVertice(grafo, destino);

    if (idxOrigem == -1 || idxDestino == -1) {
        return -1.0;
    }
    atual = grafo->vertices[idxOrigem].adj;

    while (atual != NULL) {
        if (atual->destino == idxDestino) {
            return atual->vm;
        }
        atual = atual->prox;
    }

    return -1.0;
}

static void visitarComponente(Grafo* grafo, int inicio, double vl, int* visitado, double* minX, double* minY, double* maxX, double* maxY) {
    int* pilha;
    int topo = 0;
    int i;
    int atualIdx;
    Aresta* aresta;
    Vertice* v;

    pilha = malloc(sizeof(int) * grafo->qtd);
    if (!pilha) {
        return;
    }

    pilha[topo++] = inicio;
    visitado[inicio] = 1;
    *minX= grafo->vertices[inicio].x;
    *maxX= grafo->vertices[inicio].x;
    *minY= grafo->vertices[inicio].y;
    *maxY= grafo->vertices[inicio].y;

    while (topo > 0) {
        atualIdx = pilha[--topo];
        v = &grafo->vertices[atualIdx];

        if (v->x<*minX) *minX= v->x;
        if (v->x>*maxX) *maxX= v->x;
        if (v->y<*minY) *minY= v->y;
        if (v->y>*maxY) *maxY= v->y;

        aresta = v->adj;

        while (aresta != NULL) {
            if (aresta->vm >= vl && !visitado[aresta->destino]) {
                visitado[aresta->destino] = 1;
                pilha[topo++] = aresta->destino;
            }
            aresta = aresta->prox;
        }

        for (i = 0; i < grafo->qtd; i++) {
            aresta = grafo->vertices[i].adj;

            while (aresta != NULL) {
                if (aresta->destino == atualIdx && aresta->vm >= vl && !visitado[i]) {
                    visitado[i] = 1;
                    pilha[topo++] = i;
                }
                aresta = aresta->prox;
            }
        }
    }
    free(pilha);
}

int calcularComponentesConexos(Grafo* grafo,double vl) {
    int* visitado;
    int i;
    int qtd = 0;
    double minX, minY, maxX, maxY;

    if (grafo == NULL) {
        return 0;
    }

    visitado = calloc(grafo->qtd, sizeof(int));
    if (!visitado) {
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        if (!visitado[i]) {
            visitarComponente(grafo, i, vl, visitado, &minX, &minY, &maxX, &maxY);
            qtd++;
        }
    }

    free(visitado);
    return qtd;
}

int calcularComponentesConexosBBox(Grafo* grafo, double vl, double* minX,double* minY, double* maxX,double* maxY, int maxComp) {
    int* visitado;
    int i;
    int qtd = 0;
    double cMinX, cMinY, cMaxX, cMaxY;

    if (grafo == NULL||minX == NULL|| minY == NULL || maxX == NULL ||maxY == NULL) {
        return 0;
    }

    visitado = calloc(grafo->qtd, sizeof(int));
    if (!visitado) {
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        if (!visitado[i]) {
            visitarComponente(grafo, i, vl, visitado, &cMinX, &cMinY, &cMaxX, &cMaxY);
            if (qtd < maxComp) {
                minX[qtd]= cMinX;
                minY[qtd]= cMinY;
                maxX[qtd]= cMaxX;
                maxY[qtd]= cMaxY;
            }
            qtd++;
        }
    }
    free(visitado);
    return qtd;
}

//funçoes para a expansão
typedef struct {
    int origem;
    int destino;
    Aresta* aresta;
} ArestaAgm;

static void limparExpansoes(Grafo* grafo) {
    int i;
    Aresta* atual;

    if (grafo == NULL) {
        return;
    }

    for (i = 0; i < grafo->qtd; i++) {
        atual = grafo->vertices[i].adj;

        while (atual != NULL) {
            atual->expandida = 0;
            atual = atual->prox;
        }
    }
}

static int contarArestasExpansao(Grafo* grafo) {
    int i;
    int qtd = 0;
    Aresta* atual;

    if (grafo == NULL) {
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        atual = grafo->vertices[i].adj;
        while (atual != NULL) {
            qtd++;
            atual = atual->prox;
        }
    }
    return qtd;
}

static void trocarArestasAgm(ArestaAgm* a, ArestaAgm* b) {
    ArestaAgm aux;
    aux = *a;
    *a = *b;
    *b = aux;
}

static void ordenarArestasPorComprimento(ArestaAgm* arestas, int qtd) {
    int i;
    int j;
    for (i = 0; i < qtd - 1; i++) {
        for (j = i + 1; j < qtd; j++) {
            if (arestas[j].aresta->cmp < arestas[i].aresta->cmp) {
                trocarArestasAgm(&arestas[i], &arestas[j]);
            }
        }
    }
}

static int buscarPai(int* pai, int x) {
    while (pai[x] != x) {
        pai[x] = pai[pai[x]];
        x = pai[x];
    }
    return x;
}

static int unirConjuntos(int* pai, int* rank, int a, int b) {
    int pa = buscarPai(pai, a);
    int pb = buscarPai(pai, b);

    if (pa == pb) {
        return 0;
    }

    if (rank[pa] < rank[pb]) {
        pai[pa] = pb;
    } else if (rank[pa] > rank[pb]) {
        pai[pb] = pa;
    } else {
        pai[pb] = pa;
        rank[pa]++;
    }

    return 1;
}

int aplicarExpansaoViaria(Grafo* grafo, double vl) {
    int qtdArestas;
    int i;
    int k = 0;
    int expandidas = 0;
    int* pai;
    int* rank;
    ArestaAgm* arestas;
    Aresta* atual;

    if (grafo == NULL || grafo->qtd <= 0) {
        return 0;
    }

    limparExpansoes(grafo);

    qtdArestas = contarArestasExpansao(grafo);

    if (qtdArestas == 0) {
        return 0;
    }

    arestas = malloc(sizeof(ArestaAgm) * qtdArestas);
    pai = malloc(sizeof(int) * grafo->qtd);
    rank = calloc(grafo->qtd, sizeof(int));

    if (arestas == NULL || pai == NULL || rank == NULL) {
        free(arestas);
        free(pai);
        free(rank);
        return 0;
    }

    for (i = 0; i < grafo->qtd; i++) {
        pai[i] = i;
    }

    for (i = 0; i < grafo->qtd; i++) {
        atual = grafo->vertices[i].adj;

        while (atual != NULL) {
            arestas[k].origem = i;
            arestas[k].destino = atual->destino;
            arestas[k].aresta = atual;
            k++;

            atual = atual->prox;
        }
    }

    ordenarArestasPorComprimento(arestas, qtdArestas);

    for (i= 0; i< qtdArestas; i++) {
        if (unirConjuntos(pai, rank, arestas[i].origem, arestas[i].destino)) {
            if (arestas[i].aresta->vm < vl) {
                arestas[i].aresta->vm *= 1.5;
                arestas[i].aresta->expandida = 1;
                expandidas++;
            }
        }
    }

    free(arestas);
    free(pai);
    free(rank);

    return expandidas;
}

int arestaFoiExpandida(Grafo* grafo, const char* origem, const char* destino) {
    int idxOrigem;
    int idxDestino;
    Aresta* atual;

    if (grafo== NULL||origem== NULL||destino== NULL) {
        return 0;
    }

    idxOrigem= buscarIndiceVertice(grafo, origem);
    idxDestino= buscarIndiceVertice(grafo, destino);

    if (idxOrigem == -1||idxDestino == -1) {
        return 0;
    }

    atual = grafo->vertices[idxOrigem].adj;

    while (atual != NULL) {
        if (atual->destino == idxDestino) {
            return atual->expandida;
        }

        atual = atual->prox;
    }

    return 0;
}

void desenharExpansaoViariaSvg(Grafo* grafo, FILE* svg) {
    int i;
    Aresta* atual;
    Vertice* origem;
    Vertice* destino;

    if (grafo == NULL||svg == NULL) {
        return;
    }

    for (i = 0; i < grafo->qtd; i++) {
        origem = &grafo->vertices[i];
        atual = origem->adj;

        while (atual != NULL) {
            if (atual->expandida) {
                destino = &grafo->vertices[atual->destino];

                fprintf(svg, "<line x1=\"%.2lf\" y1=\"%.2lf\" x2=\"%.2lf\" y2=\"%.2lf\" ""stroke=\"red\" stroke-width=\"4\" />\n", origem->x,origem->y, destino->x, destino->y);
            }
            atual = atual->prox;
        }
    }
}