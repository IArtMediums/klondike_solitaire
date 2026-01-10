#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>
#include <stdbool.h>

struct Card;
struct Tableau;
struct Foundation;
struct Stack;
struct GameBoard;

typedef enum ObjectType {
	OBJ_NONE,
	OBJ_TABLEAU,
	OBJ_FOUNDATION,
	OBJ_STOCK,
	OBJ_WASTE,
	OBJ_CARD
} ObjectType;

typedef struct Vector2 {
	int x, y;
} Vector2;

typedef struct Collision {
	SDL_Rect hitbox;
	Vector2 center_pos;
} Collision;

typedef union ObjectData {
	struct Tableau* d_tableau;
	struct Foundation* d_foundation;
	struct Stack* d_stack;
	struct Card* d_card;
}ObjectData;

typedef struct CollisionData {
	ObjectType object_type;
	ObjectData object_data;
	int stack_index;
	int column_index;
} CollisionData;

Vector2 get_collision_center_position(Collision obj);
Collision new_collision_from_transform(SDL_Rect* transform);
Collision new_collision(int x, int y, int w, int h);
bool collides_with_point(Collision obj,Vector2 point);
bool collides_with_collision(Collision a, Collision b);
CollisionData collide_with_board(struct GameBoard* board, Collision event);

#endif
