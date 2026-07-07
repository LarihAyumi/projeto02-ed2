#ifndef HASHFILE_H
#define HASHFILE_H
#include <stdio.h>
#include <stddef.h>

/**
 * Módulo de hashfile extensível em disco.
 * Armazena registros completos associados a chaves alfanuméricas.
 * Usa arquivo .hf para buckets e .hfc para cabeçalho/diretório.
 */


//Estrutura arquivo hash
typedef struct HashFile HashFile;


/** Cria um hashfile em disco
 * @param filename Nome base do arquivo
 * @param sizeB Tamanho em bytes de cada registro
 * @return Ponteiro para HashFile ou NULL em caso de erro
 */
HashFile* createFile(const char* filename, size_t sizeB);


/** Abre um arquivo hash existente
 * @param filename Nome do arquivo
 * @return Ponteiro para HashFile/NULL
 */
HashFile* openFile(const char* filename);


/** Insere um registro completo associado a uma chave alfanumérica
 * @param file Hashfile
 * @param key Chave alfanumérica
 * @param record Ponteiro para o registro completo
 * @return 0 em sucesso, -1 em erro
 */
int insertRegister(HashFile* file, const char* key, const void* record);

/** Busca um registro pela chave
 * @param file Hashfile
 * @param key Chave alfanumérica
 * @param outRecord Ponteiro para onde o registro encontrado será copiado
 * @return 0 se encontrado, -1 se não encontrado
 */
int searchRegister(HashFile* file, const char* key, void* outRecord);


/** Remove um registro pela chave
 * @param file Hashfile
 * @param key Chave alfanumérica
 * @return 0 em sucesso, -1 em erro
 */
int removeRegister(HashFile* file, const char* key);


/** Gera arquivo HFD com a representação do hashfile
 * @param h estrutura do hashfile
 * @param filename nome do arquivo que será gerado hfd
 */
void generateHFD(HashFile* h, const char* filename);

/** Fecha o hashfile, salva o cabeçalho e libera a memória
 * @param file estrutura do hashfile
 */
void closeFile(HashFile* file);

/** Callback pra fazer a varredura do hashfile
 * @param key Chave associada ao registro encontrado
 * @param record Ponteiro para o registro armazenado
 * @param extra Ponteiro auxiliar passado pelo usuário para guardar contexto
 */
typedef void (*HashScanCallback)(const char* key, void* record, void* extra);

/** Percorre todos os registros válidos armazenados no hashfile
 *  - Cada registro é enviado para a função callback informada.
 *  - Buckets repetidos no diretório são ignorados para evitar visitar o mesmo registro mais de uma vez.
 * @param h Hashfile a ser percorrido
 * @param callback Função chamada para cada registro encontrado
 * @param extra Ponteiro auxiliar repassado ao callback
 */
void scanRegisters(HashFile* h, HashScanCallback callback, void* extra);

#endif 