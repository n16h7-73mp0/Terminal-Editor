#include "graficos.h"

void inicia_ncurses(void)
{
	initscr();				//Inicia o modo ncurses
	noecho();				//Não imprime as telcas
	cbreak();				//Desabilita o buffering de linhas
	keypad(stdscr, true);	//Habilita teclas especiais
	refresh();				//Recarrega a struct da janela
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