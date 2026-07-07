#include "../include/pm.h"
#include "../include/pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processPM(const char* pmPath, HashFile* pessoasHash) {
    FILE* pm = fopen(pmPath, "r");

    if (!pm) {
        printf("Erro ao abrir arquivo PM: %s\n", pmPath);
        return;
    }

    char comando[10];

    while (fscanf(pm, "%s", comando) != EOF) {
        if (strcmp(comando, "p") == 0) {
            char cpf[20], nome[50], sobrenome[50], nasc[11];
            char sexo;

            fscanf(pm, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nasc);
            Pessoa* p = createPessoa(cpf, nome, sobrenome, sexo, nasc);

            insertRegister(pessoasHash, cpf, p);

            free(p);
        }

        else if (strcmp(comando, "m") == 0) {
            char cpf[20], cep[20], compl[50];
            char face;
            int num;

            fscanf(pm, "%s %s %c %d %s", cpf, cep, &face, &num, compl);

            void* p = malloc(getPessoaSize());

            if (searchRegister(pessoasHash, cpf, p) == 0) {
                setMoradia((Pessoa*) p, cep, face, num, compl);
                insertRegister(pessoasHash, cpf, p);
            }
            free(p);
        }
    }

    fclose(pm);
}