#include "./card_object.h"

typedef enum StackType {
	TABLEAU,
	FOUNDATION,
	STOCK,
	WASTE
} StackType;

typedef struct Stack {
	StackType type;
	int size;
	int capacity;
	Card** data;
} Stack;

typedef struct GameBoard {
	Stack** tableau;
	Stack** foundation;
	Stack** stock;
	Stack** waste;
} GameBoard;
