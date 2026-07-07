#ifndef GEO_H
#define GEO_H
#include <stdio.h>
#include "hashfile.h"

/** Módulo responsável pelo processamento do arquivo .geo.
 *  O arquivo .geo descreve a cidade (quadras) e define parâmetros visuais.
 *  As quadras são armazenadas em um hashfile e também desenhadas em um SVG já aberto.
 */

/** Processa o arquivo .geo e desenha as quadras em SVG
 * @param geoPath Caminho do arquivo .geo
 * @param quadrasHash Hashfile para armazenar quadras
 * @param svg Arquivo SVG já aberto para escrita
 */
void processGeo(const char* geoPath, HashFile* quadrasHash, FILE* svg);



#endif