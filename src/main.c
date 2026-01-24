#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./renderer.h"
#include "./card.h"
#include "./board.h"
#include "./hand.h"
#include "./commands.h"
#include "./win.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

int new_game();
int start_new_game = FALSE;

int win = FALSE;

int initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
	);
	if (window == NULL) {
		fprintf(stderr, "Error creating SDL Window\n");
		return FALSE;
	}
	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating SDL Renderer\n");
		return FALSE;
	}

	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) && img_flags)) {
		fprintf(stderr, "SDL_image could not initialize! IMG_Error: %s\n", IMG_GetError());
		return FALSE;
	}
	return TRUE;
}

void process_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				game_is_running = FALSE;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					game_is_running = FALSE;
					win = FALSE;
				}
				if (event.key.keysym.sym  == SDLK_BACKSPACE && win == FALSE) {
					Reverse_Command();
				}
				if (event.key.keysym.sym == SDLK_r) {
					win = FALSE;
					Restart_CurrentBoard();
				}
				if (event.key.keysym.sym == SDLK_n) {
					win = FALSE;
					start_new_game = TRUE;
				}
				/*
				if (event.key.keysym.sym == SDLK_w) {
					Debug_Win();
				}
				*/
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT && win == FALSE) {
					On_MouseButtonDown();
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT && win == FALSE) {
					On_MouseButtonUp();
					win = Is_GameCompleted();
				}
				break;
		}
	}
}

void update() {
	Update_Hand();
}

void render() {
	SDL_SetRenderDrawColor(renderer, 92, 8, 11, 255);
	SDL_RenderClear(renderer);

	RenderItems(renderer);
	
	SDL_RenderPresent(renderer);
}

int setup() {
	srand(time(NULL));
	if (Init_Textures(renderer) == FALSE) return FALSE;
	if (Init_Deck() == FALSE) return FALSE;
	if (Init_Board() == FALSE) return FALSE;
	if (Init_Hand() == FALSE) return FALSE;
	if (Init_CommandQueue() == FALSE) return FALSE;
	return TRUE;
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void destroy_game() {
	Destroy_CommandQueue();
	Destroy_Hand();
	Destroy_Board();
	Destroy_Deck();
	Destroy_Textures();
}

int new_game() {
	Destroy_CommandQueue();
	Destroy_Hand();
	Destroy_Board();
	Destroy_Deck();
	if (Init_Deck() == FALSE) return FALSE;
	if (Init_Board() == FALSE) return FALSE;
	if (Init_Hand() == FALSE) return FALSE;
	if (Init_CommandQueue() == FALSE) return FALSE;
	return TRUE;
}

int main() {
	game_is_running = initialize_window();
	game_is_running = setup();

	while (game_is_running) {
		process_input();
		update();
		render();
		if (start_new_game == TRUE) {
			start_new_game = FALSE;
			new_game();
		}
	}

	destroy_game();
	destroy_window();
}
