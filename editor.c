#include "editor.h"

//Declaração de variáveis
MODO modo;
int modificado;
char* nome_arquivo;

//Cria uma string vazia
void cria_string(String* cabeca)
{
	*cabeca = NULL; 
}

//Adiciona um caractere no final da string
void adiciona_caractere(String* cabeca, char chr)
{
	//Aloca memória para a estrutura
	Caractere c = (Caractere) malloc(sizeof(struct caractere));
	c->chr = chr;

	//Se a lista não estiver vazia
	if(*cabeca)
	{	
		//Vai até o último caractere
		IteradorString iterador = *cabeca;
		while(iterador->proximo != NULL)
			iterador = iterador->proximo;

		//Coloca o novo caractere no final da lista
		c->anterior = iterador;
		c->proximo = NULL;
		iterador->proximo = c;
	}
	//Senão adiciona o caractere no inicio da lista.
	else
	{
		c->anterior = NULL;
		c->proximo = NULL;
		*cabeca = c;
	}
}

void remove_caractere(Cabeca* cabeca, size_t linha, size_t coluna)
{
	//Quando um caractere é removido, o arquivo fica marcado com modificado
	modificado = 1;

	size_t linha_atual = 0;
	size_t coluna_atual = 0;

	IteradorLinha iterador_linha = *cabeca;
	//Vai até a linha 'linha'
	while(iterador_linha)
	{
		if(linha == linha_atual)
			break;
		linha_atual++;
		iterador_linha = iterador_linha->proximo;
	}

	if(coluna == 0)
	{
		//Se o usuário tentar apagar o que seria o último caractre, o programa deve concatenar a linha atual
		//com a linha anterior.
		IteradorString iterador_coluna = iterador_linha->anterior->string;
		//Caminha até o penultimo nó, antes do '\n'

		while(iterador_coluna->proximo->proximo)
			iterador_coluna = iterador_coluna->proximo;

		//Remove o '\n' da memória.
		free(iterador_coluna->proximo);
		//Reorganiza as linhas, se preparando para excluir a linha atual
		//O '\n' da string anterior é substituido pelo começo da string da linha atual
		
		iterador_coluna->proximo = iterador_linha->string;
		//O proximo da linha aterior é o proximo da linha atual
		//i.e. se existir uma linha anterior
		if(iterador_linha->anterior)
			iterador_linha->anterior->proximo = iterador_linha->proximo;
		//O anterior da proxima linha é o anterior da linha atual
		//i.e. se existir uma proxima linha.
		if(iterador_linha->proximo)
			iterador_linha->proximo->anterior = iterador_linha->anterior;
		//Tira a linha atual da memória
		free(iterador_linha);
		return;
	}

	//Se o caractere for o primeiro
	if(coluna == 1)
	{
		//Prepara para remover
		Caractere lixo = iterador_linha->string;
		
		//Se ele não for o unico caractere
		if(lixo->proximo)
		{
			lixo->proximo->anterior = NULL;
			iterador_linha->string = lixo->proximo;
		}
		//Tira o caractere da memória e sai da função
		free(lixo);
		return;
	}

	//Se não for nenhum dos casos específicos, procura e remove o caractere
	IteradorString iterador_coluna = iterador_linha->string;
	
	//Vai até o caractere a ser removido
	while(iterador_coluna)
	{
		//o caractere fica a ser apagado fica atras do cursor
		if(coluna - 1 == coluna_atual)
			break;
		coluna_atual++;
		iterador_coluna = iterador_coluna->proximo;
	}
	//Se houver algum atras dele
	if(iterador_coluna->anterior)
		iterador_coluna->anterior->proximo = iterador_coluna->proximo;
	//Se houver algum depois dele
	if(iterador_coluna->proximo)
		iterador_coluna->proximo->anterior = iterador_coluna->anterior;
	//Tira ele da memória
	free(iterador_coluna);
}

void insere_linha(Cabeca* cabeca_linha, String cabeca_str)
{
	//Aloca espaço na memória
	Cabeca l = (Cabeca) malloc(sizeof(struct linha));
	l->string = cabeca_str;

	//Se a lista não for vazia
	if(*cabeca_linha)
	{
		//Vai até a última linha
		IteradorLinha iterador = *cabeca_linha;
		while(iterador->proximo != NULL)
			iterador = iterador->proximo;

		//Adiciona a nova linha no final
		l->anterior = iterador;
		l->proximo = NULL;
		iterador->proximo = l;
	}
	else
	{
		//Senão, a lista é somente essa nova linha
		l->anterior = NULL;
		l->proximo = NULL;
		*cabeca_linha = l;
	}
}

void quebra_linha(Cabeca* cabeca, size_t linha, size_t coluna)
{
	//Se uma linha for quebrada o arquivo fica marcado como modificado
	modificado = 1;
	//Aloca memória para a nova linha
	Linha nova_linha = (Linha) malloc(sizeof(struct linha));
	//Vai até a linha 'linha'
	size_t linha_atual = 0;
	IteradorLinha iterador_linha = *cabeca;
	while(iterador_linha)
	{
		if(linha_atual == linha)
			break;
		iterador_linha = iterador_linha->proximo;
		linha_atual++;
	}

	//Vai até a coluna 'coluna'
	size_t coluna_atual = 0;
	IteradorString iterador_coluna = iterador_linha->string;
	while(iterador_coluna)
	{
		if(coluna_atual == coluna)
			break;
		iterador_coluna = iterador_coluna->proximo;
		coluna_atual++;
	}

	//Organiza a ordem das linhas
	//A nova linha se posiciona depois da linha onde o cursor está. i.e. o texto cresce para baixo.
	nova_linha->anterior = iterador_linha;
	nova_linha->proximo = iterador_linha->proximo;
	iterador_linha->proximo = nova_linha;

	//Ajusta a separação da linha, quebra a linha na posição do cursor e manda o texto a
	//frente dele para a próxima linha, o texto antes do cursor fica na linha atual.
	if(iterador_coluna->proximo)
	{
		iterador_coluna->anterior->proximo = NULL;
		iterador_coluna->anterior = NULL;
		adiciona_caractere(&(iterador_linha->string), '\n');
		nova_linha->string = iterador_coluna;
	}else
	{
		nova_linha->string = (String)malloc(sizeof(struct caractere));
		nova_linha->string->chr = '\n';
		nova_linha->string->anterior = NULL;
		nova_linha->string->proximo = NULL;
	}
}

void insere_caractere_meio(Cabeca* cabeca, size_t linha, size_t coluna, char chr)
{
	//Se um caractere for inserido o arquivo fica marcado como modificado
	modificado = 1;

	Caractere c = (Caractere)malloc(sizeof(struct caractere));
	c->chr = chr;
	int linha_atual = 0;

	//Encontra a linha
	IteradorLinha iterador_linha = *cabeca;
	while(iterador_linha)
	{
		if(linha == linha_atual)
			break;
		iterador_linha = iterador_linha->proximo;
		linha_atual++;
	}

	//Se for adicionar no inicio da linha
	if(coluna == 0)
	{
		c->proximo = iterador_linha->string;
		iterador_linha->string->anterior = c;
		iterador_linha->string = c;
		return;
	}

	//Senão encontra a coluna
	IteradorString iterador_coluna = iterador_linha->string;
	int coluna_atual = 0;
	while(iterador_coluna)
	{
		//O caractere fica atrás do cursor
		if(coluna - 1 == coluna_atual)
			break;
		iterador_coluna = iterador_coluna->proximo;
		coluna_atual++;
	}
	//Organiza a ordem dos caracteres
	c->proximo = iterador_coluna->proximo;
	c->anterior = iterador_coluna;
	iterador_coluna->proximo = c;

	return;
}

CODIGO_RETORNO cria_buffer_arquivo(Cabeca* cabeca)
{
	FILE* arquivo;
	
	//Se abrir o arquivo com sucesso
	if(abrir_arquivo(&arquivo))
	{
		
		char chr;
		Caractere c;
		cria_string(&c);

		int nova_linha = 1;

		while((chr = getc(arquivo)) != EOF)
		{
			if(chr == '\n')
			{
				/* Se o caractere for a nova linha, adiciona a string já lida, 
				como linha e parte para outra */
				adiciona_caractere(&c, chr);
				insere_linha(cabeca, c);
				nova_linha = 1;
				c = NULL;
				continue;
			}
			
			//Se for uma nova linha
			if(nova_linha)
			{
				cria_string(&c);
				adiciona_caractere(&c, chr);
				nova_linha = 0;
			}else
			{
				adiciona_caractere(&c, chr);
			}
		}

		if(*cabeca == NULL)
		{
			//Significa que o arquivo ou estava completamente vazio, ou o arquivo não existia.
			//Cria um buffer vazio.
			*cabeca = (Linha)malloc(sizeof(struct linha));
			(*cabeca)->string = (Caractere)malloc(sizeof(struct caractere));
			(*cabeca)->anterior = NULL;
			(*cabeca)->proximo = NULL;
			(*cabeca)->string->anterior = NULL;
			(*cabeca)->string->proximo = NULL;
			(*cabeca)->string->chr = '\n';
		}

		fclose(arquivo);
		return SUCESSO;
	}
	return ERRO;
}

CODIGO_RETORNO salva_buffer_arquivo(Cabeca* cabeca)
{
	//Abre no modo de escrita sobrescrevendo todo o conteúdo antigo
	//com o novo
	FILE* arquivo = fopen(nome_arquivo, "w+");
	
	//Se abriu com sucesso
	if(arquivo)
	{
		//Itera linha por linha
		IteradorLinha iterador_linha = *cabeca;
		while(iterador_linha)
		{
			//Itera caractere por caractere
			IteradorString iterador_caractere = iterador_linha->string;
			while(iterador_caractere)
			{
				//Se o caractere não for NULL
				if(iterador_caractere->chr != '\0')
					//Imprime o caractere no arquivo
					fprintf(arquivo, "%c", iterador_caractere->chr);
				iterador_caractere = iterador_caractere->proximo;
			}
			iterador_linha = iterador_linha->proximo;
		}
	}else
		return ERRO;

	//Fecha o arquivo
	fclose(arquivo);
	return SUCESSO;
}


//Percorre os modos do editor
void percorre_modo(MODO* modo)
{
	//OCIOSO -> INSERCAO -> OCIOSO -> INSERCAO -> ...
	switch(*modo)
	{
		case OCIOSO:
			*modo = INSERCAO;
		break;

		case INSERCAO:
			*modo = OCIOSO;
		break;
	}
}

//Funções auxiliares para graficos

int ultima_linha(Cabeca cabeca, size_t linha)
{
	//Vai até a linha
	IteradorLinha iterador  = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador = iterador->proximo;
	}
	//Se ela não for a última
	if(iterador->proximo)
		return 0;
	//Se ela for a última
	return 1;
}

int ultimo_caractere(Cabeca cabeca, size_t linha, size_t coluna)
{
	//Vai até a linha
	IteradorLinha iterador_linha = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador_linha = iterador_linha->proximo;
	}

	//Vai até a coluna
	IteradorString iterador = iterador_linha->string;
	size_t coluna_atual = 0;
	while(coluna_atual != coluna)
	{
		coluna_atual++;
		iterador = iterador->proximo;
	}

	//Se ele não for o último
	if(iterador->proximo)
		return 0;
	//Se ele for o último
	return 1;
}

int tamanho_linha(Cabeca cabeca, size_t linha)
{
	//Vai até a linha
	IteradorLinha iterador_linha = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador_linha = iterador_linha->proximo;
	}
	//Vai contando caractere por carctere 
	IteradorString iterador = iterador_linha->string;
	size_t colunas = 0;
	while(iterador)
	{
		colunas++;
		iterador = iterador->proximo;
	}
	//Numero de carcteres = numero de caracteres - 1 quebra de linha
	return colunas - 1;
}