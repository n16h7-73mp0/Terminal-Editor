#include "graficos.h"

int cursor_y, cursor_x;
WINDOW* janela_editor;
WINDOW* barra_titulo;
WINDOW* barra_status;
tamanho_janela tamanho_stdscr;
int modificado;

void inicia_ncurses(void)
{
	initscr();				//Inicia o modo ncurses
	noecho();				//Não imprime as telcas
	cbreak();				//Desabilita o buffering de linhas
	keypad(stdscr, true);	//Habilita teclas especiais
	refresh();				//Recarrega a struct da janela

	//Pega o tamanho da janela
	getmaxyx(stdscr, tamanho_stdscr.y, tamanho_stdscr.x);


	//Cria as janelas
	janela_editor = newwin(tamanho_stdscr.y - 6, tamanho_stdscr.x, 3, 0);
	wrefresh(janela_editor);

	barra_titulo = newwin(3, tamanho_stdscr.x, 0, 0);
	box(barra_titulo, 0, 0);
	wrefresh(barra_titulo);

	barra_status = newwin(3, tamanho_stdscr.x, tamanho_stdscr.y - 3, 0);
	box(barra_status, 0, 0);

	wrefresh(barra_status);
}

//Função que dado uma cabeca de linha, imprime todo o buffer na tela.
void renderiza_buffer(Cabeca cabeca)
{
	//Apaga a janela do editor
	werase(janela_editor);
	//Move o cursor para o canto superior esquerdo
	wmove(janela_editor, 0,0);

	//Percorre linha por linha
	IteradorLinha iterador_linha = cabeca;
	while(iterador_linha)
	{
		//Percorre caractere por caractere
		IteradorString iterador_str = iterador_linha->string;
		while(iterador_str)
		{
			//Imprime todo caractere diferente de NULL
			if(iterador_str->chr != '\0')
				waddch(janela_editor, iterador_str->chr);
			iterador_str = iterador_str->proximo; 
		}
		iterador_linha = iterador_linha->proximo;
	}
	//move o cursor pro lugar que ele estava antes
	wmove(janela_editor, cursor_y, cursor_x);
	//atualiza a exibição da janela
	wrefresh(janela_editor);
}

void move_cursor(size_t y, size_t x)
{
	//Move o cursor
	wmove(janela_editor, y, x);
	//Atualiza variáveis (obs: getyx é uma macro)
	getyx(janela_editor, cursor_y, cursor_x);
}

void atualiza_barra_status(MODO modo)
{
	//Apaga a janela
	werase(barra_status);
	//Desenha a linha em volta da janela
	box(barra_status, 0, 0);
	
	switch(modo)
	{
		case OCIOSO:
			//Desliga o cursor
			curs_set(0);
		break;

		case INSERCAO:
			//Liga o cursor
			curs_set(1);
			//Imprime informações
			mvwprintw(barra_status, 1, 1, "--INSERÇÃO--");
			mvwprintw(barra_status, 1, 20, "%d:%d", cursor_y, cursor_x);
		break;
	}
	//Se o arquivo foi modificado
	if(modificado)
		mvwprintw(barra_status, 1, 30, "--MODIFICADO--");

	wrefresh(barra_status);
}

void termina_ncurses(void)
{
	//termina as janelas
	delwin(janela_editor);
	delwin(barra_titulo);
	delwin(barra_status);
	endwin();
}