#include "../Unity/unity.h"
#include "../include/quadra.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste_quadra.dat");
}

void testCriarQuadra(void) {
    Quadra* q = createQuadra("cep1", 10.0, 20.0, 100.0, 50.0);

    TEST_ASSERT_NOT_NULL(q);
    TEST_ASSERT_EQUAL_STRING("cep1", getCepQuadra(q));
    TEST_ASSERT_EQUAL_FLOAT(10.0, getXQuadra(q));
    TEST_ASSERT_EQUAL_FLOAT(20.0, getYQuadra(q));
    TEST_ASSERT_EQUAL_FLOAT(100.0, getWQuadra(q));
    TEST_ASSERT_EQUAL_FLOAT(50.0, getHQuadra(q));

    free(q);
}

void testSalvarQuadra(void) {
    Quadra* q = createQuadra("cep1", 10.0, 20.0, 100.0, 50.0);
    TEST_ASSERT_NOT_NULL(q);

    FILE* file = fopen("teste_quadra.dat", "wb+");
    TEST_ASSERT_NOT_NULL(file);

    long offset = -1;
    saveQuadra(q, file, &offset);

    TEST_ASSERT_EQUAL_INT(0, offset);

    Quadra* lida = malloc(getQuadraSize());
    TEST_ASSERT_NOT_NULL(lida);

    fseek(file, offset, SEEK_SET);
    fread(lida, getQuadraSize(), 1, file);

    TEST_ASSERT_EQUAL_MEMORY(q, lida, getQuadraSize());

    fclose(file);
    free(lida);
    free(q);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCriarQuadra);
    RUN_TEST(testSalvarQuadra);

    return UNITY_END();
}