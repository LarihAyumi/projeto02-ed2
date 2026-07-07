#ifndef QRY_H
#define QRY_H

#include "hashfile.h"
#include "grafo.h"
#include <stdio.h>

/** Módulo responsável pelo processamento de comandos de consulta e atualização.
 *  Os comandos são lidos de um arquivo .qry e aplicados sobre os dados da cidade.
 *  As saídas podem ser geradas em arquivos texto (.txt) e SVG.
 */


 /** Processa o arquivo .qry executando os comandos sobre o sistema
 * @param qryPath Caminho do arquivo .qry
 * @param pessoasHash hashfile contendo os habitantes da cidade
 * @param quadrasHash hashfile contendo as quadras da cidade
 * @param txt Arquivo de saída para relatórios em texto
 * @param svg Arquivo de saída para visualização gráfica
 * @param grafo Grafo do sistema viário da cidade
 * COMANDOS PARA UTILIZAR
 * - rq = Remove quadra de acordo com o cep
 * - pq = Calcula o número de moradores da quadra (por face e total).
 * - censo = Produz várias estatísticas sobre habitantes de Bitnópolis.
 * - h? = Dados sobre habitante identificado por cpf. 
 * - nasc = Pessoa nasce.
 * - rip = Pessoa falece.
 * - mud = Morador identificado por cpf muda-se para novo endereço.
 * - dspj = Morador identificado por cpf é despejado.
 * - @o? = Armazena a posição geográfica do endereço no registrador
 * - mvm = Atualiza velocidade média de arestas em uma região
 * - regs = Calcula componentes conexos considerando trechos lentos.
 * - exp = Seleciona trechos para expansão viária de arestas com velocidade inferior a v1.
 * - p? = Calcula percurso mais curto e mais rápido entre dois registradores.
 */
void processQry( const char* qryPath, HashFile* pessoasHash, HashFile* quadrasHash, FILE* txt, FILE* svg, Grafo* grafo);

#endif