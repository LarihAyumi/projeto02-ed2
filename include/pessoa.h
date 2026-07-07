#ifndef PESSOA_H
#define PESSOA_H
#include <stddef.h>

/** Módulo responsável pela criação e armazenamento das pessoas.
 *  As pessoas são representadas internamente e podem ser salvas em arquivo binário.
 */

//Struct da pessoa
typedef struct Pessoa Pessoa;

/** Retorna o tamanho, em bytes, da estrutura Pessoa
 * @return Tamanho da estrutura Pessoa
 */
size_t getPessoaSize(void);

/** Cria uma Pessoa
 * @param cpf Identificador da pessoa
 * @param nome nome da pessoa
 * @param sobrenome sobrenome da pessoa
 * @param sexo gênero da pessoa
 * @param nasc data em que nasceu
 * @return Ponteiro para a pessoa criada
 */
Pessoa* createPessoa(const char* cpf, const char* nome, const char* sobrenome, char sexo, const char* nasc);

/** Vincula uma pessoa a uma moradia
 * @param p Ponteiro para pessoa que se vinculará
 * @param cep cep onde a pessoa morará
 * @param face lado em que a pessoa morará
 * @param num numero da casa
 * @param comp casa ou apartamento
 */
void setMoradia(Pessoa* p, const char* cep, char face, int num, const char* comp);


/** Exclui uma moradia
 * @param p Ponteiro para pessoa(s) que virará sem-teto
 */
void removeMoradia(Pessoa* p);


//Getters pessoa
const char* getCpf(Pessoa* p);
const char* getNome(Pessoa* p);
const char* getSobrenome(Pessoa* p);
char getSexo(Pessoa* p);
const char* getNasc(Pessoa* p);

//Getters Moradia
int pessoaTemMoradia(Pessoa* p);
const char* getCepMoradia(Pessoa* p);
char getFaceMoradia(Pessoa* p);
int getNumMoradia(Pessoa* p);
const char* getCompMoradia(Pessoa* p);

#endif