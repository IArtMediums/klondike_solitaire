#ifndef HAND_H
#define HAND_H

#include <SDL2/SDL.h>
#include "./vector.h"
#include "./constants.h"
#include "./hand.h"
#include "./board.h"

typedef struct Collision {
	int is_colliding;
	Stack* src;
	int index;
} Collision;

extern Vector2* HandPosition;

void Update_Hand();
void On_MouseButtonDown();
Collision Get_Collision();
int is_colliding(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2);
void update_card_positions();
int Init_Hand();
void Destroy_Hand();

#endif
