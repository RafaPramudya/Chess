#pragma once

#include <SDL3/SDL.h>

#include "Chess.h"

#include <map>

#define SPRITE_WIDTH 60
#define SPRITE_HEIGHT 60

class SpriteManager {
private:
	inline static std::map<char, const SDL_FRect> spriteMap = {
		{'q', SDL_FRect{.x = 0.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'Q', SDL_FRect{.x = 0.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'k', SDL_FRect{.x = 1.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'K', SDL_FRect{.x = 1.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'r', SDL_FRect{.x = 2.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'R', SDL_FRect{.x = 2.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'n', SDL_FRect{.x = 3.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'N', SDL_FRect{.x = 3.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'b', SDL_FRect{.x = 4.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'B', SDL_FRect{.x = 4.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'p', SDL_FRect{.x = 5.0 * SPRITE_WIDTH, .y = 0.0, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
		{'P', SDL_FRect{.x = 5.0 * SPRITE_WIDTH, .y = SPRITE_HEIGHT, .w = SPRITE_WIDTH, .h = SPRITE_HEIGHT}},
	};
public:
	static const SDL_FRect getSpriteRect(Piece piece) {
		char notation = piece.getNotation();
		return spriteMap[notation];
	}
};

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;
	
	SDL_Texture* boardTexture = nullptr;
	SDL_Texture* spriteTexture = nullptr;

	bool running = false;

	Board* board = nullptr;
public:
	Game();
	~Game();

	void setBoard(Board* board) { this->board = board; }

	void event();
	void render();

	bool isRunning() const { return running; }
};