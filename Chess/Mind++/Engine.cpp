#include "Engine.h"

Engine::~Engine()
{
	if (enableLogging) loggingFile.close();
}

uint64_t Engine::transversePosition(uint8_t depth)
{
	if (depth == 0)
	{
		if (enableLogging) {
			loggingFile << board->getMoveNotation() << "\n";
		}
		return 1;
	}

	uint64_t totalMove = 0;
	auto moves = Move::generateMoves(board);

	for (auto& move : moves) {
		board->makeMove(move);
		totalMove += transversePosition(depth - 1); // Recursion
		board->unMakeMove(move);
	}

	return totalMove;
}

void Engine::enableLoggingFile(const char* filename)
{
	enableLogging = true;

	board->turnNotationDebug(true);
	loggingFile.open(filename);
}
