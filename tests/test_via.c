#include "../Unity/unity.h"
#include "../include/via.h"
#include "../include/grafo.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {
    FILE* via = fopen("teste.via", "w");
    TEST_ASSERT_NOT_NULL(via);

    fprintf(via, "4\n");
    fprintf(via, "v v1 10.0 10.0\n");
    fprintf(via, "v v2 110.0 10.0\n");
    fprintf(via, "v v3 110.0 110.0\n");
    fprintf(via, "v v4 10.0 110.0\n");
    fprintf(via, "e v1 v2 cep1 cep2 100.0 10.0 Rua_A\n");
    fprintf(via, "e v2 v3 cep2 cep3 100.0 8.0 Rua_B\n");
    fprintf(via, "e v3 v4 cep3 cep4 100.0 12.0 Rua_C\n");

    fclose(via);
}

void tearDown(void) {
    remove("teste.via");
}

void testViaCriaGrafo(void) {
    Grafo* grafo = processVia("teste.via");

    TEST_ASSERT_NOT_NULL(grafo);

    destruirGrafo(grafo);
}

void testViaLeVertices(void) {
    Grafo* grafo = processVia("teste.via");

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(4, contarVerticesGrafo(grafo));

    destruirGrafo(grafo);
}

void testViaLeArestas(void) {
    Grafo* grafo = processVia("teste.via");

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(3, contarArestasGrafo(grafo));

    destruirGrafo(grafo);
}

void testViaArquivoInexistente(void) {
    Grafo* grafo = processVia("arquivo_inexistente.via");

    TEST_ASSERT_NULL(grafo);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testViaCriaGrafo);
    RUN_TEST(testViaLeVertices);
    RUN_TEST(testViaLeArestas);
    RUN_TEST(testViaArquivoInexistente);

    return UNITY_END();
}