#include "graficos.h"

int limite_superior_tela;
int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;

void inicia_ncurses(void)
{
	initscr();				//Inicia o modo ncurses
	noecho();				//Não imprime as telcas
	cbreak();				//Desabilita o buffering de linhas
	keypad(stdscr, true);	//Habilita teclas especiais
	refresh();				//Recarrega a struct da janela

	getmaxyx(stdscr, tamanho_stdscr.y, tamanho_stdscr.x);
	limite_superior_tela = 3;

	janela_editor = newwin(tamanho_stdscr.y - 6, tamanho_stdscr.x, 3, 0);
	wrefresh(janela_editor);

	barra_titulo = newwin(3, tamanho_stdscr.x, 0, 0);
	box(barra_titulo, 0, 0);
	wrefresh(barra_titulo);

	barra_status = newwin(3, tamanho_stdscr.x, tamanho_stdscr.y - 3, 0);
	box(barra_status, 0, 0);

	wrefresh(barra_status);
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
		waddch(janela_editor, iterador->chr);
		iterador = iterador->proximo;
	}
	waddch(janela_editor, '\n');
}


//Função que dado uma cabeca de linha, imprime todo o buffer na tela.
void renderiza_buffer(LINHA* buff)
{
	werase(janela_editor);
	wmove(janela_editor, 0,0);
	LINHA* iterador_linha = buff;

	while(iterador_linha)
	{
		CARACTERE* iterador_str = iterador_linha->string;
		while(iterador_str)
		{
			if(iterador_str->chr != '\0')
				waddch(janela_editor, iterador_str->chr);
			iterador_str = iterador_str->proximo; 
		}
		//waddch(janela_editor, '\n');
		iterador_linha = iterador_linha->proximo;
	}
	wmove(janela_editor, cursor_y, cursor_x);
	wrefresh(janela_editor);
}

void move_cursor(size_t y, size_t x)
{
	wmove(janela_editor, y, x);
	getyx(janela_editor, cursor_y, cursor_x);
}

void atualiza_barra_status(DESCRITOR_ARQUIVO desc_arq, MODO modo)
{
	werase(barra_status);
	box(barra_status, 0, 0);

	switch(modo)
	{
		case COMANDO:
			curs_set(0);
			mvwprintw(barra_status, 1, 1, "");
		break;

		case INSERCAO:
			curs_set(1);
			mvwprintw(barra_status, 1, 1, "--INSERÇÃO--");
			mvwprintw(barra_status, 1, 20, "%d:%d", cursor_y, cursor_x);
		break;

		case SOBRESCRICAO:
			curs_set(1);
			mvwprintw(barra_status, 1, 1, "--SOBRESCRITA--");
			mvwprintw(barra_status, 1, 20, "%d:%d", cursor_y, cursor_x);
		break;
	}
	wrefresh(barra_status);
}