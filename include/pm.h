#ifndef PM_H
#define PM_H

#include "hashfile.h"
/** Módulo responsável pelo processamento do arquivo .pm.
 *  O arquivo .pm contém informações sobre habitantes e suas moradias.
 *  Os dados são inseridos ou atualizados no hashfile de pessoas.
 */


/** Processa arquivo .pm contendo pessoas e moradores
 * @param pmPath Caminho do arquivo .pm
 * @param pessoasHash Hashfile onde as pessoas serão armazenadas
 */
void processPM(const char* pmPath, HashFile* pessoasHash);

#endif