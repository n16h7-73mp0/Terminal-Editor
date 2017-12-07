#include "editor.h"

MODO modo;
PROPRIEDADES_EDITOR editor_prop;

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
		CARACTERE* lixo = iterador_linha->string;
		iterador_linha->string->proximo->anterior = NULL;
		iterador_linha->string = iterador_linha->string->proximo;
		free(lixo);
		return;
	}

	CARACTERE* iterador_coluna = iterador_linha->string;
	

	while(iterador_coluna)
	{
		if(coluna == coluna_atual)
		{
			if(iterador_coluna->anterior)
				iterador_coluna->anterior->proximo = iterador_coluna->proximo;
			
			if(iterador_coluna->proximo)
				iterador_coluna->proximo->anterior = iterador_coluna->anterior;

			free(iterador_coluna);
			break;
		}

		coluna_atual++;
		iterador_coluna = iterador_coluna->proximo;
	}
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


CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer, FILE* arquivo)
{
	fseek(arquivo, 0, SEEK_SET);
	char chr;
	CARACTERE* c;
	cria_string(&c);
	
	editor_prop.linhas = 1;
	editor_prop.colunas = 1;

	while((chr = getc(arquivo)) != EOF)
	{
		if(chr == '\n')
		{
			editor_prop.linhas++;
			/* Se o caractere for a nova linha, adiciona a string já lida, como linha e parte para outra */
			insere_linha(buffer, c);
			cria_string(&c);
		}else
		{
			adiciona_caractere(&c, chr);
		}
	}

	//Insere a ultima linha lida
	insere_linha(buffer, c);

	//Faz nada
	return ERRO;
}

void percorre_modo(MODO* modo)
{
	switch(*modo)
	{
		case COMANDO:
			*modo = INSERCAO;
		break;

		case INSERCAO:
			*modo = SOBRESCRICAO;
		break;

		case SOBRESCRICAO:
			*modo = COMANDO;
		break;
	}
}