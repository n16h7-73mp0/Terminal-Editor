#include <stdlib.h>
#include <string.h>
#include <ncurses.h>


//estrutura da lista que representa o texto de uma linha
typedef struct caractere
{
	char chr;
	struct caractere* anterior;
	struct caractere* proximo;
} CARACTERE;


//estrutura da lista que representa uma linha
typedef struct linha
{
	CARACTERE* string;
	struct linha* anterior;
	struct linha* proximo;
} LINHA;

//Codigos de retorno de funcoes
typedef enum { ERRO, SUCESSO } CODIGO_RETORNO;

//Inicia o ncurses
void inicia_ncurses(void)
{
	initscr();				//Inicia o modo ncurses
	noecho();				//Não imprime as telcas
	cbreak();				//Desabilita o buffering de linhas
	keypad(stdscr, true);	//Habilita teclas especiais
}

//Tenta abrir um arquivo
CODIGO_RETORNO abrir_arquivo(const char* nome_arq, const char* modo, FILE** arquivo)
{
	*arquivo = fopen(nome_arq, modo);
	if(*arquivo)
		return SUCESSO;
	else
		return ERRO;
}

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

/* 
	Função de DEBUG 
	Imprime a string na tela
*/
void imprime_string(CARACTERE* cabeca)
{
	CARACTERE* iterador = cabeca;
	while(iterador)
	{
		addch(iterador->chr);
		iterador = iterador->proximo;
	}
	addch('\n');
}

void insere_linha(LINHA** cabeca_linha, CARACTERE* cabeca_str)
{
	LINHA* l = (LINHA*) malloc(sizeof(struct linha));
	l->string = cabeca_str;
	//imprime_string(cabeca_str);
	if(*cabeca_linha)
	{
		LINHA* iterador = *cabeca_linha;

		while(iterador->proximo != NULL)
			iterador = iterador->proximo;

		l->anterior = iterador;
		l->proximo = NULL;
		iterador->proximo = l;

		//return SUCESSO;
	}
	else
	{
		l->anterior = NULL;
		l->proximo = NULL;
		*cabeca_linha = l;
		//return ERRO;
	}
}


CODIGO_RETORNO cria_buffer_arquivo(LINHA** buffer, FILE* arquivo)
{
	// *buffer = (LINHA*) malloc(sizeof(struct linha));
	char chr;
	CARACTERE* c;
	cria_string(&c);
	//adiciona_caractere(&c, '@');
	// (*buffer)->string = c;
	// (*buffer)->anterior = NULL;
	// (*buffer)->proximo = NULL;

	while((chr = getc(arquivo)) != EOF)
	{
		if(chr == '\n')
		{
			/* Se o caractere for a nova linha, adiciona a string já lida, como linha e parte para outra */
			//cria_string(&c);
			//adiciona_caractere(&c, '@');
			insere_linha(buffer, c);
			cria_string(&c);
			//getch();
		}else
		{

			adiciona_caractere(&c, chr);
			//imprime_string(c);
		}
	}

	//Faz nada
	return ERRO;
}

//Função que dado uma cabeca de linha, imprime todo o buffer na tela.
void renderiza_buffer(LINHA* buff)
{
	LINHA* iterador_linha = buff;

	while(iterador_linha)
	{
		CARACTERE* iterador_str = iterador_linha->string;
		while(iterador_str)
		{
			addch(iterador_str->chr);
			iterador_str = iterador_str->proximo; 
		}
		addch('\n');
		iterador_linha = iterador_linha->proximo;
	}
}

int main(int argc, char** argv)
{
	inicia_ncurses();
	
	FILE* arquivo;
	
	if(abrir_arquivo("teste.txt", "r", &arquivo))
	{
		LINHA* cabeca = NULL;
		// LINHA* cabeca = (LINHA*) malloc(sizeof(struct linha));
		// CARACTERE* str;
		// cria_string(&str);
		// adiciona_caractere(&str, 'H');
		// adiciona_caractere(&str, 'e');
		// adiciona_caractere(&str, 'l');
		// adiciona_caractere(&str, 'l');
		// adiciona_caractere(&str, 'o');
		// cabeca->string = str;
		// cabeca->proximo = NULL;
		// cabeca->anterior = NULL;
		cria_buffer_arquivo(&cabeca, arquivo);

		renderiza_buffer(cabeca);
	}
	else
		printw("Não funcionou");

	refresh();
	getch();
	endwin();
	fclose(arquivo);

	return(0);
}