#include "../Unity/unity.h"
#include "../include/grafo.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste_grafo.svg");
}

void testGrafoCriaVazio(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);
    TEST_ASSERT_EQUAL_INT(0, contarVerticesGrafo(grafo));
    TEST_ASSERT_EQUAL_INT(0, contarArestasGrafo(grafo));

    destruirGrafo(grafo);
}

void testGrafoInsereVertices(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 20.0);
    inserirVertice(grafo, "v2", 30.0, 40.0);

    TEST_ASSERT_EQUAL_INT(2, contarVerticesGrafo(grafo));
    TEST_ASSERT_EQUAL_INT(0, contarArestasGrafo(grafo));

    destruirGrafo(grafo);
}

void testGrafoNaoInsereVerticeRepetido(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 20.0);
    inserirVertice(grafo, "v1", 30.0, 40.0);

    TEST_ASSERT_EQUAL_INT(1, contarVerticesGrafo(grafo));

    destruirGrafo(grafo);
}

void testGrafoInsereAresta(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 20.0);
    inserirVertice(grafo, "v2", 30.0, 40.0);

    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 100.0, 10.0, "Rua_A");

    TEST_ASSERT_EQUAL_INT(2, contarVerticesGrafo(grafo));
    TEST_ASSERT_EQUAL_INT(1, contarArestasGrafo(grafo));

    destruirGrafo(grafo);
}

void testGrafoNaoInsereArestaComVerticeInexistente(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 20.0);

    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 100.0, 10.0, "Rua_A");

    TEST_ASSERT_EQUAL_INT(1, contarVerticesGrafo(grafo));
    TEST_ASSERT_EQUAL_INT(0, contarArestasGrafo(grafo));

    destruirGrafo(grafo);
}

void testGrafoDesenhaSvg(void) {
    Grafo* grafo = criarGrafo();
    FILE* svg;

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 20.0);
    inserirVertice(grafo, "v2", 30.0, 40.0);
    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 100.0, 10.0, "Rua_A");

    svg = fopen("teste_grafo.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    fprintf(svg, "<svg xmlns=\"http://www.w3.org/2000/svg\">\n");
    desenharGrafoSvg(grafo, svg);
    fprintf(svg, "</svg>\n");

    fclose(svg);

    svg = fopen("teste_grafo.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);

    destruirGrafo(grafo);
}

void testGrafoAtualizaVelocidadeRegiao(void) {
    Grafo* grafo = criarGrafo();

    TEST_ASSERT_NOT_NULL(grafo);

    inserirVertice(grafo, "v1", 10.0, 10.0);
    inserirVertice(grafo, "v2", 50.0, 50.0);
    inserirVertice(grafo, "v3", 200.0, 200.0);

    inserirAresta(grafo, "v1", "v2", "cep1", "cep2", 100.0, 5.0, "Rua_A");
    inserirAresta(grafo, "v1", "v3", "cep1", "cep3", 100.0, 7.0, "Rua_B");

    TEST_ASSERT_EQUAL_INT(1, atualizarVelocidadeRegiao(grafo, 12.5, 0.0, 0.0, 100.0, 100.0));

    TEST_ASSERT_EQUAL_INT(125, (int)(obterVelocidadeAresta(grafo, "v1", "v2") * 10 + 0.5));
    TEST_ASSERT_EQUAL_INT(70, (int)(obterVelocidadeAresta(grafo, "v1", "v3") * 10 + 0.5));
   
    destruirGrafo(grafo);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testGrafoCriaVazio);
    RUN_TEST(testGrafoInsereVertices);
    RUN_TEST(testGrafoNaoInsereVerticeRepetido);
    RUN_TEST(testGrafoInsereAresta);
    RUN_TEST(testGrafoNaoInsereArestaComVerticeInexistente);
    RUN_TEST(testGrafoDesenhaSvg);
    RUN_TEST(testGrafoAtualizaVelocidadeRegiao);

    return UNITY_END();
}