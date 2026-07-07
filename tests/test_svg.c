#include "../Unity/unity.h"
#include "../include/svg.h"
#include <string.h>
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste.svg");
    remove("testDrawXVermelho.svg");
    remove("testDrawTextoSVG.svg");
    remove("testDrawCruzVermelha.svg");
    remove("testDrawQuadradoCpf.svg");
    remove("testDrawCirculoPreto.svg");    
}


void testCriacao(void) {
    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    endSVG(svg);

    fclose(svg);

    svg = fopen("teste.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);
}


void testDesenho(void) {
    FILE* svg = fopen("teste.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "CEP1", 50, 20, 200, 150, "gray", "black", "5");
    drawQuadra(svg, "CEP2", 400, 20, 200, 150, "pink", "black", "5");

    endSVG(svg);
    fclose(svg);

    svg = fopen("teste.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);
}

void testDrawXVermelho(void) {
    FILE* svg = fopen("testDrawXVermelho.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "cep1", 100, 100, 100, 100, "pink", "black", "1");
    drawXVermelho(svg, 100, 100);

    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawXVermelho.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "<line") != NULL) {
            encontrou = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrou);
}

void testDrawTextoSVG(void) {
    FILE* svg = fopen("testDrawTextoSVG.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    drawTextoSVG(svg, 100, 100, "oi oi oi :)");
    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawTextoSVG.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int encontrou = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "oi oi oi :)") != NULL) {
            encontrou = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(encontrou);
}

void testDrawCruzVermelha(void) {
    FILE* svg = fopen("testDrawCruzVermelha.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "cep1", 100, 100, 200, 100, "pink", "black", "1");
    drawCruzVermelha(svg, 120, 120);

    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawCruzVermelha.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "<line") != NULL) {
            count++;
        }
    }

    fclose(svg);
    TEST_ASSERT_TRUE(count >= 2);
}

void testDrawQuadradoCpf(void) {
    FILE* svg = fopen("testDrawQuadradoCpf.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);

    drawQuadra(svg, "cep1", 200, 200, 100, 100, "pink", "black", "1");
    drawQuadradoCpf(svg, 250, 200, "12345678910");

    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawQuadradoCpf.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[300];
    int achouRect = 0;
    int achouCpf = 0;

    while (fgets(buffer, sizeof(buffer), svg) != NULL) {
        if (strstr(buffer, "<rect") != NULL) achouRect = 1;
        if (strstr(buffer, "12345678910") != NULL) achouCpf = 1;
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achouRect);
    TEST_ASSERT_TRUE(achouCpf);
}

void testDrawCirculoPreto(void) {
    FILE* svg = fopen("testDrawCirculoPreto.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    drawQuadra(svg, "cep1", 200, 200, 100, 100, "pink", "black", "1");
    drawCirculoPreto(svg, 220, 210);
    endSVG(svg);
    fclose(svg);

    svg = fopen("testDrawCirculoPreto.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    char buffer[200];
    int achou = 0;

    while (fgets(buffer, sizeof(buffer), svg)) {
        if (strstr(buffer, "<circle") != NULL) {
            achou = 1;
        }
    }

    fclose(svg);

    TEST_ASSERT_TRUE(achou);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCriacao);
    RUN_TEST(testDesenho);
    RUN_TEST(testDrawXVermelho);
    RUN_TEST(testDrawTextoSVG);
    RUN_TEST(testDrawCruzVermelha);
    RUN_TEST(testDrawQuadradoCpf);
    RUN_TEST(testDrawCirculoPreto);
    
    return UNITY_END();
}