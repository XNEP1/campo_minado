/* 
 * Biblioteca para pilhas.
 * De Bruno Krügel. GRR: GRR20206874
 * Para compilar use o padrão C99.
*/

#include "pilha.h"

void inicializar_pilha(type_pilha *P){
	P->ini = 0;
}

bool pilha_vazia(type_pilha *P){
	return (P->ini == 0);
}

bool pilha_unitaria(type_pilha *P){
	return (P->ini == 1);
}

void empilhar(struct coordenada x, type_pilha *P){
	if(P->ini >= TAM_MAX_PILHA){
		printf("A pilha está cheia! O tamanho máximo é de %d\n", TAM_MAX_PILHA);
		exit(1);
	}

	P->ini++;
	P->v[P->ini] = x;
}

struct coordenada desempilhar(type_pilha *P){
	if(P->ini != 0){
		P->ini--;
		return P->v[(P->ini)+1];
	}
	exit(1);
}

struct coordenada topo(type_pilha *P){
        if(P->ini != 0){
                return P->v[P->ini];
        }
	exit(1);
}
