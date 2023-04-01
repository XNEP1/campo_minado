/* 
 * Biblioteca para pilhas.
 * De Bruno Krügel. GRR: GRR20206874
 * Para compilar use o padrão C99.
*/

#ifndef PILHA_H
#define PILHA_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define TAM_MAX_PILHA 1024

typedef struct coordenada{
        int x;
        int y;
} type_coordenada;

typedef struct pilha{
        int ini; 			/* ini é o indice onde a pilha começa.  */
        type_coordenada v[TAM_MAX_PILHA];    /* v é o vetor de structs da pilha.  */
} type_pilha; 

void inicializar_pilha(type_pilha *P);
/* Define o valor ini da pilha como 0.  */

bool pilha_vazia(type_pilha *P);
/* Retorna true se a pilha estiver vazia ou false caso contrário. */

bool pilha_unitaria(type_pilha *P);
/* Retorna true se a pilha tiver um único elemento. */

void empilhar(struct coordenada x, type_pilha *P);
/* Insere x no inicio da pilha. */

struct coordenada desempilhar(type_pilha *P);
/* Retorna o elemento do inicio da pilha e o remove dela. */

struct coordenada topo(type_pilha *P);
/* Retorna o elemento do inicio da pilha, mas não o remove dela. */

#endif
