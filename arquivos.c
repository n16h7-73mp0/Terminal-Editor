#include "arquivos.h"

DESCRITOR_ARQUIVO desc_arq;

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(FILE** arquivo){
	*arquivo = fopen(desc_arq.nome, "a+");
	//Verifica se o arquivo foi aberto/criado com sucesso
	if(*arquivo)
	{
		fseek(*arquivo, SEEK_SET, 0);
		return SUCESSO;
	}
	//Mostra a mensagem de erro
	perror("ERRO");
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