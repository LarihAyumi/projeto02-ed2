#include "../include/geo.h"
#include "../include/pm.h"
#include "../include/qry.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include "../include/pessoa.h"
#include "../include/svg.h"
#include "../include/grafo.h"
#include "../include/via.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void montarPath(char* destino, size_t tam, const char* dir, const char* arq) {
    if (dir == NULL || strlen(dir) == 0) {
        snprintf(destino, tam, "%s", arq);
    } else {
        snprintf(destino, tam, "%s/%s", dir, arq);
    }
}

static void nomeBaseSemExtensao(char* destino, size_t tam, const char* caminho) {
    const char* base = strrchr(caminho, '/');
    const char* baseWin = strrchr(caminho, '\\');

    if (baseWin != NULL && (base == NULL || baseWin > base)) {
        base = baseWin;
    }

    base = base ? base + 1 : caminho;

    snprintf(destino, tam, "%s", base);

    char* ponto = strrchr(destino, '.');
    if (ponto != NULL) {
        *ponto = '\0';
    }
}

static int criarDiretorioSaida(const char* dir) {
    char comando[600];

#ifdef _WIN32
    snprintf(comando, sizeof(comando), "if not exist \"%s\" mkdir \"%s\"", dir, dir);
#else
    snprintf(comando, sizeof(comando), "mkdir -p \"%s\"", dir);
#endif

    return system(comando);
}

int main(int argc, char* argv[]) {
    char bed[256] = "";
    char bsd[256] = "";
    char arqGeo[256] = "";
    char arqPm[256] = "";
    char arqQry[256] = "";
    char arqVia[256] = "";

    Grafo* grafo = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            strcpy(bed, argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            strcpy(arqGeo, argv[++i]);
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            strcpy(bsd, argv[++i]);
        } else if (strcmp(argv[i], "-pm") == 0 && i + 1 < argc) {
            strcpy(arqPm, argv[++i]);
        } else if (strcmp(argv[i], "-q") == 0 && i + 1 < argc) {
            strcpy(arqQry, argv[++i]);
        } else if (strcmp(argv[i], "-v") == 0 && i + 1 < argc) {
            strcpy(arqVia, argv[++i]);
        }
    }

    if (strlen(arqGeo) == 0 || strlen(bsd) == 0) {
        printf("Uso: ./ted [-e entrada] -f cidade.geo [-pm pessoas.pm] [-q consulta.qry] [-v vias.via] -o saida\n");
        return 1;
    }

    if (criarDiretorioSaida(bsd) != 0) {
        printf("Erro ao criar diretorio de saida: %s\n", bsd);
        return 1;
    }

    char geoPath[2048] = "";
    char pmPath[2048] = "";
    char qryPath[2048] = "";
    char viaPath[2048] = "";
    char svgPath[2048] = "";
    char txtPath[2048] = "";
    char quadrasBasePath[2048] = "";
    char pessoasBasePath[2048] = "";
    char quadrasHfdPath[2048] = "";
    char pessoasHfdPath[2048] = "";

    montarPath(geoPath, sizeof(geoPath), bed, arqGeo);

    if (strlen(arqPm) > 0) {
        montarPath(pmPath, sizeof(pmPath), bed, arqPm);
    }

    if (strlen(arqQry) > 0) {
        montarPath(qryPath, sizeof(qryPath), bed, arqQry);
    }

    if (strlen(arqVia) > 0) {
        montarPath(viaPath, sizeof(viaPath), bed, arqVia);
    }

    char geoBase[128];
    char qryBase[128];
    char nomeSaida[512];

    nomeBaseSemExtensao(geoBase, sizeof(geoBase), arqGeo);

    if (strlen(arqQry) > 0) {
        nomeBaseSemExtensao(qryBase, sizeof(qryBase), arqQry);
        snprintf(nomeSaida, sizeof(nomeSaida), "%s-%s", geoBase, qryBase);
    } else {
        snprintf(nomeSaida, sizeof(nomeSaida), "%s", geoBase);
    }

    snprintf(svgPath, sizeof(svgPath), "%s/%s.svg", bsd, nomeSaida);

    if (strlen(arqQry) > 0) {
        snprintf(txtPath, sizeof(txtPath), "%s/%s.txt", bsd, nomeSaida);
    }

    snprintf(quadrasBasePath, sizeof(quadrasBasePath), "%s/quadras", bsd);
    snprintf(pessoasBasePath, sizeof(pessoasBasePath), "%s/pessoas", bsd);
    snprintf(quadrasHfdPath, sizeof(quadrasHfdPath), "%s/quadras.hfd", bsd);
    snprintf(pessoasHfdPath, sizeof(pessoasHfdPath), "%s/pessoas.hfd", bsd);

    HashFile* quadras = createFile(quadrasBasePath, getQuadraSize());
    HashFile* pessoas = createFile(pessoasBasePath, getPessoaSize());

    if (!quadras || !pessoas) {
        printf("Erro ao criar hashfiles\n");

        if (quadras) closeFile(quadras);
        if (pessoas) closeFile(pessoas);

        return 1;
    }

    FILE* svg = fopen(svgPath, "w");
    FILE* txt = NULL;

    if (strlen(arqQry) > 0) {
        txt = fopen(txtPath, "w");
    }

    if (!svg || (strlen(arqQry) > 0 && !txt)) {
        printf("Erro ao criar arquivos de saida\n");

        if (svg) fclose(svg);
        if (txt) fclose(txt);

        closeFile(quadras);
        closeFile(pessoas);

        return 1;
    }

    startSVG(svg);

    processGeo(geoPath, quadras, svg);

    if (strlen(arqPm) > 0) {
        processPM(pmPath, pessoas);
    }

    if (strlen(arqVia) > 0) {
        grafo = processVia(viaPath);

        if (grafo != NULL) {
            desenharGrafoSvg(grafo, svg);
        }
    }

    if (strlen(arqQry) > 0) {
        processQry(qryPath, pessoas, quadras, txt, svg, grafo);
    }

    endSVG(svg);

    fclose(svg);

    if (txt) {
        fclose(txt);
    }

    generateHFD(quadras, quadrasHfdPath);
    generateHFD(pessoas, pessoasHfdPath);

    if (grafo != NULL) {
        destruirGrafo(grafo);
    }

    closeFile(quadras);
    closeFile(pessoas);

    return 0;
}