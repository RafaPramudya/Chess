#include "Game.h"
#include "Chess.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <chrono>

#define FPS 60.0

int main(int argc, char* argv[])
{
	Game game_instance;

	Board board(Board::defaultFen);
	//Board board("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	//Board board("rn1qkbnr/pbpppppp/1p6/8/8/N7/PPPPPPPP/1RBQKBNR w Kkq - 2 3");

	board.turnNotationDebug(true);
	game_instance.setBoard(&board);

	const std::chrono::duration<double> frameTime(1.0 / FPS);

	while (game_instance.isRunning()) {
		auto frameStart = std::chrono::high_resolution_clock::now();
		game_instance.event();
		game_instance.render();
		auto frameEnd = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed = frameEnd - frameStart;

		if (elapsed < frameTime) {
			uint32_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(frameTime - elapsed).count();

			if (ms > 0) SDL_Delay(ms);
		}
	}
	board.printBoard();
	return 0;
}
