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

    fprintf(svg, "\n<!----- Sistema viário ----->\n");

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