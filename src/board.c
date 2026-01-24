#include <stdlib.h>
#include "./vector.h"
#include "./card.h"
#include "./constants.h"
#include "./renderer.h"
#include "./board.h"

Board* CurrentBoard = NULL;

RenderBuffer Get_BoardRenderBuffer() {
	RenderBuffer r = {0};
	if (CurrentBoard == NULL) return (RenderBuffer){0};

	r.size = 0;
	r.capacity = DECK_SIZE;
	r.data = malloc(r.capacity * sizeof(RenderItem));
	if (r.data == NULL) return (RenderBuffer){0};

	// load foundation
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		Stack* current_column = CurrentBoard->foundation[i];
		if (current_column == NULL) continue;
		for (int j = 0; j < current_column->size; j++) {
			Card* card = Stack_PeekIndex(current_column, j);
			if (card == NULL) continue;
			r.data[r.size] = Get_CardRenderItem(card);
			r.size++;
		}
	}
	// load stock
	if (CurrentBoard->stock != NULL) {
		for (int i = 0; i < CurrentBoard->stock->size; i++) {
			Card* card = Stack_PeekIndex(CurrentBoard->stock, i);
			if (card == NULL) continue;
			r.data[r.size] = Get_CardRenderItem(card);
			r.size++;
		}
	}
	// load waste
	if (CurrentBoard->waste != NULL) {
		for (int i = 0; i < CurrentBoard->waste->size; i++) {
			Card* card = Stack_PeekIndex(CurrentBoard->waste, i);
			if (card == NULL) continue;
			r.data[r.size] = Get_CardRenderItem(card);
			r.size++;
		}
	}
	
	// load tableau
	for (int i = 0; i < TABLEAU_COLUMS; i++) {
		Stack* current_column = CurrentBoard->tableau[i];
		if (current_column == NULL) continue;
		for (int j = 0; j < current_column->size; j++) {
			Card* card = Stack_PeekIndex(current_column, j);
			if (card == NULL) continue;
			r.data[r.size] = Get_CardRenderItem(card);
			r.size++;
		}
	}
	// load hand
	if (CurrentBoard->hand != NULL) {
		for (int i = 0; i < CurrentBoard->hand->size; i++) {
			Card* card = Stack_PeekIndex(CurrentBoard->hand, i);
			if (card == NULL) continue;
			r.data[r.size] = Get_CardRenderItem(card);
			r.size++;
		}
	}
	return r;
}

int Init_Board() {
	if (CurrentDeck == NULL) return FALSE;
	if (CurrentBoard != NULL) {
		Destroy_Board();
	}
	CurrentBoard = malloc(sizeof(Board));
	if (CurrentBoard == NULL) return FALSE;
	
	// init tableau
	CurrentBoard->tableau = calloc(TABLEAU_COLUMS, sizeof(Stack*));
	if (CurrentBoard->tableau == NULL) {
		free(CurrentBoard);
		CurrentBoard = NULL;
		return FALSE;
	}
	for (int i = 0; i < TABLEAU_COLUMS; i++) {
		CurrentBoard->tableau[i] = new_stack(
			20, 
			New_Vector2(TABLEAU_X + (TABLEAU_X_INCREMENT * i), TABLEAU_Y), 
			New_Vector2(CARD_WIDTH, CARD_HEIGHT),
			TABLEAU,
			i
		);
		if (CurrentBoard->tableau[i] == NULL) {
			Destroy_Board();
			return FALSE;
		}
	}

	// init foundation
	CurrentBoard->foundation = calloc(FOUNDATION_COLUMNS, sizeof(Stack*));
	if (CurrentBoard->foundation == NULL)  {
		Destroy_Board();
		return FALSE;
	}
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		CurrentBoard->foundation[i] = new_stack(
			13,
			New_Vector2(FOUNDATION_X + (FOUNDATION_X_INCREMENT * i), TOP_Y),
			New_Vector2(CARD_WIDTH, CARD_HEIGHT),
			FOUNDATION,
			i
		);
		if (CurrentBoard->foundation[i] == NULL) {
			Destroy_Board();
			return FALSE;
		}
	}

	// init stock
	CurrentBoard->stock = new_stack(
		30,
		New_Vector2(STOCK_X, TOP_Y),
		New_Vector2(CARD_WIDTH, CARD_HEIGHT),
		STOCK,
		0
	);
	if (CurrentBoard->stock == NULL) {
		Destroy_Board();
		return FALSE;
	}
	// init waste
	CurrentBoard->waste = new_stack(
		30,
		New_Vector2(WASTE_X, TOP_Y),
		New_Vector2(CARD_WIDTH, CARD_HEIGHT),
		WASTE,
		0
	);
	if (CurrentBoard->waste == NULL) {
		Destroy_Board();
		return FALSE;
	}
	// init hand
	CurrentBoard->hand = new_stack(
		30,
		New_Vector2(0, 0),
		New_Vector2(1, 1),
		HAND,
		0
	);
	if (CurrentBoard->hand == NULL) {
		Destroy_Board();
		return FALSE;
	}

	// populate board
	int current_card_index = 0;
	for (int i = 0; i < TABLEAU_COLUMS; i++) {
		Stack* current_column = CurrentBoard->tableau[i];
		for (int j = 0; j < i + 1; j++) {
			Stack_PushCard(current_column, CurrentDeck->data[current_card_index]);
			if (j == i) {
				CurrentDeck->data[current_card_index]->is_facedown = FALSE;
			}
			current_card_index++;
		}
	}
	// populate stock
	for (int i = current_card_index; i < CurrentDeck->size; i++) {
		Stack_PushCard(CurrentBoard->stock, CurrentDeck->data[i]);
	}
	return TRUE;
}

void Destroy_Board() {
	if (CurrentBoard == NULL) return;
	// destroy tableau
	if (CurrentBoard->tableau != NULL) {
		for (int i = 0; i < TABLEAU_COLUMS; i++) {
			Stack* current_column = CurrentBoard->tableau[i];
			destroy_stack(current_column);
		}
		free(CurrentBoard->tableau);
	}
	// destroy foundation
	if (CurrentBoard->foundation != NULL) {
		for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
			Stack* current_column = CurrentBoard->foundation[i];
			destroy_stack(current_column);
		}
		free(CurrentBoard->foundation);
	}
	
	if (CurrentBoard->stock != NULL) {
		destroy_stack(CurrentBoard->stock);
	}
	if (CurrentBoard->waste != NULL) {
		destroy_stack(CurrentBoard->waste);
	}
	if (CurrentBoard->hand != NULL) {
		destroy_stack(CurrentBoard->hand);
	}
	free(CurrentBoard);
	CurrentBoard = NULL;
}

Stack* new_stack(int capacity, Vector2 position, Vector2 hitbox_size, StackType type, int column_index) {
	Stack* stack = malloc(sizeof(Stack));
	if (stack == NULL) return NULL;
	stack->type = type;
	stack->position = position;
	stack->hitbox_size = hitbox_size;
	stack->column_index = column_index;
	stack->size = 0;
	stack->capacity = capacity;
	stack->data = calloc(capacity, sizeof(Card*));
	if (stack->data == NULL) {
		free(stack);
		return NULL;
	}
	return stack;
}

void set_card_position(Stack* stack, Card* card) {
	if (stack == NULL) return;
	if (card == NULL) return;
	switch (stack->type) {
		case TABLEAU:
			if (stack->size == 0) {
				card->position.x = stack->position.x;
				card->position.y = stack->position.y;
			} else {
				card->position.x = stack->position.x;
				Card* last_card = Stack_PeekCard(stack);
				card->position.y = last_card->position.y + TABLEAU_Y_INCREMENT;
			}
			break;
		case FOUNDATION:
		case STOCK:
		case WASTE:
			card->position.x = stack->position.x;
			card->position.y = stack->position.y;
			break;
		case HAND:
			return;
	}
}

void Stack_PushCard(Stack* dest, Card* card) {
	if (card == NULL) return;
	if (dest == NULL) return;
	if (dest->size == dest->capacity) {
		int new_cap = dest->capacity * 2;
		Card** old_data = dest->data;
		dest->data = realloc(dest->data, new_cap * sizeof(Card*));
		if (dest->data == NULL) {
			dest->data = old_data;
			return;
		}
		dest->capacity = new_cap;
	}
	dest->data[dest->size] = card;
	set_card_position(dest, card);
	dest->size++;
}

Card* Stack_PopCard(Stack* src) {
	if (src == NULL) return NULL;
	if (src->size == 0) return NULL;
	src->size--;
	return src->data[src->size];
}

Card* Stack_PeekCard(Stack* src) {
	if (src == NULL) return NULL;
	if (src->size == 0) return NULL;
	return src->data[src->size - 1];
}

Card* Stack_PeekIndex(Stack* src, int index) {
	if (src == NULL) return NULL;
	if (src->size == 0) return NULL;
	if (index >= src->size) return NULL;
	if (index == -1) return src->data[src->size -1];
	return src->data[index];
}

int Stack_GetCardIndex(Stack* src, Card* card) {
	if (card == NULL) return -1;
	if (src == NULL) return -1;
	for (int i = 0; i < src->size; i++) {
		if (src->data[i] == card) {
			return i;
		}
	}
	return -1;
}

void Stack_SplitAndMerge(Stack* src, Stack* dest, int split_index) {
	if (src == NULL) return;
	if (dest == NULL) return;
	if (split_index >= src->size || split_index < 0) return;
	for (int i = split_index; i < src->size; i++) {
		Stack_PushCard(dest, src->data[i]);
		src->data[i] = NULL;
	}
	src->size = split_index;
}

void destroy_stack(Stack* src) {
	if (src == NULL) return;
	free(src->data);
	free(src);
}
