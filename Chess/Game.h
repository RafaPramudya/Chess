#pragma once

#include <SDL3/SDL.h>

class Game {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event e;
	
	bool running = false;
public:
	Game();
	~Game();

	void event();
	void render();

	bool isRunning() const { return running; }
};