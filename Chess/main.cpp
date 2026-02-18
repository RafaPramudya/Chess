#include "Game.h"
#include "Chess.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char* argv[])
{
	Game chess;

	Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	chess.setBoard(&board);

	while (chess.isRunning()) {
		chess.event();
		chess.render();
	}
	board.printBoard();
	return 0;
}
