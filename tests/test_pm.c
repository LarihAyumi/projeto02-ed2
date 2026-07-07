#include "../Unity/unity.h"
#include "../include/hashfile.h"
#include "../include/pessoa.h"
#include "../include/pm.h"
#include <stdlib.h>
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste_pessoas.hf");
    remove("teste_pessoas.hfc");
    remove("teste_pessoas.hfd");
    remove("saida.hfd");
}

void testCarregaPessoa(void) {
    HashFile* pessoasHash = createFile("teste_pessoas", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    processPM("tests/test_pm.pm", pessoasHash);

    Pessoa* p = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(p);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "000.000.001-91", p));
    TEST_ASSERT_EQUAL_STRING("Taisa_Ana", getNome(p));
    TEST_ASSERT_EQUAL_STRING("Lombardi_De_Angelis", getSobrenome(p));
    TEST_ASSERT_EQUAL_CHAR('F', getSexo(p));

    free(p);
    closeFile(pessoasHash);
}

void testCarregaMoradia(void) {
    HashFile* pessoasHash = createFile("teste_pessoas", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    processPM("tests/test_pm.pm", pessoasHash);

    Pessoa* p = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(p);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "000.000.001-91", p));
    TEST_ASSERT_TRUE(pessoaTemMoradia(p));
    TEST_ASSERT_EQUAL_STRING("cep1", getCepMoradia(p));
    TEST_ASSERT_EQUAL_CHAR('S', getFaceMoradia(p));
    TEST_ASSERT_EQUAL_INT(45, getNumMoradia(p));
    TEST_ASSERT_EQUAL_STRING("casa", getCompMoradia(p));

    free(p);
    closeFile(pessoasHash);
}

void testMantemSemTeto(void) {
    HashFile* pessoasHash = createFile("teste_pessoas", getPessoaSize());
    TEST_ASSERT_NOT_NULL(pessoasHash);

    processPM("tests/test_pm.pm", pessoasHash);

    Pessoa* p = malloc(getPessoaSize());
    TEST_ASSERT_NOT_NULL(p);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(pessoasHash, "000.000.011-63", p));
    TEST_ASSERT_FALSE(pessoaTemMoradia(p));

    free(p);
    closeFile(pessoasHash);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCarregaPessoa);
    RUN_TEST(testCarregaMoradia);
    RUN_TEST(testMantemSemTeto);

    return UNITY_END();
}