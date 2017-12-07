#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>

#include "arquivos.h"
#include "editor.h"
#include "graficos.h"

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	inicia_ncurses();
	
	tamanho_janela tamanho_stdscr;
	getmaxyx(stdscr, tamanho_stdscr.y, tamanho_stdscr.x);

	WINDOW* barra_status = newwin(3, tamanho_stdscr.x, tamanho_stdscr.y - 3, 0);
	box(barra_status, 0, 0);
	mvwprintw(barra_status, 1, 1, "--INSERÇÂO--");

	wrefresh(barra_status);
	
	FILE* arquivo;
	
	if(abrir_arquivo("teste.txt", "r+", &arquivo))
	{
		descritor_arquivo desc_arq;
		strcpy(desc_arq.nome, "teste.txt");
		desc_arq.bytes = tamanho_arquivo(arquivo);
		mvwprintw(barra_status, 1, 20, "%d Bytes", desc_arq.bytes);
		wrefresh(barra_status);
		LINHA* cabeca = NULL;
		cria_buffer_arquivo(&cabeca, arquivo);

		renderiza_buffer(cabeca);
	}
	else
		printw("Não funcionou");


	//Loop principal
	while(1)
	{
		int cursor_x, cursor_y;
		getyx(stdscr, cursor_y, cursor_x);
		char ch = getch();

		switch(ch)
		{
			case 2:
				move(cursor_y + 1, cursor_x);
			break;

			case 3:
				move(cursor_y - 1, cursor_x);
			break;

			case 4:
				move(cursor_y, cursor_x - 1);
			break;

			case 5:
				move(cursor_y, cursor_x + 1);
			break;

		}

		//printw("%d ", ch);

		refresh();
	}

	refresh();
	getch();
	endwin();
	fclose(arquivo);

	return(0);
}