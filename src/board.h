#ifndef BOARD_H
#define BOARD_H

#include <stdlib.h>
#include "./vector.h"
#include "./card.h"
#include "./constants.h"
#include "./renderer.h"


typedef enum StackType {
	TABLEAU,
	FOUNDATION,
	STOCK,
	WASTE,
	HAND
} StackType;

typedef struct Stack {
	StackType type;
	Vector2 position;
	Vector2 hitbox_size;
	int column_index;
	int size;
	int capacity;
	Card** data;
} Stack;

typedef struct Board {
	Stack** tableau;
	Stack** foundation;
	Stack* stock;
	Stack* waste;
	Stack* hand;
} Board;

extern Board* CurrentBoard;

RenderBuffer Get_BoardRenderBuffer();
int Init_Board();
void Destroy_Board();
Stack* new_stack(int capacity, Vector2 position, Vector2 hitbox_size, StackType type, int column_index);
void set_card_position(Stack* stack, Card* card);
void Stack_PushCard(Stack* dest, Card* card);
Card* Stack_PopCard(Stack* src);
Card* Stack_PeekCard(Stack* src);
Card* Stack_PeekIndex(Stack* src, int index);
int Stack_GetCardIndex(Stack* src, Card* card);
void Stack_SplitAndMerge(Stack* src, Stack* dest, int split_index);
void destroy_stack(Stack* src);

#endif
