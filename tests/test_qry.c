#include "../Unity/unity.h"
#include "../include/qry.h"
#include "../include/svg.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include "../include/pessoa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "h? 800.577.369-28\n");

    fclose(qry);
}

void tearDown(void) {
    remove("teste.qry");
    remove("teste_qry_hash.hf");
    remove("teste_qry_hash.hfc");
    remove("teste_quadras_hash.hf");
    remove("teste_quadras_hash.hfc");
    remove("saida.hfd");

    remove("testeRQ.txt");
    remove("testeRQ.svg");

    remove("testePQ.txt");
    remove("testePQ.svg");

    remove("testeCENSO.txt");
    remove("testeCENSO.svg");

    remove("testeDSPJ.txt");
    remove("testeDSPJ.svg");

    remove("testeH.txt");
    remove("testeH.svg");

    remove("testeMUD.txt");
    remove("testeMUD.svg");

    remove("testeNasc.txt");
    remove("testeNasc.svg");

    remove("testeRIP.txt");
    remove("testeRIP.svg");

    remove("testeO.txt");
    remove("testeO.svg");

    remove("testeMvm.txt");
    remove("testeMvm.svg");

    remove("testeRegs.txt");
    remove("testeRegs.svg");
}

void testRq(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "rq cep1\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());

    TEST_ASSERT_NOT_NULL(pessoasHash);
    TEST_ASSERT_NOT_NULL(quadrasHash);


    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);
    insertRegister(quadrasHash, "cep1", q);
    free(q);

    Pessoa* p = createPessoa("123", "Larissa", "Costa", 'F', "26/08/2004");
    setMoradia(p, "cep1", 'S', 10, "casa");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeRQ.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeRQ.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    svg = fopen("testeRQ.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char bufferSvg[300];
    int encontrouLine = 0;
    int encontrouRed = 0;

    while (fgets(bufferSvg, sizeof(bufferSvg), svg)) {
        if (strstr(bufferSvg, "<line") != NULL) encontrouLine = 1;
        if (strstr(bufferSvg, "stroke=\"red\"") != NULL) encontrouRed = 1;
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrouLine);
    TEST_ASSERT_TRUE(encontrouRed);

    Pessoa* res = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(res);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "123", res));
    TEST_ASSERT_FALSE(pessoaTemMoradia(res));

    free(res);

    txt = fopen("testeRQ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrou = 1;
        }
    }
    fclose(txt);

    TEST_ASSERT_TRUE(encontrou);

    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testPq(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "pq cep1\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());

    TEST_ASSERT_NOT_NULL(pessoasHash);
    TEST_ASSERT_NOT_NULL(quadrasHash);

    //quadra
    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);
    insertRegister(quadrasHash, "cep1", q);
    free(q);

    //pessoas
    Pessoa* p1 = createPessoa("1", "A", "A", 'F', "01");
    setMoradia(p1, "cep1", 'N', 10, "casa");

    Pessoa* p2 = createPessoa("2", "B", "B", 'F', "01");
    setMoradia(p2, "cep1", 'S', 10, "casa");

    Pessoa* p3 = createPessoa("3", "C", "C", 'F', "01");
    setMoradia(p3, "cep1", 'S', 10, "casa");

    insertRegister(pessoasHash, "1", p1);
    insertRegister(pessoasHash, "2", p2);
    insertRegister(pessoasHash, "3", p3);

    free(p1); free(p2); free(p3);

    FILE* txt = fopen("testePQ.txt", "w");
    FILE* svg = fopen("testePQ.svg", "w");

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    svg = fopen("testePQ.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char bufferSvg[300];
    int encontrouText = 0;
    int encontrouTotalSvg = 0;

    while (fgets(bufferSvg, sizeof(bufferSvg), svg) != NULL) {
        if (strstr(bufferSvg, "<text") != NULL) {
            encontrouText = 1;
        }

        if (strstr(bufferSvg, ">3<") != NULL) {
            encontrouTotalSvg = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrouText);
    TEST_ASSERT_TRUE(encontrouTotalSvg);

    txt = fopen("testePQ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrouTotal = 0;

    while (fgets(buffer, sizeof(buffer), txt)) {
        if (strstr(buffer, "Total: 3") != NULL) {
            encontrouTotal = 1;
        }
    }

    TEST_ASSERT_TRUE(encontrouTotal);

    fclose(txt);

    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testCenso(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);
    fprintf(qry, "censo\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL( pessoasHash);

    Pessoa* p1 = createPessoa("1", "Larissa", "Costa", 'F', "26/08/2004");
    setMoradia(p1, "cep1", 'N', 10, "apartamento");
    Pessoa* p2 = createPessoa("2", "Bruna", "Yokoshiro", 'F', "23/09/2005");
    setMoradia(p2, "cep1", 'S', 20, "casa");
    Pessoa* p3 = createPessoa("3", "Victor", "Hirota", 'M', "13/01/2005");

    insertRegister(pessoasHash, "1",p1);
    insertRegister(pessoasHash, "2",p2);
    insertRegister(pessoasHash, "3",p3);

    free(p1);
    free(p2);
    free(p3);

    FILE* txt = fopen("testeCENSO.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeCENSO.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg, NULL);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeCENSO.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int achouHabitantes = 0;
    int achouMoradores = 0;
    int achouSemTetos = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "3 HABITANTES") != NULL) achouHabitantes = 1;
        if (strstr(buffer, "Moradores: 2") != NULL) achouMoradores = 1;
        if (strstr(buffer, "Sem-tetos: 1") != NULL) achouSemTetos = 1;
    }

    TEST_ASSERT_TRUE(achouHabitantes);
    TEST_ASSERT_TRUE(achouMoradores);
    TEST_ASSERT_TRUE(achouSemTetos);

    fclose(txt);
    closeFile(pessoasHash);
}

void testH(void) {
    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    Pessoa* p = createPessoa( "800.577.369-28", "Larissa", "Costa", 'F', "26/08/2004");

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeH.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeH.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    processQry("teste.qry", pessoasHash, NULL, txt, svg, NULL);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeH.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];

    TEST_ASSERT_NOT_NULL(fgets(buffer, sizeof(buffer), txt));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Larissa"));
    TEST_ASSERT_NOT_NULL(strstr(buffer, "Costa"));

    fclose(txt);
    closeFile(pessoasHash);
}

void testNasc(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "nasc 123 Larissa Costa F 26/08/2004\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    FILE* txt = fopen("testeNasc.txt", "w");
    FILE* svg = fopen("testeNasc.svg", "w");

    processQry("teste.qry", pessoasHash, NULL, txt, svg, NULL);

    fclose(txt);
    fclose(svg);

    // pra verificar se a pessoa foi inserida
    Pessoa* p = malloc(getPessoaSize());

    int res = searchRegister(pessoasHash, "123", p);

    TEST_ASSERT_EQUAL_INT(0, res);
    TEST_ASSERT_EQUAL_STRING("Larissa", getNome(p));
    TEST_ASSERT_EQUAL_STRING("26/08/2004", getNasc(p));

    free(p);
    closeFile(pessoasHash);
}


void testRip(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "rip 800.577.369-28\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);
    Pessoa* p = createPessoa( "800.577.369-28", "Larissa", "Costa", 'F', "26/08/2004");
    setMoradia(p, "cep1", 'S', 50, "casa");

    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);
    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);

    insertRegister(pessoasHash, getCpf(p), p);
    free(p);
    insertRegister(quadrasHash, "cep1", q);
    free(q);

    FILE* txt = fopen("testeRIP.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeRIP.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);
    

    fclose(txt);
    fclose(svg);

    Pessoa* removida = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(removida);

    TEST_ASSERT_EQUAL_INT(-1, searchRegister(pessoasHash, "800.577.369-28", removida));

    free(removida);

    svg = fopen("testeRIP.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char bufferSVG[300];
    int encontrouLine = 0;

    while (fgets(bufferSVG, sizeof(bufferSVG), svg)) {
        if (strstr(bufferSVG, "<line") != NULL) {
            encontrouLine = 1;
        }
    }

    fclose(svg);
    TEST_ASSERT_TRUE(encontrouLine);

    txt = fopen("testeRIP.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrou = 1;
            break;
        }
    }

    TEST_ASSERT_TRUE(encontrou);

    fclose(txt);
    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testMud(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "mud 800.577.369-28 cep1 S 50 casa\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);
    Pessoa* p = createPessoa("800.577.369-28", "Larissa", "Costa", 'F',"26/08/2004");

    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);
    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);

    insertRegister(quadrasHash, "cep1", q);
    free(q);
    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeMUD.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeMUD.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    svg = fopen("testeMUD.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char bufferSvg[300];
    int achouRect = 0;
    int achouCpf = 0;

    while (fgets(bufferSvg, sizeof(bufferSvg), svg) != NULL) {
        if (strstr(bufferSvg, "<rect") != NULL) achouRect = 1;
        if (strstr(bufferSvg, "800.577.369-28") != NULL) achouCpf = 1;
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achouRect);
    TEST_ASSERT_TRUE(achouCpf);

    Pessoa* resultado = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(resultado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "800.577.369-28", resultado));
    TEST_ASSERT_TRUE(pessoaTemMoradia(resultado));
    TEST_ASSERT_EQUAL_STRING("cep1", getCepMoradia(resultado));
    TEST_ASSERT_EQUAL_CHAR('S', getFaceMoradia(resultado));
    TEST_ASSERT_EQUAL_INT(50, getNumMoradia(resultado));
    TEST_ASSERT_EQUAL_STRING("casa", getCompMoradia(resultado));

    free(resultado);
    closeFile(pessoasHash);
}

void testDspj(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "dspj 800.577.369-28\n");

    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);
    Pessoa* p = createPessoa("800.577.369-28", "Larissa", "Costa", 'F',"26/08/2004");
    setMoradia(p, "cep1", 'S', 45, "casa");

    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);
    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);


    insertRegister(quadrasHash, "cep1", q);
    free(q);
    insertRegister(pessoasHash, getCpf(p), p);
    free(p);

    FILE* txt = fopen("testeDSPJ.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeDSPJ.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    Pessoa* resultado = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(resultado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "800.577.369-28", resultado));
    TEST_ASSERT_FALSE(pessoaTemMoradia(resultado));

    free(resultado);

    svg = fopen("testeDSPJ.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char bufferSvg[300];
    int achouCircle = 0;

    while (fgets(bufferSvg, sizeof(bufferSvg), svg)) {
        if (strstr(bufferSvg, "<circle") != NULL) {
            achouCircle = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achouCircle);

    txt = fopen("testeDSPJ.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[200];
    int encontrouNome = 0;
    int encontrouEndereco = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Larissa") != NULL) {
            encontrouNome = 1;
        }
        if (strstr(buffer, "cep1") != NULL) {
            encontrouEndereco = 1;
        }
    }

    TEST_ASSERT_TRUE(encontrouNome);
    TEST_ASSERT_TRUE(encontrouEndereco);

    fclose(txt);
    closeFile(pessoasHash);
}

void testO(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "@o? R0 cep1 S 50\n");
    fclose(qry);

    HashFile* pessoasHash = createFile("teste_qry_hash", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    HashFile* quadrasHash = createFile("teste_quadras_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);
    Quadra* q = createQuadra("cep1", 200, 200, 100, 100);

    insertRegister(quadrasHash, "cep1", q);
    free(q);

    FILE* txt = fopen("testeO.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    FILE* svg = fopen("testeO.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", pessoasHash, quadrasHash, txt, svg, NULL);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeO.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[300];
    int achouReg = 0;
    int achouX = 0;
    int achouY = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "R0:") != NULL) achouReg = 1;
        if (strstr(buffer, "250.00") != NULL) achouX = 1;
        if (strstr(buffer, "200.00") != NULL) achouY = 1;
    }

    fclose(txt);

    TEST_ASSERT_TRUE(achouReg);
    TEST_ASSERT_TRUE(achouX);
    TEST_ASSERT_TRUE(achouY);

    svg = fopen("testeO.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    int achouLine = 0;
    int achouTracejado = 0;
    int achouTexto = 0;

    while (fgets(buffer, sizeof(buffer), svg) != NULL) {
        if (strstr(buffer, "<line") != NULL) achouLine = 1;
        if (strstr(buffer, "stroke-dasharray") != NULL) achouTracejado = 1;
        if (strstr(buffer, "R0") != NULL) achouTexto = 1;
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achouLine);
    TEST_ASSERT_TRUE(achouTracejado);
    TEST_ASSERT_TRUE(achouTexto);

    closeFile(pessoasHash);
    closeFile(quadrasHash);
}

void testMvm(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "mvm 12.5 0 0 100 100\n");
    fclose(qry);

    Grafo* grafo = criarGrafo();
    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 10.0);
    inserirVertice(grafo, "v2", 50.0, 50.0);
    inserirVertice(grafo, "v3", 200.0, 200.0);

    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 100.0, 5.0, "Rua_A");
    inserirAresta(grafo, "v1", "v3", "cep1", "cep3", 100.0, 7.0, "Rua_B");

    FILE* txt = fopen("testeMvm.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);

    processQry("teste.qry", NULL, NULL, txt, NULL, grafo);

    fclose(txt);

    TEST_ASSERT_EQUAL_INT(125, (int)(obterVelocidadeAresta(grafo, "v1", "v2") * 10 + 0.5));
    TEST_ASSERT_EQUAL_INT(70, (int)(obterVelocidadeAresta(grafo, "v1", "v3") * 10 + 0.5));
    
    destruirGrafo(grafo);
}

void testRegs(void) {
    FILE* qry = fopen("teste.qry", "w");
    TEST_ASSERT_NOT_NULL(qry);

    fprintf(qry, "regs 5.0\n");
    fclose(qry);

    Grafo* grafo = criarGrafo();
    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 0.0, 0.0);
    inserirVertice(grafo, "v2", 50.0, 20.0);
    inserirVertice(grafo, "v3", 100.0, 10.0);
    inserirVertice(grafo, "v4", 300.0, 300.0);
    inserirVertice(grafo, "v5", 350.0, 330.0);

    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 50.0, 10.0, "Rua_A");
    inserirAresta(grafo, "v2", "v3", "cep2", "cep3", 50.0, 10.0, "Rua_A");
    inserirAresta(grafo, "v4", "v5", "cep4", "cep5", 50.0, 10.0, "Rua_B");

    FILE* txt = fopen("testeRegs.txt", "w");
    TEST_ASSERT_NOT_NULL(txt);
    FILE* svg = fopen("testeRegs.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processQry("teste.qry", NULL, NULL, txt, svg, grafo);
    endSVG(svg);

    fclose(txt);
    fclose(svg);

    txt = fopen("testeRegs.txt", "r");
    TEST_ASSERT_NOT_NULL(txt);

    char buffer[300];
    int achouComponentes = 0;
    int achouQtd = 0;

    while (fgets(buffer, sizeof(buffer), txt) != NULL) {
        if (strstr(buffer, "Componentes conexos") != NULL) {
            achouComponentes = 1;
        }

        if (strstr(buffer, "2") != NULL) {
            achouQtd = 1;
        }
    }

    fclose(txt);

    TEST_ASSERT_TRUE(achouComponentes);
    TEST_ASSERT_TRUE(achouQtd);

    svg = fopen("testeRegs.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    int achouRect = 0;
    int achouOpacity = 0;

    while (fgets(buffer, sizeof(buffer), svg) != NULL) {
        if (strstr(buffer, "<rect") != NULL) {
            achouRect = 1;
        }

        if (strstr(buffer, "fill-opacity=\"0.5\"") != NULL) {
            achouOpacity = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achouRect);
    TEST_ASSERT_TRUE(achouOpacity);
    destruirGrafo(grafo);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testRq);
    RUN_TEST(testPq);
    RUN_TEST(testCenso);
    RUN_TEST(testH);
    RUN_TEST(testNasc);
    RUN_TEST(testRip);
    RUN_TEST(testMud);
    RUN_TEST(testDspj);
    RUN_TEST(testO);
    RUN_TEST(testMvm);
    RUN_TEST(testRegs);

    return UNITY_END();
}