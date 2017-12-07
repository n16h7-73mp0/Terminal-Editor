#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdlib.h>
#include <stdio.h>
#include "utilidades.h"

typedef struct descritor_arquivo
{
	char nome[255];
	size_t bytes;
} DESCRITOR_ARQUIVO;

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(const char* nome_arq, const char* modo, FILE** arquivo);
size_t tamanho_arquivo(FILE* arq);

#endif