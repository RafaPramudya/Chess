#include "Chess.h"
#include "Engine.h"

#include <iostream>

int main(int argc, char* argv[]) {
	Board board(Board::defaultFen);

	Engine engine(&board);
	engine.enableLoggingFile("depth4_list.txt");

	for (uint8_t depth = 1; depth <= 6; depth++) {
		std::printf("Total Moves with depth %d : %llu\n", depth, engine.transversePosition(depth));
	}

	return 0;
}