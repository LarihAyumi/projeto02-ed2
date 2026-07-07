#include "../include/via.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processarLinhaVertice(Grafo* grafo, char* linha) {
    (void)grafo;
    (void)linha;
}

void processarLinhaAresta(Grafo* grafo, char* linha) {
    (void)grafo;
    (void)linha;
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


    fgets(linha, sizeof(linha), arq);

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