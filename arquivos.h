#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "utilidades.h"

typedef struct descritor_arquivo
{
	char nome[255];
	size_t bytes;
} DESCRITOR_ARQUIVO;

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(FILE** arquivo);

size_t tamanho_arquivo(FILE* arq);

extern DESCRITOR_ARQUIVO desc_arq;

#endif