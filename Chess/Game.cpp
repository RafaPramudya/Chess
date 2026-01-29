#include "Game.h"

#include <iostream>
#include <exception>

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	800

#define BOARD_WIDTH		800
#define BOARD_HEIGHT	800

Game::Game()
{
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) throw std::exception("Error SDL Initialization");

	window = SDL_CreateWindow("Catur", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
	if (!window) throw std::exception("Error Window Initialization");

	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) throw std::exception("Error Renderer Initialization");
	
	boardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, BOARD_WIDTH, BOARD_HEIGHT);

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
			break;
		default:
			break;
		}
	}
}

void Game::render()
{
	SDL_SetRenderTarget(renderer, boardTexture);
	SDL_SetRenderDrawColor(renderer, 215, 186, 137, 255);
	for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 4; file++) {
			int offset = (rank % 2) ? BOARD_WIDTH / 8 : 0;
			SDL_FRect drawRect = {
				.x = static_cast<float>(file * BOARD_WIDTH / 4 + offset),
				.y = static_cast<float>(rank * BOARD_HEIGHT / 8),
				.w = static_cast<float>(BOARD_WIDTH / 8),
				.h = static_cast<float>(BOARD_HEIGHT / 8)
			};

			SDL_RenderFillRect(renderer, &drawRect);
		}
	}

	SDL_SetRenderDrawColor(renderer, 152, 107, 65, 255);
	for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 4; file++) {
			int offset = (rank % 2) ? 0 : SCREEN_WIDTH / 8;
			SDL_FRect drawRect = {
				.x = static_cast<float>(file * BOARD_WIDTH / 4 + offset),
				.y = static_cast<float>(rank * BOARD_HEIGHT / 8),
				.w = static_cast<float>(BOARD_WIDTH / 8),
				.h = static_cast<float>(BOARD_HEIGHT / 8)
			};

			SDL_RenderFillRect(renderer, &drawRect);
		}
	}
	
	SDL_SetRenderTarget(renderer, nullptr);
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);
	SDL_RenderTexture(renderer, boardTexture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}
