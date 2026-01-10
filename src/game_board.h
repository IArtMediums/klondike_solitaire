#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "./card_object.h"
#include "./collision.h"

#define TABLEAU_COLUMNS 7
#define MAX_TABLEAU_CAPACITY 52
#define FOUNDATION_COLUMNS 4
#define MAX_FOUNDATION_CAPACITY 13
#define MAX_STOCK_CAPACITY 24
#define STACK_OFFSET 3

#define STOCK_X 74
#define WASTE_X 172
#define FOUNDATION_X 383
#define FOUNDATION_INCREMENT 34
#define UPPER_ROW_Y 44
#define TABLEAU_X 74
#define TABLEAU_X_INCREMENT 34
#define TABLEAU_Y 204
#define TABLEAU_Y_INCREMENT 30

typedef enum StackType {
	TABLEAU,
	FOUNDATION,
	STOCK,
	WASTE,
	HAND,
	CARD,
	NONE
} StackType;

typedef struct Stack {
	StackType type;
	Collision hitbox;
	int size;
	int capacity;
	Card** data;
} Stack;

typedef struct Tableau {
	int index;
	Collision hitbox;
	Stack* data;
} Tableau;

typedef struct Foundation {
	Suit suit;
	Collision hitbox;
	Stack* data;
} Foundation;

typedef struct GameBoard {
	Tableau** tableau;
	Foundation** foundation;
	Stack* stock;
	Stack* waste;
	Stack* hand;
} GameBoard;

extern GameBoard* Board;

Stack* new_stack(int capacity, StackType type);
void destroy_stack(Stack* obj);
void split_append_stack(Stack* src, Stack* dest, int from);
Card* stack_pop(Stack* obj);
Card* get_card_from_stack_index(Stack* obj, int index);
int get_index_from_stack_card(Stack* obj, Card* card);
void stack_push(Stack* obj, Card* card);
void extend_stack(Stack* obj);
Tableau** init_tableau();
Foundation** init_foundation();
GameBoard* init_gameboard();
void destroy_tableau(Tableau* obj);
void destroy_tableau_field(Tableau** obj);
void destroy_foundation(Foundation* obj);
void destroy_foundation_field(Foundation** obj);
void destroy_gameboard(GameBoard* obj);
void deal_cards(Deck* deck);
TextureResourceList* get_card_render_buffer();
Collision get_tableau_collision(int column, int row);

#endif
