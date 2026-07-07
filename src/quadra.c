#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/quadra.h"

struct Quadra {
    char cep[20];
    double x, y, w, h;
};

size_t getQuadraSize(void) {
    return sizeof(Quadra);
}

Quadra* createQuadra(const char* cep, double x, double y, double w, double h) {
    Quadra* q = malloc(sizeof(Quadra));

    strcpy(q->cep, cep);
    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;

    return q;
}

void saveQuadra(Quadra* q, FILE* file, long* offset) {
    fseek(file, 0, SEEK_END);
    *offset = ftell(file);
    fwrite(q, sizeof(Quadra), 1, file);
}


//getters
const char* getCepQuadra(Quadra* q) {
    return q->cep;
}

double getXQuadra(Quadra* q) {
    return q->x;
}

double getYQuadra(Quadra* q) {
    return q->y;
}

double getWQuadra(Quadra* q) {
    return q->w;
}

double getHQuadra(Quadra* q) {
    return q->h;
}