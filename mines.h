/* 
 * Header do jogo do campo minado. 
 * De Bruno Krügel. GRR: GRR20206874 
 * Para compilar use o padrão C99.  
*/

#ifndef MINES_H
#define MINES_H
#include <stdbool.h>

typedef struct campo_minado campo_minado;			/*  */

void inicializar_campo(campo_minado *c);			/* Garante que não há lixo de memória na matriz do campo, defenindo os valores corretos para cada quadrado. */

bool eh_borda(campo_minado *c, int i, int j);			/* Verifica se o quadrado de coordenada (i,j) está em uma posição que deve ter o valor de -2(BORDA). */

void zerar_campo(campo_minado *c);				/* Define todos os quadrados como 0(NADA) e ,também ,define como -2(BORDA) toda a borda da matriz. */

void gerar_minas(campo_minado *c);				/* Usa a função rand() do stdlib para gerar posições aleatórias onde terão bombas. */

void contar_vizinhos_com_mina(campo_minado *c);			/* Define o valor de cada quadrado com 0(NADA) de acordo com o valor dado pela função num_vizinhos_com_mina. */

int num_vizinhos_com_mina(campo_minado *c, int i, int j);	/* Lê todos os quadrados adjacentes de um quadrado na coordenada (i,j) e retorna quantos -1(MINA) ele achou. */

void ler_jogada(campo_minado *c, int *x, int *y);		/* Pergunta ao jogador qual coordenada ele escolhe para jogar. Só são válidos quadrados ainda não revelados. */

void imprimir_campo(campo_minado c);				/*  */

void executar_jogada(campo_minado *c, int x, int y);		/* Atualiza os dados na estrutura do campo para uma certa jogada. */

void revelar_tudo(campo_minado *c); 				/* Revela todos os blocos, usado quando o jogador perde. */

int abrir_vizinhos_sem_mina(campo_minado *c, type_pilha *pnt_p);/* Revelar uma grande área onde não há nenhuma mina por perto, só para de revelar quando o valor do quadrado for >0. Retorna quantos blocos foram revelados. */

#endif
