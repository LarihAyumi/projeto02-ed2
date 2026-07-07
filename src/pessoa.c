#include "../include/pessoa.h"
#include <stdlib.h>
#include <string.h>

struct Pessoa {
    char cpf[20];
    char nome[50];
    char sobrenome[50];
    char sexo;
    char nasc[11];

    int morador;
    char cep[20];
    char face;
    int num;
    char comp[50];
};

size_t getPessoaSize(void) {
    return sizeof(Pessoa);
}

Pessoa* createPessoa(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc) {
    Pessoa* p = malloc(sizeof(Pessoa));

    strcpy(p->cpf, cpf);
    strcpy(p->nome, nome);
    strcpy(p->sobrenome, sobrenome);
    p->sexo = sexo;
    strcpy(p->nasc, nasc);

    p->morador = 0;
    p->cep[0] = '\0';
    p->face = '\0';
    p->num = -1;
    p->comp[0] = '\0';

    return p;
}

void setMoradia(Pessoa* p, const char* cep, char face, int num, const char* comp) {
    p->morador = 1;
    strcpy(p->cep, cep);
    p->face = face;
    p->num = num;
    strcpy(p->comp, comp);
}

void removeMoradia(Pessoa* p) {
    p->morador = 0;
    p->cep[0] = '\0';
    p->face = '\0';
    p->num = -1;
    p->comp[0] = '\0';
}


//getters
const char* getCpf(Pessoa* p) {
    return p->cpf;
}

const char* getNome(Pessoa* p) {
    return p->nome;
}

const char* getSobrenome(Pessoa* p) {
    return p->sobrenome;
}

char getSexo(Pessoa* p) {
    return p->sexo;
}

const char* getNasc(Pessoa* p) {
    return p->nasc;
}

int pessoaTemMoradia(Pessoa* p) {
    return p->morador;
}

const char* getCepMoradia(Pessoa* p) {
    return p->cep;
}

char getFaceMoradia(Pessoa* p) {
    return p->face;
}

int getNumMoradia(Pessoa* p) {
    return p->num;
}

const char* getCompMoradia(Pessoa* p) {
    return p->comp;
}