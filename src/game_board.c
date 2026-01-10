#include "./game_board.h"
#include "./card_object.h"
#include "./texture_object.h"
#include "./constants.h"
#include "./collision.h"

GameBoard* Board = NULL;



TextureResourceList* get_card_render_buffer() {
	if (Board == NULL) return NULL;
	TextureResourceList* buffer = new_texture_resource_list(DECK_SIZE);
	
	// Load tableau to buffer
	for (int i = 0; i < TABLEAU_COLUMNS; i++) {
		Tableau* current_column = Board->tableau[i];
		Stack* current_stack = current_column->data;
		for (int j = 0; j < current_stack->size; j++) {
			add_texture_resource_to_list(buffer, current_stack->data[j]->resource);
		}
	}

	// Load foundation to buffer
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		Foundation* current_column = Board->foundation[i];
		Stack* current_stack = current_column->data;
		for (int j = 0; j < current_stack->size; j++) {
			add_texture_resource_to_list(buffer, current_stack->data[j]->resource);
		}
	}

	// Load stock to buffer
	for (int i = 0; i < Board->stock->size; i++) {
		add_texture_resource_to_list(buffer, Board->stock->data[i]->resource);
	}

	// Load waste to buffer
	for (int i = 0; i < Board->waste->size; i++) {
		add_texture_resource_to_list(buffer, Board->waste->data[i]->resource);
	}

	// Load hand to buffer
	for (int i = 0; i < Board->hand->size; i++) {
		add_texture_resource_to_list(buffer, Board->hand->data[i]->resource);
	}

	return buffer;
	
}

void deal_cards(Deck* deck) {
	if (Board != NULL) {
		destroy_gameboard(Board);
	}
	Board = init_gameboard();
	int cards_dealt = 0;
	for (int i = 0; i < TABLEAU_COLUMNS; i++) {
		Tableau* current_column = Board->tableau[i];
		Stack* current_stack = current_column->data;
		for (int j = 0; j < i + 1; j++) {
			Card* current_card = deck->data[cards_dealt];
			stack_push(current_stack, current_card);
			Collision collision = get_tableau_collision(i, j);
			current_card->resource->transform->x = collision.hitbox.x;
			current_card->resource->transform->y = collision.hitbox.y;
			cards_dealt++;
		}
	}
	for (int i = cards_dealt; i < DECK_SIZE; i++) {
		Card* current_card = deck->data[i];
		stack_push(Board->stock, current_card);
		current_card->resource->transform->x = STOCK_X;
		current_card->resource->transform->y = UPPER_ROW_Y;
	}
}

Collision get_tableau_collision(int column, int row) {
	return new_collision(TABLEAU_X + ((CARD_WIDTH + TABLEAU_X_INCREMENT) * column), TABLEAU_Y + (TABLEAU_Y_INCREMENT * row), CARD_WIDTH, CARD_HEIGHT);
}

GameBoard* init_gameboard() {
	GameBoard* obj = malloc(sizeof(GameBoard));
	if (obj == NULL) return NULL;
	obj->tableau = init_tableau();
	obj->foundation = init_foundation();
	obj->stock = new_stack(MAX_STOCK_CAPACITY, STOCK);
	obj->stock->hitbox = new_collision(STOCK_X, UPPER_ROW_Y, CARD_WIDTH, CARD_HEIGHT);
	obj->waste = new_stack(MAX_STOCK_CAPACITY, WASTE);
	obj->waste->hitbox = new_collision(WASTE_X, UPPER_ROW_Y, CARD_WIDTH, CARD_HEIGHT);
	obj->hand = new_stack(MAX_STOCK_CAPACITY, HAND);
	obj->hand->hitbox = new_collision(2, 2, 3, 3);
	return obj;
}

Foundation** init_foundation() {
	Foundation** obj = malloc(FOUNDATION_COLUMNS * sizeof(Foundation*));
	if (obj == NULL) return NULL;
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		obj[i] = malloc(sizeof(Foundation));
		if (obj[i] == NULL) {
			for (int j = 0; j < i; j++) {
				destroy_stack(obj[j]->data);
				free(obj[j]);
			}
			free(obj);
			return NULL;
		}
		obj[i]->suit = i;
		obj[i]->data = new_stack(MAX_FOUNDATION_CAPACITY, FOUNDATION);
		obj[i]->hitbox = new_collision(FOUNDATION_X + ((CARD_WIDTH + FOUNDATION_INCREMENT) * i), UPPER_ROW_Y, CARD_WIDTH, CARD_HEIGHT);
	}
	return obj;
}

void destroy_gameboard(GameBoard* obj) {
	destroy_tableau_field(obj->tableau);
	destroy_foundation_field(obj->foundation);
	destroy_stack(obj->stock);
	destroy_stack(obj->waste);
	destroy_stack(obj->hand);
	free(obj);
}

void destroy_foundation_field(Foundation** obj) {
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		destroy_foundation(obj[i]);
	}
	free(obj);
}

void destroy_foundation(Foundation* obj) {
	destroy_stack(obj->data);
	free(obj);
}

Tableau** init_tableau() {
	Tableau** obj = malloc(TABLEAU_COLUMNS * sizeof(Tableau*));
	if (obj == NULL) return NULL;
	for (int i = 0; i < TABLEAU_COLUMNS; i++) {
		obj[i] = malloc(sizeof(Tableau));
		if (obj[i] == NULL) {
			for (int j = 0; j < i; j++) {
				destroy_stack(obj[j]->data);
				free(obj[j]);
			}
			free(obj);
			return NULL;
		}
		obj[i]->index = i;
		obj[i]->data = new_stack(MAX_TABLEAU_CAPACITY, TABLEAU);
		obj[i]->hitbox = new_collision(TABLEAU_X + ((CARD_WIDTH + TABLEAU_X_INCREMENT) * i), TABLEAU_Y, CARD_WIDTH, CARD_HEIGHT);
	}
	return obj;
}

void destroy_tableau_field(Tableau** obj) {
	for (int i = 0; i < TABLEAU_COLUMNS; i++) {
		destroy_tableau(obj[i]);
	}
	free(obj);
}

void destroy_tableau(Tableau* obj) {
	destroy_stack(obj->data);
	free(obj);
}

Stack* new_stack(int capacity, StackType type) {
	Stack* obj = malloc(sizeof(Stack));
	if (obj == NULL) return NULL;
	obj->capacity = capacity;
	obj->size = 0;
	obj->type = type;
	obj->data = calloc(capacity, sizeof(Card*));
	if (obj->data == NULL) {
		free(obj);
		return NULL;
	}
	return obj;
}

void destroy_stack(Stack* obj) {
	free(obj->data);
	free(obj);
}

void split_append_stack(Stack* src, Stack* dest, int from) {
	for (int i = from; i < src->size; i++) {
		Card* current = src->data[i];
		stack_push(dest, current);
	}
	for (int i = src->size -1; i >= from; i--) {
		stack_pop(src);
	}
}

Card* get_card_from_stack_index(Stack* obj, int index) {
	if (index >= obj->size || index < -1) return NULL;
	if (index == -1) return obj->data[obj->size - 1];
	return obj->data[index];
}

int get_index_from_stack_card(Stack* obj, Card* card) {
	if (card == NULL) return -1;
	if (obj->size == 0) return -1;
	for (int i = 0; i < obj->size; i++) {
		Card* current_card = obj->data[i];
		if (current_card == card) return i;
	}
	return -1;
}

Card* stack_pop(Stack* obj) {
	if (obj->size == 0) return NULL;
	Card* item = obj->data[obj->size - 1];
	obj->data[obj->size - 1] = NULL;
	obj->size--;
	return item;
}

void stack_push(Stack* obj, Card* card) {
	if (obj->size == obj->capacity) {
		int old_cap = obj->capacity;
		extend_stack(obj);
		if (old_cap == obj->capacity) return;
	}
	obj->data[obj->size] = card;
	obj->size++;
}

void extend_stack(Stack* obj) {
	Card** old = obj->data;
	int new_cap = obj->capacity * 2;
	Card** extended = realloc(obj->data, new_cap * sizeof(Card*));
	if (extended == NULL) {
		obj->data = old;
		return;
	}
	obj->capacity = new_cap;
	obj->data = extended;
}
