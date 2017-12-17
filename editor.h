#ifndef EDITOR_H
#define EDITOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "utilidades.h"
#include "arquivos.h"

//Modo do editor
typedef enum { OCIOSO, INSERCAO} MODO;

//estrutura da lista que representa o texto de uma linha
struct caractere
{
	char chr;
	struct caractere* anterior;
	struct caractere* proximo;
};

//typedefs, somente para semântica
typedef struct caractere* String;
typedef struct caractere* Caractere;
typedef struct caractere* IteradorString;

//estrutura da lista que representa uma linha
struct linha
{
	String string;
	struct linha* anterior;
	struct linha* proximo;
};

//typedefs, somente para semântica
typedef struct linha* Cabeca;
typedef struct linha* Linha;
typedef struct linha* IteradorLinha;

//Cria uma string vazia
void cria_string(String* cabeca);
//Adiciona um caractere no final de uma string
void adiciona_caractere(String* cabeca, char chr);
//Rremove um caractere, na posicao linha:coluna
void remove_caractere(Cabeca* cabeca, size_t linha, size_t coluna);
//Insere uma linha no da lista
void insere_linha(Cabeca* cabeca_linha, String cabeca_str);
//Quebra uma linha em duas, na posicao linha:coluna
void quebra_linha(Cabeca* cabeca_linha, size_t linha, size_t coluna);
//Insere um caractere no texto na posicao linha:coluna
void insere_caractere_meio(Cabeca* caebca, size_t linha, size_t coluna, char chr);
//Joga o conteudo de um arquivo para uma lista
CODIGO_RETORNO cria_buffer_arquivo(Cabeca* cabeca);
//Salva a lista em um arquivo
CODIGO_RETORNO salva_buffer_arquivo(Cabeca* cabeca);
//Percorre sobre os modos
void percorre_modo(MODO* modo);
//1 se a linha 'linha' for a ultima linha, 0 se não
int ultima_linha(Cabeca cabeca, size_t lihna);
//1 se o caractere 'linha:coluna' for o ultimo caractere, 0 se não.
int ultimo_caractere(Cabeca cabeca, size_t linha,  size_t coluna);
//Retorna o tamanho, em caracteres da linha 'linha'
int tamanho_linha(Cabeca cabeca, size_t linha);

//Várivaies globais
extern MODO modo;
extern int modificado;
#endif