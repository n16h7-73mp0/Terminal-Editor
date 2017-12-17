#include "arquivos.h"

char* nome_arquivo;

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(FILE** arquivo){
	*arquivo = fopen(nome_arquivo, "a+");
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
