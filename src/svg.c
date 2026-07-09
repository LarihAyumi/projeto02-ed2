#include "../include/svg.h"
#include <stdio.h>


void startSVG(FILE* svg) {
    if (svg == NULL) return;
    
    fprintf(svg,"<svg xmlns=\"http://www.w3.org/2000/svg\" ""width=\"2200\" height=\"1600\" ""viewBox=\"-150 -150 2200 1600\" ""style=\"background:white; overflow:visible\">\n");
}


void endSVG(FILE* svg) {
    fprintf(svg, "</svg>\n");
}


void drawQuadra(FILE* svg, const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, const char* sw) {

    //Quadra
    fprintf(svg, "<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" " "fill=\"%s\" stroke=\"%s\" stroke-width=\"%s\" />\n", x, y, w, h, cfill, cstrk, sw);

    //Ponto de âncora
    //fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"10\" fill=\"white\" stroke=\"%s\" stroke-width=\"%s\" />\n", x, y, cstrk, sw);

    //CEP
    fprintf(svg, "<text x=\"%lf\" y=\"%lf\" font-size=\"10\" fill=\"black\" stroke=\"black\">%s</text>\n", x + w/4, y + h/2, cep);
}


// dos comandos
void drawXVermelho(FILE* svg, double x, double y) {
    double t = 10;

    fprintf(svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x-t, y-t, x+t, y+t);

    fprintf(svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x-t, y+t, x+t, y-t);
}


void drawTextoSVG(FILE* svg, double x, double y, const char* texto) {
    fprintf(svg, "<text x=\"%lf\" y=\"%lf\" font-size=\"15\" fill=\"purple\" text-anchor=\"middle\">%s</text>\n", x, y, texto);
}


void drawCruzVermelha(FILE* svg, double x, double y) {
    double t = 10;

    fprintf(svg,"<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x-t, y-3, x+t, y-3);

    fprintf(svg, "<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"red\" stroke-width=\"3\" />\n", x, y-t, x, y+t);
}

void drawQuadradoCpf(FILE* svg, double x, double y, const char* cpf) {
    fprintf(svg,"<rect x=\"%lf\" y=\"%lf\" width=\"30\" height=\"30\" fill=\"red\" stroke=\"red\" stroke-width=\"2\" />\n", x-10, y-10);

    
    fprintf(svg,"<text x=\"%lf\" y=\"%lf\" font-size=\"5\" fill=\"black\" text-anchor=\"middle\">%s</text>\n",x, y+2, cpf);
}

void drawCirculoPreto(FILE* svg, double x, double y) {
    fprintf(svg, "<circle cx=\"%lf\" cy=\"%lf\" r=\"5\" fill=\"black\" />\n", x, y);
}
