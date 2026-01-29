#include "Game.h"

#include <iostream>
#include <exception>

#include <SDL3_image/SDL_image.h>

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

	spriteTexture = IMG_LoadTexture(renderer, "assets/ChessPiecesArray.png");
	if (!spriteTexture) throw std::exception("Error Texture Intialization");

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
	//SDL_SetRenderTarget(renderer, spriteTexture);
	//SDL_SetRenderDrawColor(renderer, 10, 10, 255, 255);
	//SDL_RenderClear(renderer);

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

	for (int rank = 7; rank >= 0; rank--) {
		for (int file = 0; file < 8; file++) {
			auto& piece = board->getPieces()[rank * 8 + file];
			if ((piece.getId() % 8) == Piece::NONE) continue;

			const auto sourceRect = SpriteManager::getSpriteRect(piece);
			const auto destRect = SDL_FRect{ 
				.x = static_cast<float>(file * BOARD_WIDTH / 8), 
				.y = static_cast<float>((7 - rank) * BOARD_HEIGHT / 8),
				.w = BOARD_WIDTH / 8, 
				.h = BOARD_HEIGHT / 8 
			};
			SDL_RenderTexture(renderer, spriteTexture, &sourceRect, &destRect);
		}
	}

	SDL_SetRenderTarget(renderer, nullptr);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
	SDL_RenderClear(renderer);

	const auto boardRenderDestRect = SDL_FRect{
		.x = 0,
		.y = 0,
		.w = BOARD_WIDTH,
		.h = BOARD_HEIGHT
	};
	SDL_RenderTexture(renderer, boardTexture, nullptr, &boardRenderDestRect);
	SDL_RenderPresent(renderer);
}
