#include "../Unity/unity.h"
#include "../include/geo.h"
#include "../include/svg.h"
#include "../include/hashfile.h"
#include "../include/quadra.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void) {
    FILE* geo = fopen("teste.geo", "w");
    TEST_ASSERT_NOT_NULL(geo);

    fprintf(geo, "cq 2 gray black\n");
    fprintf(geo, "q cep1 10 20 100 50\n");
    fprintf(geo, "q cep2 200 30 80 40\n");

    fclose(geo);
}

void tearDown(void) {
    remove("teste.geo");
    remove("teste_geo.svg");
    remove("teste_geo_hash.hf");
    remove("teste_geo_hash.hfc");
    remove("quadras.dat");
    remove("saida.hfd");
}

void testGeoCriaSvg(void) {
    HashFile* quadrasHash = createFile("teste_geo_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);

    FILE* svg = fopen("teste_geo.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);
    
    startSVG(svg);
    processGeo("teste.geo", quadrasHash, svg);
    endSVG(svg);

    fclose(svg);

    svg = fopen("teste_geo.svg", "r");
    TEST_ASSERT_NOT_NULL(svg);

    fclose(svg);
    closeFile(quadrasHash);
}

void testGeoInsereQuadras(void) {
    HashFile* quadrasHash = createFile("teste_geo_hash", getQuadraSize());
    TEST_ASSERT_NOT_NULL(quadrasHash);

    FILE* svg = fopen("teste_geo.svg", "w");
    TEST_ASSERT_NOT_NULL(svg);

    startSVG(svg);
    processGeo("teste.geo", quadrasHash, svg);
    endSVG(svg);

    fclose(svg);

    Quadra* q = malloc(getQuadraSize());
    TEST_ASSERT_NOT_NULL(q);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(quadrasHash, "cep1", q));
    TEST_ASSERT_EQUAL_STRING("cep1", getCepQuadra(q));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 10.0, getXQuadra(q));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 20.0, getYQuadra(q));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 100.0, getWQuadra(q));
    TEST_ASSERT_FLOAT_WITHIN(0.0001, 50.0, getHQuadra(q));

    TEST_ASSERT_EQUAL_INT(0, searchRegister(quadrasHash, "cep2", q));
    TEST_ASSERT_EQUAL_STRING("cep2", getCepQuadra(q));

    free(q);
    closeFile(quadrasHash);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testGeoCriaSvg);
    RUN_TEST(testGeoInsereQuadras);

    return UNITY_END();
}