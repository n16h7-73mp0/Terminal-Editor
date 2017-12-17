#include "editor.h"

MODO modo;
PROPRIEDADES_EDITOR editor_prop;
DESCRITOR_ARQUIVO desc_arq;

//Basicamente faz nada
void cria_string(CARACTERE** cabeca)
{
	*cabeca = NULL; 
}

//Adiciona um caractere no final da string
void adiciona_caractere(CARACTERE** cabeca, char chr)
{
	CARACTERE* c = (CARACTERE*) malloc(sizeof(struct caractere));
	c->chr = chr;

	if(*cabeca)
	{	
		CARACTERE* iterador = *cabeca;

		while(iterador->proximo != NULL)
			iterador = iterador->proximo;

		c->anterior = iterador;
		c->proximo = NULL;
		iterador->proximo = c;
	}
	else
	{
		c->anterior = NULL;
		c->proximo = NULL;
		*cabeca = c;
	}
}

void concatena_linha_anterior(LINHA** cabeca, size_t linha)
{
	if(*cabeca != NULL)
	{
		if(linha == 0)
		{
			LINHA* lixo = *cabeca;
			if((*cabeca)->proximo)
				(*cabeca)->proximo->anterior = NULL;
			*cabeca = (*cabeca)->proximo;
			free(lixo);
			return;
		}

		LINHA* iterador_anterior = *cabeca;
		LINHA* iterador_atual = (*cabeca)->proximo;
		int linha_atual = 1;

		while(iterador_atual)
		{
			if(linha_atual == linha)
			{
				//Navega até o final da string da linha anterior.
				CARACTERE* iterador_string = iterador_anterior->string;
				while(iterador_string->proximo)
					iterador_string = iterador_string->proximo;
				
				//Define como próximo caractere da string
				//O começo da string inicial da linha atual
				iterador_string->proximo = iterador_atual->string;
				iterador_atual->string->anterior = iterador_string;

				//A proxima linha do anterior agora é a proxima linha do atual
				iterador_anterior->proximo = iterador_atual->proximo;

				//Se não for a última linha, define o anterior do proximo como o anterior do atual
				if(iterador_atual->proximo)
					iterador_atual->proximo->anterior = iterador_anterior;
				//Deleta a linha da memória
				free(iterador_atual);
				return;
			}

			iterador_anterior = iterador_atual;
			iterador_atual = iterador_atual->proximo;
			linha_atual++;
		}
	}
}

void remove_caractere(LINHA** cabeca, size_t linha, size_t coluna)
{
	size_t linha_atual = 0;
	size_t coluna_atual = 0;

	LINHA* iterador_linha = *cabeca;

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
		CARACTERE* iterador_coluna = iterador_linha->anterior->string;
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

	if(coluna == 1)
	{
		CARACTERE* lixo = iterador_linha->string;
		
		if(iterador_linha->string->proximo)
		{
			iterador_linha->string->proximo->anterior = NULL;
			iterador_linha->string = iterador_linha->string->proximo;
		}
		free(lixo);
		return;
	}

	CARACTERE* iterador_coluna = iterador_linha->string;
	

	while(iterador_coluna)
	{
		//O indice da letra a ser apagada, é INDICE_REAL + 1
		if(coluna - 1 == coluna_atual)
			break;
		coluna_atual++;
		iterador_coluna = iterador_coluna->proximo;
	}

	if(iterador_coluna->anterior)
		iterador_coluna->anterior->proximo = iterador_coluna->proximo;
			
	if(iterador_coluna->proximo)
		iterador_coluna->proximo->anterior = iterador_coluna->anterior;

	free(iterador_coluna);
}

void insere_linha(LINHA** cabeca_linha, CARACTERE* cabeca_str)
{
	LINHA* l = (LINHA*) malloc(sizeof(struct linha));
	l->string = cabeca_str;

	if(*cabeca_linha)
	{
		LINHA* iterador = *cabeca_linha;

		while(iterador->proximo != NULL)
			iterador = iterador->proximo;

		l->anterior = iterador;
		l->proximo = NULL;
		iterador->proximo = l;
	}
	else
	{
		l->anterior = NULL;
		l->proximo = NULL;
		*cabeca_linha = l;
	}
}

void quebra_linha(LINHA** cabeca, size_t linha, size_t coluna)
{
	LINHA* nova_linha = (LINHA*) malloc(sizeof(struct linha));
	size_t linha_atual = 0;
	LINHA* iterador_linha = *cabeca;

	while(iterador_linha)
	{
		if(linha_atual == linha)
			break;

		iterador_linha = iterador_linha->proximo;
		linha_atual++;
	}

	size_t coluna_atual = 0;
	CARACTERE* iterador_coluna = iterador_linha->string;
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
		mvprintw(31, 0, "ENTER, nao e o ultimo");
		iterador_coluna->anterior->proximo = NULL;
		iterador_coluna->anterior = NULL;
		adiciona_caractere(&(iterador_linha->string), '\n');
		nova_linha->string = iterador_coluna;
	}else
	{
		mvprintw(31, 0, "ENTER, é o ultimo");
		nova_linha->string = (CARACTERE*)malloc(sizeof(struct caractere));
		nova_linha->string->chr = '\n';
		nova_linha->string->anterior = NULL;
		nova_linha->string->proximo = NULL;
	}
	refresh();
}

void insere_caractere_meio(LINHA** cabeca, size_t linha, size_t coluna, char chr)
{
	CARACTERE* c = (CARACTERE*)malloc(sizeof(struct caractere));
	c->chr = chr;
	int linha_atual = 0;

	LINHA* iterador_linha = *cabeca;

	//Encontra a linha
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

	CARACTERE* iterador_coluna = iterador_linha->string;
	int coluna_atual = 0;
	
	//Encontra a coluna
	while(iterador_coluna)
	{
		if(coluna - 1 == coluna_atual)
			break;
		iterador_coluna = iterador_coluna->proximo;
		coluna_atual++;
	}

	c->proximo = iterador_coluna->proximo;
	c->anterior = iterador_coluna;
	iterador_coluna->proximo = c;

	return;
}

CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer)
{
	FILE* arquivo;
	
	if(abrir_arquivo(&arquivo))
	{
		char chr;
		CARACTERE* c;
		cria_string(&c);

		int nova_linha = 1;
		
		editor_prop.linhas = 0;

		while((chr = getc(arquivo)) != EOF)
		{
			if(chr == '\n')
			{
				/* Se o caractere for a nova linha, adiciona a string já lida, 
				como linha e parte para outra */
				adiciona_caractere(&c, chr);
				insere_linha(buffer, c);
				editor_prop.linhas++;
				nova_linha = 1;
				c = NULL;
				continue;
			}
			
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

		if(*buffer == NULL)
		{
			//Significa que o arquivo ou estava completamente vazio, ou o arquivo não existia.
			//Cria um buffer vazio.
			*buffer = (LINHA*)malloc(sizeof(struct linha));
			(*buffer)->string = (CARACTERE*)malloc(sizeof(struct caractere));
			(*buffer)->anterior = NULL;
			(*buffer)->proximo = NULL;
			(*buffer)->string->anterior = NULL;
			(*buffer)->string->proximo = NULL;
			(*buffer)->string->chr = '\n';
		}

		fclose(arquivo);
		return SUCESSO;
	}

	return ERRO;
}

CODIGO_RETORNO salva_buffer_arquivo(LINHA** buffer)
{
	//Abre no modo de escrita sobrescrevendo todo o conteúdo antigo
	//com o novo
	FILE* arquivo = fopen(desc_arq.nome, "w+");
	
	//Se abriu com sucesso
	if(arquivo)
	{
		//Itera linha por linha
		LINHA* iterador_linha = *buffer;
		while(iterador_linha)
		{
			//Itera caractere por caractere
			CARACTERE* iterador_caractere = iterador_linha->string;
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
int ultima_linha(LINHA* cabeca, size_t linha)
{
	LINHA* iterador  = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador = iterador->proximo;
	}

	if(iterador->proximo)
		return 0;
	return 1;
}

int ultimo_caractere(LINHA* cabeca, size_t linha, size_t coluna)
{
	LINHA* iterador_linha = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador_linha = iterador_linha->proximo;
	}

	CARACTERE* iterador = iterador_linha->string;
	size_t coluna_atual = 0;
	while(coluna_atual != coluna)
	{
		coluna_atual++;
		iterador = iterador->proximo;
	}

	if(iterador->proximo)
		return 0;
	return 1;
}

int tamanho_linha(LINHA* cabeca, size_t linha)
{
	LINHA* iterador_linha = cabeca;
	size_t linha_atual = 0;
	while(linha_atual != linha)
	{
		linha_atual++;
		iterador_linha = iterador_linha->proximo;
	}

	CARACTERE* iterador = iterador_linha->string;
	size_t colunas = 0;
	while(iterador)
	{
		colunas++;
		iterador = iterador->proximo;
	}
	return colunas - 1;
}