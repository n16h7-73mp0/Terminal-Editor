#include "editor.h"

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

	while((chr = getc(arquivo)) != EOF)
	{
		if(chr == '\n')
		{
			/* Se o caractere for a nova linha, adiciona a string já lida, como linha e parte para outra */
			insere_linha(buffer, c);
			cria_string(&c);
		}else
		{
			adiciona_caractere(&c, chr);
		}
	}

	//Faz nada
	return ERRO;
}