#ifndef EDITOR_H
#define EDITOR_H

#include <stdlib.h>
#include <stdio.h>
#include "utilidades.h"

//estrutura da lista que representa o texto de uma linha
typedef struct caractere
{
	char chr;
	struct caractere* anterior;
	struct caractere* proximo;
} CARACTERE;


//estrutura da lista que representa uma linha
typedef struct linha
{
	CARACTERE* string;
	struct linha* anterior;
	struct linha* proximo;
} LINHA;

//Basicamente faz nada
void cria_string(CARACTERE** cabeca);
void adiciona_caractere(CARACTERE** cabeca, char chr);
void insere_linha(LINHA** cabeca_linha, CARACTERE* cabeca_str);
CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer, FILE* arquivo);

#endif