#pragma once

#include "Chess.h"
#include <fstream>

class Engine {
private:
	Board* board = nullptr;

	std::ofstream loggingFile;
	bool enableLogging = false;
public:
	Engine(Board* board) : board(board) {}
	~Engine();

	uint64_t transversePosition(uint8_t depth);

	void enableLoggingFile(const char* filename);
};