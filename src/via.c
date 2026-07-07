#include "../include/via.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ID 64
#define MAX_CEP 64
#define MAX_NOME 128

void processarLinhaVertice(Grafo* grafo, char* linha) {
    char cmd;
    char id[MAX_ID];
    double x;
    double y;

    if (grafo == NULL || linha == NULL) {
        return;
    }

    if (sscanf(linha, " %c %63s %lf %lf", &cmd, id, &x, &y) == 4) {
        inserirVertice(grafo, id, x, y);
    }
}


void processarLinhaAresta(Grafo* grafo, char* linha) {
    char cmd;
    char origem[MAX_ID];
    char destino[MAX_ID];
    char ldir[MAX_CEP];
    char lesq[MAX_CEP];
    char nome[MAX_NOME];
    double cmp;
    double vm;

    if (grafo == NULL || linha == NULL) {
        return;
    }

    if (sscanf(
            linha,
            " %c %63s %63s %63s %63s %lf %lf %127s",
            &cmd,
            origem,
            destino,
            ldir,
            lesq,
            &cmp,
            &vm,
            nome
        ) == 8) {
        inserirAresta(grafo, origem, destino, ldir, lesq, cmp, vm, nome);
    }
}


Grafo* processVia(const char* viaPath) {
    FILE* arq;
    Grafo* grafo;
    char linha[512];

    arq = fopen(viaPath, "r");

    if (arq == NULL) {
        printf("Erro ao abrir arquivo VIA: %s\n", viaPath);
        return NULL;
    }

    grafo = criarGrafo();

    if (grafo == NULL) {
        fclose(arq);
        return NULL;
    }

    /*
     * Primeira linha do .via contem o numero de vertices.
     * Por enquanto, apenas lemos e ignoramos, pois o grafo cresce dinamicamente.
     */
    if (fgets(linha, sizeof(linha), arq) == NULL) {
        fclose(arq);
        return grafo;
    }

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        if (linha[0] == 'v') {
            processarLinhaVertice(grafo, linha);
        } else if (linha[0] == 'e') {
            processarLinhaAresta(grafo, linha);
        }
    }

    fclose(arq);

    return grafo;
}