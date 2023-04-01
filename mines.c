/*
 * Jogo do campo minado.
 * De Bruno Krügel. GRR: GRR20206874
 * Para compilar use o padrão C99.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "pilha.h"
#include "mines.h"

#define TAMANHO 9 
/* Tamanho MAXIMO que um lado do campo retangular pode ter. */

#define BORDA -2
#define MINA -1
#define NADA 0
#define VITORIA 1
#define DERROTA 2
#define EM_ANDAMENTO 3

const char SPRITE_MINA[] = "💣";
const char SPRITE_NADA[] = ". ";
const char SPRITE_ESCONDIDO[] = "■ ";

struct quadrado {
	int info;	/* Informa o numero de minas vizinahs desse quadrado. Ou -1, caso tenha uma mina nesse quadrado. */
	bool revelado;  /* Quando true imprime o conteudo info na tela. */
};

typedef struct campo_minado {
	int x,y; 				      /* Numero de linhas e colunas. */
	int total_minas; 			      /* Numero total de minas no campo. */
	int falta_abrir; 		  	      /* Numero de quadrados que faltam abrir. */
	int status; 			  	      /* Indica se o jogador ganhou ou perdeu o jogo. */
	struct quadrado matriz[TAMANHO+2][TAMANHO+2]; /* Informações de cada quadrado do campo. O campo jogavel são apenas nos indices 1..TAMANHO. */
} campo_minado;

void inicializar_campo(campo_minado *c){
	printf("\e[1;1H\e[2J"); /* 'Limpa' a tela em ANSI. */
	do{
		printf("++Defina o número de linhas(entre 1 e %d): ", TAMANHO);
		scanf("%d", &c->x);
		printf("\n");
	} while (c->x <1 || c->x >TAMANHO);
	
	do{
		printf("++Defina o número de colunas(entre 1 e %d): ", TAMANHO);
        	scanf("%d", &c->y);
       		printf("\n");
	} while (c->y <1 || c->y >TAMANHO);

	do{
		printf("++Defina o número de minas(entre 1 e %d): ", (c->x * c->y));
		scanf("%d", &c->total_minas);
		printf("\n");
	} while (c->total_minas <1 || c->total_minas >(c->x * c->y));

	c->falta_abrir = ((c->x * c->y) - c->total_minas);
	c->status = EM_ANDAMENTO; /* status = 3 */
	zerar_campo(c);
	gerar_minas(c);
	contar_vizinhos_com_mina(c);
}

bool eh_borda(campo_minado *c, int i, int j){
	return ( (i == 0) || (i == c->x+1) || (j == 0) || (j == c->y+1) );
}

void zerar_campo(campo_minado *c){
	for(int i=0; i<=(c->x+1); i++){
		for(int j=0; j<=(c->y+1); j++){
			if(eh_borda(c, i, j))
				c->matriz[i][j].info = BORDA;
			else
				c->matriz[i][j].info = NADA;
			c->matriz[i][j].revelado = false;
		}
	}

}

void gerar_minas(campo_minado *c){
	int x,y;
	srand(time(NULL));

	for(int i=0; i<c->total_minas; i++){
		do{
			x = (rand() % c->x) + 1;
			y = (rand() % c->y) + 1;
		} while (!(c->matriz[x][y].info == NADA));
		c->matriz[x][y].info = MINA;
	}
}

void contar_vizinhos_com_mina(campo_minado *c){
	for(int i=1; i<=c->x; i++)
		for(int j=1; j<=c->y; j++)
			if(c->matriz[i][j].info == NADA)
				c->matriz[i][j].info = num_vizinhos_com_mina(c, i, j);
}

int num_vizinhos_com_mina(campo_minado *c, int i, int j){
	int cont = 0;
	for(int x=(i-1); x<=(i+1); x++)
		for(int y=(j-1); y<=(j+1); y++)
			if(c->matriz[x][y].info == MINA)
				cont++;
	return cont;
}

void ler_jogada(campo_minado *c, int *x, int *y){
	do {
		printf("Escolha a linha para jogar: ");
		scanf("%d", x);
		printf("\n");

		printf("Escolha a coluna para jogar: ");
		scanf("%d", y);
		printf("\n");
	} while (c->matriz[*x][*y].revelado);
}

void imprimir_campo(campo_minado c){
	printf("\e[1;1H\e[2J"); /* 'Limpa' a tela em ANSI. */
	printf("   ");
	for(int i=1; i<=c.x; i++)
		printf("%d ",i);
	printf("\n");
	printf(" +");
	for(int i=1; i<=(2*c.x); i++)
		printf("-");
	printf("\n");
	for(int i=1; i<=c.x; i++){
		printf("%d| ", i);
		for(int j=1; j<=c.y; j++){
			if(!(c.matriz[i][j].revelado))
				printf("%s", SPRITE_ESCONDIDO);
			else
				switch(c.matriz[i][j].info){
					case MINA:
						printf("%s", SPRITE_MINA);
						break;
					case NADA:
						printf("%s", SPRITE_NADA);
						break;
					default:
						printf("%d ", c.matriz[i][j].info);
				}
		}
		printf("\n");
	}

}

void executar_jogada(campo_minado *c, int x, int y){
	int cont;
	type_coordenada coord;
	type_pilha p;
	type_pilha *pnt_p = &p;
	
	if(c->matriz[x][y].info == MINA){ /* Selecionou em uma mina. */
		c->status = DERROTA;
		revelar_tudo(c);
	}
	else{ /* Não selecionou um quadrado com mina. */
		cont = 1;
		c->matriz[x][y].revelado = true;
		if(c->matriz[x][y].info == NADA){ /* Esse quadrado não tem minas adjacentes. */
			coord.x = x;
			coord.y = y;
			inicializar_pilha(pnt_p);
			empilhar(coord, pnt_p);
			cont = abrir_vizinhos_sem_mina(c, pnt_p); /* Quantidade de blocos que foram revelados. */	
		}
		c->falta_abrir = c->falta_abrir - cont;
		if(c->falta_abrir == 0){ /* Ganhou */
			c->status = VITORIA;
			revelar_tudo(c);
		}
	}
}

void revelar_tudo(campo_minado *c){
/* Revela todos os blocos, usado quando o jogador perde. */
	for(int i=1; i<=c->x; i++)
		for(int j=1; j<=c->y; j++)
			c->matriz[i][j].revelado = true;
}

int abrir_vizinhos_sem_mina(campo_minado *c, type_pilha *pnt_p){
/* Revelar uma grande área onde não há nenhuma mina por perto, só para de revelar quando o valor do quadrado for >0. Retorna quantos blocos foram revelados. */

/* Notei que no campo minado mostrado no livro de alg1, só são revelados os quadrados que valem NADA. Então para deixar mais similar ao jogo do campo minado que conheço,
 * fiz com que os blocos que valem algo além de NADA também fossem revelados, mas esses não entram na pilha. ;) */
	type_coordenada quadrado, vizinho;
	int cont = 1;
	while(!(pilha_vazia(pnt_p))){ /* Enquanto tiver elementos na pilha, faça... */
		quadrado = desempilhar(pnt_p);
		for(int i=(quadrado.x - 1); i<=(quadrado.x + 1); i++)
			for(int j=(quadrado.y - 1); j<=(quadrado.y + 1); j++){
				if( !(c->matriz[i][j].revelado) && !(eh_borda(c, i, j)) ){
					if(c->matriz[i][j].info == NADA){
						vizinho.x = i;
						vizinho.y = j;
						empilhar(vizinho, pnt_p);
					}
					c->matriz[i][j].revelado = true;
					cont++;
				}	
			}
	}
	return cont;  /* Numero de blocos revelados. */
}

int main(void){
	campo_minado c;
	int x,y; /* Posições da jogada do jogador. */

	inicializar_campo(&c);
	imprimir_campo(c);
	do{
		ler_jogada(&c,&x,&y);
		executar_jogada(&c,x,y);
		imprimir_campo(c);
	} while (c.status == EM_ANDAMENTO);
	
	if(c.status == DERROTA)
		printf("Você explodiu! BBOOMMM!! X╭╮X \n");
	else /* c.status == GANHOU */
		printf("YEEI, você sobreviveu! ^u^ \n");

	return 0;
}
