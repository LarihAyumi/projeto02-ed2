#include "../Unity/unity.h"
#include "../include/pessoa.h"
#include <stdlib.h>

void setUp(void) {
}

void tearDown(void) {
}

void testCriarPessoa(void) {
    Pessoa* p = createPessoa(
        "000.000.001-91",
        "Taisa_Ana",
        "Lombardi_De_Angelis",
        'F',
        "26/03/2023"
    );

    TEST_ASSERT_NOT_NULL(p);
    TEST_ASSERT_EQUAL_STRING("000.000.001-91", getCpf(p));
    TEST_ASSERT_EQUAL_STRING("Taisa_Ana", getNome(p));
    TEST_ASSERT_EQUAL_STRING("Lombardi_De_Angelis", getSobrenome(p));
    TEST_ASSERT_EQUAL_CHAR('F', getSexo(p));
    TEST_ASSERT_EQUAL_STRING("26/03/2023", getNasc(p));
    TEST_ASSERT_FALSE(pessoaTemMoradia(p));

    free(p);
}

void testDefinirMoradia(void) {
    Pessoa* p = createPessoa(
        "000.000.001-91",
        "Taisa_Ana",
        "Lombardi_De_Angelis",
        'F',
        "26/03/2023"
    );

    TEST_ASSERT_NOT_NULL(p);

    setMoradia(p, "cep1", 'S', 45, "apto101");

    TEST_ASSERT_TRUE(pessoaTemMoradia(p));
    TEST_ASSERT_EQUAL_STRING("cep1", getCepMoradia(p));
    TEST_ASSERT_EQUAL_CHAR('S', getFaceMoradia(p));
    TEST_ASSERT_EQUAL_INT(45, getNumMoradia(p));
    TEST_ASSERT_EQUAL_STRING("apto101", getCompMoradia(p));

    free(p);
}

void testRemoverMoradia(void) {
    Pessoa* p = createPessoa(
        "000.000.001-91",
        "Taisa_Ana",
        "Lombardi_De_Angelis",
        'F',
        "26/03/2023"
    );

    TEST_ASSERT_NOT_NULL(p);

    setMoradia(p, "cep1", 'S', 45, "apto101");
    removeMoradia(p);

    TEST_ASSERT_FALSE(pessoaTemMoradia(p));
    TEST_ASSERT_EQUAL_STRING("", getCepMoradia(p));
    TEST_ASSERT_EQUAL_CHAR('\0', getFaceMoradia(p));
    TEST_ASSERT_EQUAL_INT(-1, getNumMoradia(p));
    TEST_ASSERT_EQUAL_STRING("", getCompMoradia(p));

    free(p);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCriarPessoa);
    RUN_TEST(testDefinirMoradia);
    RUN_TEST(testRemoverMoradia);

    return UNITY_END();
}