#include "Game.h"

#include <iostream>
#include <exception>

Game::Game()
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) throw std::exception("Error SDL Initialization");

	window = SDL_CreateWindow("Catur", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window) throw std::exception("Error Window Initialization");

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) throw std::exception("Error Renderer Initialization");
	
	running = true;
}

Game::~Game() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::event()
{
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_EVENT_QUIT:
			running = false;
			std::printf("Quit");
			break;
		default:
			break;
		}
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}
