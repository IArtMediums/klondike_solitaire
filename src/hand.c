#include <SDL2/SDL.h>
#include "./vector.h"
#include "./constants.h"
#include "./hand.h"
#include "./board.h"


Vector2* HandPosition = NULL;

void Update_Hand() {
	if (HandPosition == NULL) return;
	SDL_GetMouseState(&HandPosition->x, &HandPosition->y);
	if (CurrentBoard == NULL) return;
	if (CurrentBoard->hand == NULL) return;
	if (CurrentBoard->hand->size == 0) return;
	update_card_positions();
}

void On_MouseButtonDown() {
	Collision collision = Get_Collision();
	if (collision.is_colliding == TRUE) {
		printf("Collided with stack type: %d, at index %d\n", collision.src->type, collision.index);
	}
}

Collision Get_Collision() {
	Vector2 hand_size = New_Vector2(HAND_WIDTH, HAND_HEIGHT);
	// check singles
	if (CurrentBoard == NULL) return (Collision){FALSE, NULL, 0};

	if (CurrentBoard->stock != NULL) {
		if (is_colliding(*HandPosition, hand_size, CurrentBoard->stock->position, CurrentBoard->stock->hitbox_size)) {
			return (Collision){TRUE, CurrentBoard->stock, CurrentBoard->stock->size - 1};
		}
	}
	if (CurrentBoard->waste != NULL) {
		if (is_colliding(*HandPosition, hand_size, CurrentBoard->waste->position, CurrentBoard->waste->hitbox_size)) {
			return (Collision){TRUE, CurrentBoard->waste, CurrentBoard->waste->size - 1};
		}
	}
	// check foundation
	if (CurrentBoard->foundation != NULL) {
		for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
			Stack* current_stack = CurrentBoard->foundation[i];
			if (current_stack == NULL) continue;
			if (is_colliding(*HandPosition, hand_size, current_stack->position, current_stack->hitbox_size)){
				return (Collision){TRUE, current_stack, current_stack->size - 1};
			}
		}
	}

	if (CurrentBoard->tableau != NULL) {
		for (int i = 0; i < TABLEAU_COLUMS; i++) {
			Stack* current_stack = CurrentBoard->tableau[i];
			if (current_stack == NULL) continue;
			for (int j = current_stack->size -1; j >= 0; j--) {
				Card* card = current_stack->data[j];
				if (card == NULL) continue;
				if (is_colliding(*HandPosition, hand_size, card->position, New_Vector2(CARD_WIDTH, CARD_HEIGHT))) {
					return (Collision){TRUE, current_stack, j};
				}
			}
		}
	}
	return (Collision){FALSE, NULL, 0};
}

int is_colliding(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2) {
	if (pos1.x + size1.x <= pos2.x) return FALSE;
	if (pos1.x >= pos2.x + size2.x) return FALSE;
	if (pos1.y + size1.y <= pos2.y) return FALSE;
	if (pos1.y  >= pos2.y + size2.y) return FALSE;
	return TRUE;
}

void update_card_positions() {
	for (int i = 0; i < CurrentBoard->hand->size; i++) {
		Card* card = CurrentBoard->hand->data[i];
		if (card == NULL) continue;
		card->position.x = HandPosition->x;
		card->position.y = HandPosition->y + (TABLEAU_Y_INCREMENT * i);
	}
}

int Init_Hand() {
	HandPosition = malloc(sizeof(Vector2));
	if (HandPosition == NULL) return FALSE;
	HandPosition->x = 0;
	HandPosition->y = 0;
	return TRUE;
}

void Destroy_Hand() {
	if (HandPosition == NULL) return;
	free(HandPosition);
	HandPosition = NULL;
}
