#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "utilidades.h"

//Tenta abrir um arquivo se ele existe, e cria um se não.
CODIGO_RETORNO abrir_arquivo(FILE** arquivo);

//Váriavel global que armazena o nome do arquivo
extern char* nome_arquivo;

#endif