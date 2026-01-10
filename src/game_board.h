#include "./card_object.h"
#define TABLEAU_COLUMNS 7
#define MAX_TABLEAU_CAPACITY 52
#define FOUNDATION_COLUMNS 4
#define MAX_FOUNDATION_CAPACITY 13
#define MAX_STOCK_CAPACITY 24

typedef enum StackType {
	TABLEAU,
	FOUNDATION,
	STOCK,
	WASTE,
	HAND
} StackType;

typedef struct Stack {
	StackType type;
	int size;
	int capacity;
	Card** data;
} Stack;

typedef struct Tableau {
	int index;
	Stack* data;
} Tableau;

typedef struct Foundation {
	Suit suit;
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
