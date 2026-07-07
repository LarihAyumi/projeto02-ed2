#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>

/**
 * Módulo responsável pela representação do sistema viário da cidade.
 * O sistema viário é armazenado como um grafo direcionado usando listas de adjacencia.
 * Os vértices representam pontos do mapa e as arestas representam segmentos de ruas.
 */

/**
 * Tipo opaco que representa o grafo
 */
typedef struct Grafo Grafo;

/**
 * Cria um grafo vazio.
 * @return Ponteiro para o grafo criado
 */
Grafo* criarGrafo(void);

/**
 * Libera toda a memória usada pelo grafo
 * @param grafo Grafo que sera liberado
 */
void destruirGrafo(Grafo* grafo);

/**
 * Insere um vértice no grafo
 * @param grafo Grafo onde o vértice sera inserido
 * @param id Identificador do vértice
 * @param x Coordenada x do vértice
 * @param y Coordenada y do vértice
 */
void inserirVertice(Grafo* grafo, const char* id, double x, double y);

/**
 * Insere uma aresta direcionada no grafo
 * @param grafo Grafo onde a aresta será inserida
 * @param origem Identificador do vértice de origem
 * @param destino Identificador do vértice de destino
 * @param ldir CEP da quadra a direita da aresta
 * @param lesq CEP da quadra a esquerda da aresta
 * @param cmp Comprimento do segmento de rua
 * @param vm Velocidade media do segmento de rua
 * @param nome Nome da rua
 */
void inserirAresta(Grafo* grafo, const char* origem, const char* destino, const char* ldir, const char* lesq, double cmp, double vm, const char* nome);

/**
 * Desenha as arestas do grafo em um arquivo SVG ja aberto
 * @param grafo Grafo que vai ser desenhado
 * @param svg Arquivo SVG ja aberto para escrita
 */
void desenharGrafoSvg(Grafo* grafo, FILE* svg);

/**
 * Função auxiliar para contar vértices do grafo
 * @param grafo Grafo que vai ser contado
 */
int contarVerticesGrafo(Grafo* grafo);

/**
 * Função auxiliar para contar arestas do grafo
 * @param grafo Grafo que vai ser contado
 */
int contarArestasGrafo(Grafo* grafo);

/**
 * Atualiza a velocidade média das arestas dentro de uma região
 * Uma aresta é considerada dentro da região quando origem e destino estão dentro do retângulo.
 * @param grafo Grafo que será atualizado
 * @param vmNova Nova velocidade média
 * @param x Coordenada x da região
 * @param y Coordenada y da região
 * @param w Largura da região
 * @param h Altura da região
 * @return Quantidade de arestas atualizadas
 */
int atualizarVelocidadeRegiao(Grafo* grafo, double vmNova, double x, double y, double w, double h);

/**
 * Retorna a velocidade média de uma aresta.
 * Função auxiliar usada principalmente em testes.
 * @param grafo Grafo consultado
 * @param origem Id do vertice de origem
 * @param destino Id do vertice de destino
 * @return Velocidade media da aresta, ou -1.0 caso nao exista
 */
double obterVelocidadeAresta(Grafo* grafo, const char* origem, const char* destino);

#endif