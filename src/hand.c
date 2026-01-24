#include <SDL2/SDL.h>
#include "./vector.h"
#include "./constants.h"
#include "./hand.h"
#include "./board.h"
#include "./commands.h"


Vector2* HandPosition = NULL;
Collision selected = {FALSE, NULL, 0, (Vector2){0, 0}};

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
		selected = collision;
		move_cards();
	}
}

void On_MouseButtonUp() {
	if (selected.is_colliding == FALSE) return;
	if (selected.src == NULL) return;
	if (CurrentBoard == NULL) return;
	if (CurrentBoard->hand == NULL) return;
	if (CurrentBoard->hand->size == 0) return;
	Collision collision = Get_Collision();
	if (collision.is_colliding == TRUE && collision.src->type != WASTE && collision.src->type != STOCK) {
		// right now it defaults to return cards from hand
		Stack* src = selected.src;
		Stack* dest = collision.src;
		int src_index = selected.index;
		return_selection();
		Issue_Command(MOVE_CARD, src, dest, src_index);
	} else {
		return_selection();
	}
}

void move_cards() {
	switch (selected.src->type) {
		case TABLEAU:
			if (selected.src->size == 0) {
				selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
				return;
			}
			if (selected.src->data[selected.index]->is_facedown) {
				selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
				return;
			}
			pick_selection();
			break;
		case FOUNDATION:
			if (selected.src->size == 0) {
				selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
				return;
			}
			pick_selection();
			break;
		case WASTE:
			if (selected.src->size == 0 && CurrentBoard->stock->size != 0) {
				Issue_Command(STOCK_TO_WASTE, CurrentBoard->stock, CurrentBoard->waste, CurrentBoard->stock->size - 1);
				selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
			} else if (selected.src->size > 0) {
				pick_selection();
			}
			break;
		case STOCK:
			if (selected.src->size == 0 && CurrentBoard->waste->size != 0) {
				Issue_Command(REFILL_STOCK, CurrentBoard->waste, CurrentBoard->stock, -1);
			} else if (selected.src->size > 0) {
				Issue_Command(STOCK_TO_WASTE, CurrentBoard->stock, CurrentBoard->waste, CurrentBoard->stock->size - 1);
			}
			selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
			break;
		case HAND:
			selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
			
			return;
	}
}

void pick_selection() {
	if (selected.is_colliding == FALSE) return;
	if (CurrentBoard == NULL) return;
	if (CurrentBoard->hand == NULL) return;
	if (selected.src == NULL) return;
	Stack_SplitAndMerge(selected.src, CurrentBoard->hand, selected.index);
}

void return_selection() {
	if (selected.is_colliding == FALSE) return;
	if (selected.src == NULL) return;
	if (CurrentBoard == NULL) return;
	if (CurrentBoard->hand == NULL) return;
	if (CurrentBoard->hand->size == 0) return;
	Stack_SplitAndMerge(CurrentBoard->hand, selected.src, 0);
	selected = (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
}

Collision Get_Collision() {
	Vector2 hand_size = New_Vector2(HAND_WIDTH, HAND_HEIGHT);
	// check singles
	if (CurrentBoard == NULL) return (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};

	if (CurrentBoard->stock != NULL) {
		if (is_colliding(*HandPosition, hand_size, CurrentBoard->stock->position, CurrentBoard->stock->hitbox_size)) {
			return (Collision){TRUE, CurrentBoard->stock, CurrentBoard->stock->size - 1, get_offset(CurrentBoard->stock->position)};
		}
	}
	if (CurrentBoard->waste != NULL) {
		if (is_colliding(*HandPosition, hand_size, CurrentBoard->waste->position, CurrentBoard->waste->hitbox_size)) {
			return (Collision){TRUE, CurrentBoard->waste, CurrentBoard->waste->size - 1, get_offset(CurrentBoard->waste->position)};
		}
	}
	// check foundation
	if (CurrentBoard->foundation != NULL) {
		for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
			Stack* current_stack = CurrentBoard->foundation[i];
			if (current_stack == NULL) continue;
			if (is_colliding(*HandPosition, hand_size, current_stack->position, current_stack->hitbox_size)){
				return (Collision){TRUE, current_stack, current_stack->size - 1, get_offset(current_stack->position)};
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
					return (Collision){TRUE, current_stack, j, get_offset(card->position)};
				}
			}
			if (is_colliding(*HandPosition, hand_size, current_stack->position, current_stack->hitbox_size)) {
				return (Collision) {TRUE, current_stack, -1, get_offset(current_stack->position)};
			}
		}
	}
	return (Collision){FALSE, NULL, 0, New_Vector2(0, 0)};
}

Vector2 get_offset(Vector2 from) {
	return New_Vector2(from.x - HandPosition->x, from.y - HandPosition->y);
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
		card->position.x = HandPosition->x + selected.offset.x;
		card->position.y = HandPosition->y + (TABLEAU_Y_INCREMENT * i) + selected.offset.y;
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
