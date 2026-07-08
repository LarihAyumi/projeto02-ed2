#ifndef GRAFO_H
#define GRAFO_H

#include <stdio.h>

#define GRAFO_ID_MAX 64
#define GRAFO_CRITERIO_CURTO 0
#define GRAFO_CRITERIO_RAPIDO 1

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


/**
 * Calcula a quantidade de componentes conexos do grafo considerando apenas arestas com velocidade media suficiente.
 * Uma aresta e considerada valida quando sua velocidade media é maior ou igual ao limite informado.
 * @param grafo Grafo analisado
 * @param vl Velocidade limite
 * @return Quantidade de componentes conexos encontrados
 */
int calcularComponentesConexos(Grafo* grafo, double vl);

/**
 * Calcula os componentes conexos e seus respectivos retângulos envolventes.
 * Os vetores minX, minY, maxX e maxY devem possuir espaço suficiente para armazenar os dados dos componentes encontrados. A quantidade máxima permitida é informada por maxComp.
 * @param grafo Grafo analisado
 * @param vl Velocidade limite
 * @param minX Vetor com menor x de cada componente
 * @param minY Vetor com menor y de cada componente
 * @param maxX Vetor com maior x de cada componente
 * @param maxY Vetor com maior y de cada componente
 * @param maxComp Quantidade maxima de componentes armazenaveis
 * @return Quantidade de componentes conexos encontrados
 */
int calcularComponentesConexosBBox(Grafo* grafo, double vl, double* minX, double* minY, double* maxX, double* maxY, int maxComp);

/**
 * Aplica a expansão viária no grafo.
 * A função calcula uma árvore geradora mínima considerando o comprimento das arestas. Depois, dentre as arestas selecionadas pela árvore, aplica a expansão apenas nas que possuem velocidade média menor que vl
 * Cada aresta expandida tem sua velocidade média aumentada em 50%.
 * @param grafo Grafo que vai ser analisado e atualizado
 * @param vl Velocidade limite
 * @return Quantidade de arestas expandidas
 */
int aplicarExpansaoViaria(Grafo* grafo, double vl);

/**
 * Desenha no SVG as arestas que foram selecionadas pela ultima expansão viária.
 * As arestas sao desenhadas com linha grossa e vermelha, conforme o comando exp.
 * @param grafo Grafo analisado
 * @param svg Arquivo SVG de saida
 */
void desenharExpansaoViariaSvg(Grafo* grafo, FILE* svg);

/**
 * Verifica se uma aresta foi selecionada pela ultima expansão viária.
 * Função auxiliar usada principalmente em testes.
 * @param grafo Grafo analisado
 * @param origem Id do vértice de origem
 * @param destino Id do vértice de destino
 * @return 1 se a aresta foi selecionada, 0 caso contrario
 */
int arestaFoiExpandida(Grafo* grafo, const char* origem, const char* destino);

#endif