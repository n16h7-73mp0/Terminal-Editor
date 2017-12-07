#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ncurses.h>
#include <locale.h>

#include "arquivos.h"
#include "editor.h"
#include "graficos.h"

//Variaveis de graficos.h
int limte_superior_tela;
int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;

MODO modo;

PROPRIEDADES_EDITOR editor_prop;
int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");
	inicia_ncurses();

	FILE* arquivo;
	DESCRITOR_ARQUIVO desc_arq;
	LINHA* cabeca = NULL;
	
	if(abrir_arquivo("teste.txt", "r+", &arquivo))
	{
		modo = COMANDO;
		strcpy(desc_arq.nome, "teste.txt");
		desc_arq.bytes = tamanho_arquivo(arquivo);
		mvwprintw(barra_titulo, 1, (tamanho_stdscr.x - strlen(desc_arq.nome))/2, "%s", desc_arq.nome);
		mvwprintw(barra_status, 1, 20, "%d Bytes", desc_arq.bytes);
		wrefresh(barra_titulo);
		wrefresh(barra_status);

		
		cria_buffer_arquivo(&cabeca, arquivo);
		//remove_caractere(&cabeca, 0, 0);
		renderiza_buffer(cabeca);
		//wprintw(janela_editor, "%d", editor_prop.linhas);
		//wrefresh(janela_editor);
		wmove(janela_editor,0,0);
		curs_set(0);
		wrefresh(janela_editor);
	}
	else
		printw("Não funcionou");


	//Loop principal
	for(;;)
	{
		getyx(janela_editor, cursor_y, cursor_x);
		wchar_t ch = getch();

		if(ch == KEY_DOWN && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y + 1, cursor_x);
		else if(ch == KEY_UP && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y - 1, cursor_x);
		else if(ch == KEY_LEFT && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y, cursor_x - 1);
		else if(ch == KEY_RIGHT && (modo == INSERCAO || modo == SOBRESCRICAO))
			move_cursor(cursor_y, cursor_x + 1);
		else if(ch == KEY_IC)
			percorre_modo(&modo);
		else if(ch == KEY_BACKSPACE && modo == INSERCAO && cursor_x != 0)
		{
			remove_caractere(&cabeca, cursor_y, cursor_x - 1);
			renderiza_buffer(cabeca);
			move_cursor(cursor_y, cursor_x - 1);
		}

		atualiza_barra_status(desc_arq, modo);
		wrefresh(janela_editor);
	}

	refresh();
	getch();
	endwin();
	fclose(arquivo);

	return(0);
}