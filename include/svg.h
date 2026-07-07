#ifndef SVG_H
#define SVG_H

#include <stdio.h>
/** Módulo responsável pela geração do arquivo SVG
 *  Contém funções para inicialização, finalização e desenho de elementos gráficos.
 */


/** Inicia o arquivo SVG
 * @param svg Ponteiro para o arquivo de saída
 */
void startSVG(FILE* svg);


/** Finaliza o arquivo SVG
 * @param svg Ponteiro para o arquivo de saída
 */
void endSVG(FILE* svg);


/** Desenha uma quadra no SVG
 * @param svg Ponteiro para o arquivo de saída
 * @param cep Identificador da quadra
 * @param x Coordenada x da âncora
 * @param y Coordenada y da âncora
 * @param w Largura da quadra
 * @param h Altura da quadra
 * @param cfill Cor de preenchimento
 * @param cstrk Cor da borda
 * @param sw Espessura da borda
 */
void drawQuadra(FILE* svg, const char* cep, double x, double y, double w, double h, const char* cfill, const char* cstrk, const char* sw);


//FUNÇÕES PARA OS COMANDOS QRY

/** Desenha a X vermelho na âncora do comando RQ
 * @param svg Ponteiro para o arquivo de saída
 * @param x Coordenada x do desenho
 * @param y Coordenada y do desenho
 */
void drawXVermelho(FILE* svg, double x, double y);

/** Imprime texto no SVG, para o comando PQ e afins
 * @param svg Ponteiro para o arquivo de saída
 * @param x Coordenada x do texto
 * @param y Coordenada y do texto
 * @param texto Texto que será escrito no SVG
 */
void drawTextoSVG(FILE* svg, double x, double y, const char* texto);

/** Desenha a cruz vermelha do comando RIP
 * @param svg Ponteiro para o arquivo de saída
 * @param x Coordenada x do desenho
 * @param y Coordenada y do desenho
 */
void drawCruzVermelha(FILE* svg, double x, double y);

/** Desenha um quadrado com o cpf dentro para marcar destino da mudança do comando Mud
 * @param svg Ponteiro para o arquivo de saída
 * @param x Coordenada x do desenho
 * @param y Coordenada y do desenho
 * @param cpf passa o cpf que ficará dentro do quadrado
 */
void drawQuadradoCpf(FILE* svg, double x, double y, const char* cpf);

/** Desenha um circulo preto no local de despejo do comando DSPJ
 * @param svg Ponteiro para o arquivo de saída
 * @param x Coordenada x do desenho
 * @param y Coordenada y do desenho
 */
void drawCirculoPreto(FILE* svg, double x, double y);


#endif