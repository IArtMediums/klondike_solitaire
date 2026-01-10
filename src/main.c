#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./texture_object.h"
#include "./card_object.h"
#include "./game_board.h"
#include "./collision.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

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
		fprintf(stderr, "Error creating SDL Window.\n");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating SDL REnderer.\n");
		return FALSE;
	}
	return TRUE;
}

void process_input() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				game_is_running = FALSE;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				Collision collision = new_collision(event.button.x, event.button.y, 2, 2);
				CollisionData data = collide_with_board(Board, collision);
				if (data.object_type == OBJ_CARD) {
					printf("Selected Card in column: %d, row: %d\n", data.column_index, data.stack_index);
				}
			}
	}
}

void update() {

}

void render() {
	SDL_SetRenderDrawColor(renderer, 92, 8, 11, 255);
	SDL_RenderClear(renderer);

	render_texture_buffer(renderer);

	SDL_RenderPresent(renderer);
}

void setup() {
	srand(time(NULL));
	init_texture_resources(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	game_is_running = initialize_window();
	setup();
	Deck* deck = new_deck();
	deal_cards(deck);

	while (game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_gameboard(Board);
	free_deck(deck);
	destroy_texture_resources();
	destroy_window();

	return 0;
}
