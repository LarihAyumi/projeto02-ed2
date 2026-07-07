#include "../include/geo.h"
#include "../include/svg.h"
#include "../include/quadra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void processGeo(const char* geoPath, HashFile* quadrasHash, FILE* svg) {
    FILE* geo = fopen(geoPath, "r");
    FILE* quadrasFile = fopen("quadras.dat", "ab+");

    if (!geo || !svg || !quadrasFile) {
        printf("Erro ao abrir arquivos\n");
        if (geo) fclose(geo);
        if (quadrasFile) fclose(quadrasFile);
        return;
    }

    char comando[10];

    char sw[20] = "1.0";
    char cfill[20] = "white";
    char cstrk[20] = "black";

    while (fscanf(geo, "%s", comando) != EOF) {
        if (strcmp(comando, "cq") == 0) {
            fscanf(geo, "%s %s %s", sw, cfill, cstrk);
        }

        else if (strcmp(comando, "q") == 0) {
            char cep[20];
            double x, y, w, h;

            fscanf(geo, "%s %lf %lf %lf %lf", cep, &x, &y, &w, &h);

            drawQuadra(svg, cep, x, y, w, h, cfill, cstrk, sw);

            if (quadrasHash != NULL) {
                Quadra* q = createQuadra(cep, x, y, w, h);
                insertRegister(quadrasHash, cep, q);
                free(q);
            }
        }
    }
    
    fclose(quadrasFile);
    fclose(geo);
}