#include "arquivos.h"

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(const char* nome_arq, const char* modo, FILE** arquivo){
	*arquivo = fopen(nome_arq, modo);
	if(*arquivo)
		return SUCESSO;
	else
		return ERRO;
}

//Retorna o tamanho de um arquivo em bytes
size_t tamanho_arquivo(FILE* arq)
{
	size_t bytes = 0;
	fseek(arq, 0, SEEK_END);
	bytes = ftell(arq);
	return bytes;
}