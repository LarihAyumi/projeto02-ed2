#ifndef VIA_H
#define VIA_H

#include "grafo.h"

/**
 * Módulo responsável pela leitura do arquivo .via.
 * O arquivo .via descreve vértices e arestas do sistema viário
 */

/**
 * Processa o arquivo .via e monta o grafo correspondente
 * @param viaPath Caminho do arquivo .via
 * @return Grafo carregado a partir do arquivo
 */
Grafo* processVia(const char* viaPath);

#endif