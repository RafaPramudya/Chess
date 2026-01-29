#include "Game.h"
#include "Chess.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
	Game chess;

	Board board(Board::defaultFen);
	board.printBoard();

	chess.setBoard(&board);

	while (chess.isRunning()) {
		chess.event();
		chess.render();
	}
	return 0;
}
