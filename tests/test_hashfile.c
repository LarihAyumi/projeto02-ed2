#include "../Unity/unity.h"
#include "../include/hashfile.h"
#include <stdio.h>

void setUp(void) {
}

void tearDown(void) {
    remove("teste_hash.hf");
    remove("teste_hash.hfc");
    remove("teste_hash_persist.hf");
    remove("teste_hash_persist.hfc");
    remove("saida.hfd");
    remove("teste_hash_scan.hf");
    remove("teste_hash_scan.hfc");
}

typedef struct {
    int quantidade;
    int soma;
} ScanContext;

void contarRegistrosCallback(const char* key, void* record, void* extra) {
    (void) key;

    ScanContext* ctx = (ScanContext*) extra;
    int* valor = (int*) record;

    ctx->quantidade++;
    ctx->soma += *valor;
}

void testCreateFile(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));

    TEST_ASSERT_NOT_NULL(hf);

    closeFile(hf);
}

void testInsertRegistereSearchRegister(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int valorInserido = 100;
    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "10", &valorInserido));
    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "10", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(100, valorEncontrado);

    closeFile(hf);
}

void testInsertRegisterAtt(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int valor1 = 100;
    int valor2 = 999;
    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "10", &valor1));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "10", &valor2));

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "10", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(999, valorEncontrado);

    closeFile(hf);
}

void testRemoveRegister(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int valor = 200;
    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "20", &valor));
    TEST_ASSERT_EQUAL_INT(0, removeRegister(hf, "20"));
    TEST_ASSERT_EQUAL_INT(-1, searchRegister(hf, "20", &valorEncontrado));

    closeFile(hf);
}

void testSearchRegister(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(-1, searchRegister(hf, "999", &valorEncontrado));

    closeFile(hf);
}

void testPersistencia(void) {
    HashFile* hf = createFile("teste_hash_persistencia", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int v1 = 111;
    int v2 = 222;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "1", &v1));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "2", &v2));

    closeFile(hf);

    hf = openFile("teste_hash_persistencia");
    TEST_ASSERT_NOT_NULL(hf);

    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "1", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(111, valorEncontrado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "2", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(222, valorEncontrado);

    closeFile(hf);
}

void testSplitBucket(void) {
    HashFile* hf = createFile("teste_hash", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int v1 = 100;
    int v2 = 200;
    int v3 = 300;
    int v4 = 400;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "10", &v1));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "20", &v2));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "30", &v3));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "40", &v4));

    int valorEncontrado = 0;

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "10", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(100, valorEncontrado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "20", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(200, valorEncontrado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "30", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(300, valorEncontrado);

    TEST_ASSERT_EQUAL_INT(0, searchRegister(hf, "40", &valorEncontrado));
    TEST_ASSERT_EQUAL_INT(400, valorEncontrado);

    closeFile(hf);
}

void testScanRegistradores(void) {
    HashFile* hf = createFile("teste_hash_scan", sizeof(int));
    TEST_ASSERT_NOT_NULL(hf);

    int v1 = 10, v2 = 20, v3 = 30, v4 = 40;

    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "a", &v1));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "b", &v2));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "c", &v3));
    TEST_ASSERT_EQUAL_INT(0, insertRegister(hf, "d", &v4));

    ScanContext ctx;
    ctx.quantidade = 0;
    ctx.soma = 0;

    scanRegisters(hf, contarRegistrosCallback, &ctx);

    TEST_ASSERT_EQUAL_INT(4, ctx.quantidade);
    TEST_ASSERT_EQUAL_INT(100, ctx.soma);

    closeFile(hf);
}


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(testCreateFile);
    RUN_TEST(testInsertRegistereSearchRegister);
    RUN_TEST(testInsertRegisterAtt);
    RUN_TEST(testRemoveRegister);
    RUN_TEST(testSearchRegister);
    RUN_TEST(testPersistencia);
    RUN_TEST(testSplitBucket);
    RUN_TEST(testScanRegistradores);

    return UNITY_END();
}