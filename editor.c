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
				iterador_string->proximo = iterador_atual->string;
				iterador_atual->string->anterior = iterador_string;
				iterador_anterior->proximo = iterador_atual->proximo;
				if(iterador_atual->proximo)
					iterador_atual->proximo = iterador_anterior;
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
		//PLACEHOLDER
		//remove_linha(cabeca, linha);
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


CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer, const char* fnome, DESCRITOR_ARQUIVO* desc_arq)
{
	strcpy(desc_arq->nome, fnome);
	FILE* arquivo;
	if(abrir_arquivo(fnome, "r", &arquivo))
	{
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

		return SUCESSO;
	}
	fclose(arquivo);
	//Faz nada
	return ERRO;
}

void salva_buffer_arquivo(LINHA** buffer, const char* fnome)
{
	FILE* arquivo = fopen(fnome, "w");
	
	if(arquivo)
	{
		LINHA* iterador_linha = *buffer;
		while(iterador_linha)
		{
			CARACTERE* iterador_caractere = iterador_linha->string;
			while(iterador_caractere)
			{
				fprintf(arquivo, "%c", iterador_caractere->chr);
				iterador_caractere = iterador_caractere->proximo;
			}
			fprintf(arquivo, "\n");
			iterador_linha = iterador_linha->proximo;
		}
	}

	fclose(arquivo);
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