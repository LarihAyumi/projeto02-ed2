#include "../include/qry.h"
#include "../include/pessoa.h"
#include "../include/quadra.h"
#include "../include/hashfile.h"
#include "../include/svg.h"
#include "../include/grafo.h"
#include <string.h>
#include <stdlib.h>


//structs
typedef struct {
    char cep[20];
    HashFile* pessoasHash;
    FILE* txt;
} RqContext;

typedef struct {
    char cep[20];
    int N, S, L, O;
} PqContext;

typedef struct {
    int habitantes;
    int moradores;
    int homens;
    int mulheres;
    int moradoresH;
    int moradoras;
    int semTetos;
    int mendigos;
    int mendigas;
} CensoContext;

typedef struct {
    int usado;
    double x;
    double y;
    char cep[20];
    char face;
    int num;
} RegGeo;

//funcs de callbacks
static void rqCallback(const char* key, void* record, void* extra) {
    (void) key;

    RqContext* ctx = (RqContext*) extra;
    Pessoa* p = (Pessoa*) record;

    if (pessoaTemMoradia(p) && strcmp(getCepMoradia(p), ctx->cep) == 0) {
        fprintf(ctx->txt, "%s %s perdeu moradia\n", getNome(p), getSobrenome(p));
        removeMoradia(p);
        insertRegister(ctx->pessoasHash, getCpf(p), p);
    }
}

static void pqCallback(const char* key, void* record, void* extra) {
    (void) key;

    PqContext* ctx = (PqContext*) extra;
    Pessoa* p = (Pessoa*) record;

    if (pessoaTemMoradia(p) && strcmp(getCepMoradia(p), ctx->cep) == 0) {
        char face = getFaceMoradia(p);

        if (face == 'N') ctx->N++;
        else if (face == 'S') ctx->S++;
        else if (face == 'L') ctx->L++;
        else if (face == 'O') ctx->O++;
    }
}

static void censoCallback(const char* key, void* record, void* extra) {
    (void) key;

    CensoContext* ctx = (CensoContext*) extra;
    Pessoa* p = (Pessoa*) record;

    ctx->habitantes++;

    if (getSexo(p) == 'M') {
        ctx->homens++;
    } 
    else if (getSexo(p) == 'F') {
        ctx->mulheres++;
    }

    if (pessoaTemMoradia(p)) {
        ctx->moradores++;

        if (getSexo(p) == 'M') {
            ctx->moradoresH++;
        } 
        else if (getSexo(p) == 'F') {
            ctx->moradoras++;
        }
    } else {
        ctx->semTetos++;

        if (getSexo(p) == 'M') {
            ctx->mendigos++;
        } 
        else if (getSexo(p) == 'F') {
            ctx->mendigas++;
        }
    }
}

//função para os desenhos svgs
static int enderecoToXY(HashFile* quadrasHash, const char* cep, char face, int num, double* x, double* y) {
    Quadra* q = malloc(getQuadraSize());
    if (!q) return 0;

    if (searchRegister(quadrasHash, cep, q) != 0) {
        free(q);
        return 0;
    }

    double qx = getXQuadra(q);
    double qy = getYQuadra(q);
    double w  = getWQuadra(q);
    double h  = getHQuadra(q);

    switch (face) {
        case 'N': //pra baixo
            *x = qx + num;
            *y = qy + h;
            break;
        case 'S': //pra cima
            *x = qx + num;
            *y = qy;
            break;
        case 'L': //pra esquerda
            *x = qx;
            *y = qy + num;
            break;
        case 'O': //pra direita
            *x = qx + w;
            *y = qy + num;
            break;
        default:
            free(q);
            return 0;
    }
    free(q);
    return 1;
}

//função auxiliar de conversão 
static int regToIndex(const char* reg) {
    int idx;

    if (!reg || reg[0] != 'R') {
        return -1;
    }

    idx = atoi(reg + 1);

    if (idx < 0 || idx > 10) {
        return -1;
    }

    return idx;
}

//comandos
void processQry( const char* qryPath, HashFile* pessoasHash, HashFile* quadrasHash, FILE* txt, FILE* svg, Grafo* grafo) {
    FILE* qry = fopen(qryPath, "r");

    if (!qry) {
        printf("Erro ao abrir arquivo QRY: %s\n", qryPath);
        return;
    }

    char comando[10];
    RegGeo regs[11] = {0};

    while (fscanf(qry, "%s", comando) != EOF) {

        if (strcmp(comando, "rq") == 0) {
            char cep[20];
            fscanf(qry, "%s", cep);

            Quadra* q = malloc(getQuadraSize());

            if (q && searchRegister(quadrasHash, cep, q) == 0) {
                drawXVermelho(svg, getXQuadra(q), getYQuadra(q));
            }            

            free(q);

            removeRegister(quadrasHash, cep);
            RqContext ctx;

            strcpy(ctx.cep, cep);
            ctx.pessoasHash = pessoasHash;
            ctx.txt = txt;

            scanRegisters(pessoasHash, rqCallback, &ctx);
        }

        else if (strcmp(comando, "pq") == 0) {
            char cep[20];
            fscanf(qry, "%s", cep);

            PqContext ctx;
            strcpy( ctx.cep, cep);
            ctx.N = ctx.S = ctx.L = ctx.O = 0;

            scanRegisters(pessoasHash, pqCallback, &ctx);

            int total =ctx.N + ctx.S+ ctx.L + ctx.O;
            fprintf(txt,"CEP: %s\n", cep);
            fprintf(txt, "N: %d\n", ctx.N);
            fprintf(txt, "S: %d\n", ctx.S);
            fprintf(txt, "L: %d\n", ctx.L);
            fprintf(txt, "O: %d\n", ctx.O);
            fprintf(txt, "Total: %d\n\n", total);

            Quadra* q = malloc(getQuadraSize());

            if (q && searchRegister(quadrasHash, cep, q) == 0) {
                char texto[20];

                double x = getXQuadra(q);
                double y = getYQuadra(q);
                double w = getWQuadra(q);
                double h = getHQuadra(q);

                sprintf(texto, "%d", ctx.N);
                drawTextoSVG(svg, x+w/2, y+h+15, texto);

                sprintf(texto, "%d", ctx.S);
                drawTextoSVG(svg, x+w/2, y-5, texto);

                sprintf(texto, "%d", ctx.L);
                drawTextoSVG(svg, x-10, y+h/2, texto);

                sprintf(texto, "%d", ctx.O);
                drawTextoSVG(svg, x+w+10, y+h/2, texto);

                sprintf(texto, "%d", total);
                drawTextoSVG(svg, x+w/2, y+h/2, texto);
            }
            free(q);
        }

        else if (strcmp(comando, "censo") == 0) {
            CensoContext ctx = {0};
            scanRegisters(pessoasHash, censoCallback, &ctx);

            double propMoradores = ctx.habitantes > 0 ? (double) ctx.moradores / ctx.habitantes : 0.0;
            double percHomens = ctx.habitantes > 0 ? 100.0 * ctx.homens / ctx.habitantes : 0.0;
            double percMulheres = ctx.habitantes > 0 ? 100.0 * ctx.mulheres / ctx.habitantes : 0.0;
            double percMoradoresHomens = ctx.moradores > 0 ? 100.0 * ctx.moradoresH / ctx.moradores : 0.0;
            double percMoradoras = ctx.moradores > 0 ? 100.0 * ctx.moradoras / ctx.moradores : 0.0;
            double percMendigos = ctx.semTetos > 0 ? 100.0 * ctx.mendigos / ctx.semTetos : 0.0;
            double percMendigas = ctx.semTetos > 0 ? 100.0 * ctx.mendigas / ctx.semTetos : 0.0;

            fprintf(txt, "%d HABITANTES DE BITNÓPOLIS!\n\n", ctx.habitantes);
            fprintf(txt, "Moradores: %d\n", ctx.moradores);
            fprintf(txt, "Proporção moradores por habitantes: %.2lf\n", propMoradores);
            fprintf(txt, "Homens: %d\n", ctx.homens);
            fprintf(txt, "Mulheres: %d\n\n", ctx.mulheres);
            fprintf(txt, "%% de HABITANTES que são homens: %.2lf\n", percHomens);
            fprintf(txt, "%% de HABITANTES que são mulheres: %.2lf\n\n", percMulheres);
            fprintf(txt, "%% de MORADORES homens: %.2lf\n", percMoradoresHomens);
            fprintf(txt, "%% de MORADORAS mulheres: %.2lf\n\n", percMoradoras);
            fprintf(txt, "Sem-tetos: %d\n", ctx.semTetos);
            fprintf(txt, "%% de SEM-TETOS que são homens: %.2lf\n", percMendigos);
            fprintf(txt, "%% de SEM-TETOS que são mulheres: %.2lf\n\n", percMendigas);
        }

        else if (strcmp(comando, "h?") == 0) {
            char cpf[20];
            fscanf(qry, "%s", cpf);

            Pessoa* p = malloc(getPessoaSize());

            if (p && searchRegister( pessoasHash, cpf, p) == 0) {
                fprintf(txt, "Nome: %s %s\n", getNome(p), getSobrenome(p));
                fprintf(txt, "CPF: %s \n", getCpf(p));
                fprintf(txt, "Sexo: %c\n", getSexo(p));
                fprintf(txt, "Nascimento: %s\n", getNasc(p));
            }
            if (pessoaTemMoradia(p)) {
                fprintf(txt, "Endereço: %s/%c/%d/%s\n\n", getCepMoradia(p), getFaceMoradia(p), getNumMoradia(p), getCompMoradia(p));
            } else {
            fprintf(txt, "Sem-teto\n\n");
            }
            free(p);
        }

        else if (strcmp(comando, "nasc") == 0) {
            char cpf[20], nome[50], sobrenome[50], nasc[11];
            char sexo;

            fscanf(qry, "%s %s %s %c %s", cpf, nome, sobrenome, &sexo, nasc);
            Pessoa* p = createPessoa(cpf, nome, sobrenome, sexo, nasc);
            
            insertRegister(pessoasHash, cpf, p);
            
            fprintf(txt,"%s %s nasceu.\n\n", nome, sobrenome);

            free(p);
        }

        else if (strcmp(comando, "rip") == 0) {
            char cpf[20];

            fscanf(qry, "%s", cpf);

            Pessoa* p = malloc(getPessoaSize());

            if (p && searchRegister(pessoasHash, cpf, p) == 0) {
                fprintf(txt, "Descanse em paz\n");
                fprintf(txt, "Nome: %s %s\n CPF: %s\n", getNome(p), getSobrenome(p), getCpf(p));
                fprintf(txt, "Do gênero %c\n", getSexo(p));
                fprintf(txt, "Nascida em: %s\n", getNasc(p));

                if (pessoaTemMoradia(p)) {
                    double x, y;
                    fprintf(txt, "Endereço: %s/%c/%d/%s\n\n", getCepMoradia(p), getFaceMoradia(p), getNumMoradia(p), getCompMoradia(p));
                    if (enderecoToXY(quadrasHash, getCepMoradia(p), getFaceMoradia(p), getNumMoradia(p), &x, &y)) {
                        drawCruzVermelha(svg, x, y);
                    }
                    removeRegister(pessoasHash, cpf);
                } else{
                    fprintf(txt, "\n");
                }
                free(p);
            }
        }
        
        else if (strcmp(comando, "mud") == 0) {
            char cpf[20], cep[20], comp[50];
            char face;
            int num;

            fscanf(qry, "%s %s %c %d %s", cpf, cep, &face, &num, comp);
            Pessoa* p = malloc(getPessoaSize());

            if (p && searchRegister(pessoasHash, cpf, p) == 0) {
                double x, y;
                setMoradia(p, cep, face, num, comp);
                insertRegister(pessoasHash, cpf, p);
                fprintf(txt, "%s %s se mudou!\n\n", getNome(p), getSobrenome(p));

                if (enderecoToXY(quadrasHash, cep, face, num, &x, &y)) {
                    drawQuadradoCpf(svg, x, y, cpf);
                }
            }

            free(p);
        }

        else if (strcmp(comando, "dspj") == 0) {
            char cpf[20];

            fscanf(qry, "%s", cpf);
            Pessoa* p = malloc(getPessoaSize());

            if (p && searchRegister(pessoasHash, cpf, p) == 0) {
                fprintf(txt, "%s %s FOI DESPEJADA!\n", getNome(p), getSobrenome(p));
                fprintf(txt, "CPF: %s\n", getCpf(p));
                fprintf(txt, "Sexo: %c\n", getSexo(p));
                fprintf(txt, "Nascimento: %s\n", getNasc(p));

                if (pessoaTemMoradia(p)) {
                    double x, y;
                    fprintf(txt, "Endereço do despejo: %s/%c/%d/%s\n\n", getCepMoradia(p), getFaceMoradia(p), getNumMoradia(p), getCompMoradia(p));

                    if (enderecoToXY(quadrasHash, getCepMoradia(p), getFaceMoradia(p), getNumMoradia(p), &x, &y)) { 
                        drawCirculoPreto(svg, x, y);
                    }
                    removeMoradia(p);
                    insertRegister(pessoasHash, cpf, p);
                } else {
                    fprintf(txt, "\n");
                }
            }
            free(p);
        }

        else if (strcmp(comando, "@o?") == 0) {
            char reg[10], cep[20], face;
            int num, idx;
            double x, y;

            fscanf(qry, "%s %s %c %d", reg, cep, &face, &num);
            idx = regToIndex(reg);

            fprintf(txt, "@o? %s %s %c %d\n", reg, cep, face, num);

            if (idx == -1) {
                fprintf(txt, "Registrador invalido: %s\n\n", reg);
            } else if (!enderecoToXY(quadrasHash, cep, face, num, &x, &y)) {
                fprintf(txt, "Endereco nao encontrado: %s/%c/%d\n\n", cep, face, num);
            } else {
                regs[idx].usado = 1;
                regs[idx].x = x;
                regs[idx].y = y;
                strcpy(regs[idx].cep, cep);
                regs[idx].face = face;
                regs[idx].num = num;

                fprintf(txt, "%s: %.2lf %.2lf\n\n", reg, x, y);

                if (svg) {
                    fprintf(svg, "<line x1=\"%.2lf\" y1=\"0\" x2=\"%.2lf\" y2=\"%.2lf\" ""stroke=\"red\" stroke-width=\"1\" stroke-dasharray=\"5,5\" />\n", x, x, y);
                    fprintf(svg, "<text x=\"%.2lf\" y=\"12\" fill=\"red\" font-size=\"12\">%s</text>\n", x + 3, reg);
                }
            }
        }

        else if (strcmp(comando, "mvm") == 0) {
            double v, x, y, w, h;
            int qtd;

            fscanf(qry, "%lf %lf %lf %lf %lf", &v, &x, &y, &w, &h);
            fprintf(txt, "mvm %.2lf %.2lf %.2lf %.2lf %.2lf\n", v, x, y, w, h);

            if (grafo == NULL) {
                fprintf(txt, "O grafo não existe.\n\n");
            } else {
                qtd = atualizarVelocidadeRegiao(grafo, v, x, y, w, h);
                fprintf(txt, "Arestas atualizadas: %d\n\n", qtd);
            }
        }

        else if (strcmp(comando, "regs") == 0) {
            double vl, minX[1024], minY[1024], maxX[1024], maxY[1024];
            int qtd, i, desenhar;
            const int maxComp = 1024;

            const char* cores[] = {"red","blue","green","orange","purple","brown","cyan","magenta","gold","gray"};

            fscanf(qry, "%lf", &vl);
            fprintf(txt, "regs %.2lf\n", vl);

            if (grafo==NULL) {
                fprintf(txt, "O grafo não existe.\n\n");
            } else {
                qtd = calcularComponentesConexosBBox(grafo, vl, minX, minY, maxX, maxY, maxComp);
                fprintf(txt, "Componentes conexos: %d\n\n", qtd);

                if (svg) {
                    desenhar = qtd;
                    if (desenhar>maxComp) {
                        desenhar=maxComp;
                    }
                    for (i = 0; i < desenhar; i++) {
                        fprintf(svg, "<rect x=\"%.2lf\" y=\"%.2lf\" width=\"%.2lf\" height=\"%.2lf\" ""fill=\"%s\" fill-opacity=\"0.5\" stroke=\"%s\" stroke-width=\"2\" />\n", minX[i], minY[i], maxX[i]-minX[i],maxY[i]-minY[i],cores[i % 10],cores[i % 10]);
                    }
                }
            }
        }

        else if (strcmp(comando, "exp") == 0) {
            double vl;
            int qtd;

            fscanf(qry, "%lf", &vl);
            fprintf(txt, "exp %.2lf\n", vl);

            if (grafo == NULL) {
                fprintf(txt, "O grafo não existe.\n\n");
            } else {
                qtd = aplicarExpansaoViaria(grafo, vl);
                fprintf(txt, "Arestas expandidas: %d\n\n", qtd);

                if (svg != NULL) {
                    desenharExpansaoViariaSvg(grafo, svg);
                }
            }
        }

        else if (strcmp(comando, "p?") == 0) {
            char reg1[10], reg2[10];
            char cc[30], cr[30];
            char idOrigem[GRAFO_ID_MAX];
            char idDestino[GRAFO_ID_MAX];
            char caminhoCurto[1024][GRAFO_ID_MAX];
            char caminhoRapido[1024][GRAFO_ID_MAX];
            int idx1, idx2, qtdCurto, qtdRapido;
            double custoCurto = 0.0;
            double custoRapido = 0.0;

            fscanf(qry, "%s %s %s %s", reg1, reg2, cc, cr);
            fprintf(txt, "p? %s %s %s %s\n", reg1, reg2, cc, cr);
            idx1 = regToIndex(reg1);
            idx2 = regToIndex(reg2);

            if (grafo == NULL) {
                fprintf(txt, "O grafo não existe.\n\n");
            }
            else if (idx1 == -1 || idx2 == -1) {
                fprintf(txt, "Registrador invalido.\n\n");
            }
            else if (!regs[idx1].usado || !regs[idx2].usado) {
                fprintf(txt, "Registrador sem referencia geografica.\n\n");
            }
            else if (!obterVerticeMaisProximo(grafo, regs[idx1].x, regs[idx1].y, idOrigem, GRAFO_ID_MAX)||!obterVerticeMaisProximo(grafo, regs[idx2].x, regs[idx2].y, idDestino, GRAFO_ID_MAX)) {
                fprintf(txt, "Nao foi possivel encontrar vertices proximos.\n\n");
            }else {
                fprintf(txt, "Origem: %s -> vertice %s\n", reg1, idOrigem);
                fprintf(txt, "Destino: %s -> vertice %s\n", reg2, idDestino);

                qtdCurto = calcularCaminhoDijkstra(grafo,idOrigem,idDestino,GRAFO_CRITERIO_CURTO,caminhoCurto,1024,&custoCurto);
                qtdRapido = calcularCaminhoDijkstra(grafo,idOrigem,idDestino,GRAFO_CRITERIO_RAPIDO, caminhoRapido,1024,&custoRapido);

                if (qtdCurto <= 0) {
                    fprintf(txt, "Destino inacessivel pelo criterio de menor caminho.\n\n");
                } else {
                    fprintf(txt, "Custo do caminho mais curto: %.2lf\n", custoCurto);
                    escreverCaminhoTxt(grafo, txt, caminhoCurto, qtdCurto, "Caminho mais curto:");

                    if (svg != NULL) {
                        desenharCaminhoSvg(grafo, svg, caminhoCurto, qtdCurto, cc, "caminho_curto", 1);
                    }
                }

                if (qtdRapido <= 0) {
                    fprintf(txt, "Destino inacessivel pelo criterio de menor tempo.\n\n");
                } else {
                    fprintf(txt, "Custo do caminho mais rapido: %.2lf\n", custoRapido);
                    escreverCaminhoTxt(grafo, txt, caminhoRapido, qtdRapido, "Caminho mais rapido:");

                    if (svg != NULL) {
                        desenharCaminhoSvg(grafo, svg, caminhoRapido, qtdRapido, cr, "caminho_rapido", 1);
                    }
                }
            }
        }    
    }
fclose(qry);
}