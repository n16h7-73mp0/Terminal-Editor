#ifndef EDITOR_H
#define EDITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilidades.h"
#include "arquivos.h"

//Modo do editor
typedef enum { COMANDO, INSERCAO, SOBRESCRICAO} MODO;

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

typedef struct {
	DESCRITOR_ARQUIVO arquivo;
	size_t linhas;
	size_t colunas;
	size_t linha_atual;
	size_t coluna_atual;
} PROPRIEDADES_EDITOR;

//Basicamente faz nada
void cria_string(CARACTERE** cabeca);
void adiciona_caractere(CARACTERE** cabeca, char chr);
void concatena_linha_anterior(LINHA** cabeca, size_t linha);
void remove_caractere(LINHA** cabeca, size_t linha, size_t coluna);
void insere_linha(LINHA** cabeca_linha, CARACTERE* cabeca_str);
void insere_caractere_meio(LINHA** caebca, size_t linha, size_t coluna, char chr);
CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer, const char* fnome, DESCRITOR_ARQUIVO* desc_arq);
void salva_buffer_arquivo(LINHA** buffer, const char* fnome);
void percorre_modo(MODO* modo);
int ultima_linha(LINHA* cabeca, size_t lihna);

extern MODO modo;
extern PROPRIEDADES_EDITOR editor_prop;
#endif