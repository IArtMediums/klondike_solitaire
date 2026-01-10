#include <SDL2/SDL.h>
#include <stdbool.h>
#include "./collision.h"
#include "./game_board.h"
#include "./card_object.h"

CollisionData collide_with_board(GameBoard* board, Collision event) {
	CollisionData data;
	data.stack_index = -1;
	data.column_index = -1;
	// Scan static locations first
	if (collides_with_collision(event, board->stock->hitbox)) {
		data.object_type = OBJ_STOCK;
		data.object_data.d_stack = board->stock;
		return data;
	}
	if (collides_with_collision(event, board->waste->hitbox)) {
		data.object_type = OBJ_WASTE;
		data.object_data.d_stack = board->waste;
		return data;
	}
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		Foundation* f = board->foundation[i];
		if (collides_with_collision(event, f->hitbox)) {
			data.object_type = OBJ_FOUNDATION;
			data.object_data.d_foundation = f;
			data.column_index = i;
			return data;
		}
	}
	// scan tableau dynamic stack
	for (int i = 0; i < TABLEAU_COLUMNS; i++) {
		Tableau* t = board->tableau[i];
		Stack* ts = t->data;
		for (int j = ts->size - 1; j > -1; j--) {
			Card* c = ts->data[j];
			Collision cc = new_collision_from_transform(c->resource->transform);
			if (collides_with_collision(event, cc)) {
				data.object_type = OBJ_CARD;
				data.object_data.d_card = c;
				data.stack_index = j;
				data.column_index = i;
				return data;
			}
		}
		if (collides_with_collision(event, t->hitbox)) {
			data.object_type = OBJ_TABLEAU;
			data.object_data.d_tableau = t;
			data.column_index = i;
			return data;
		}
	}
	data.object_type = OBJ_NONE;
	return data;
}

Collision new_collision_from_transform(SDL_Rect* transform) {
	Collision c;
	c.hitbox.x = transform->x;
	c.hitbox.y = transform->y;
	c.hitbox.w = transform->w;
	c.hitbox.h = transform->h;
	c.center_pos = get_collision_center_position(c);
	return c;
}

Collision new_collision(int x, int y, int w, int h) {
	Collision obj;
	obj.hitbox.x = x;
	obj.hitbox.y = y;
	obj.hitbox.w = w;
	obj.hitbox.h = h;
	obj.center_pos = get_collision_center_position(obj);
	return obj;
}

Vector2 get_collision_center_position(Collision obj) {
	Vector2 center;
	center.x = obj.hitbox.x + (obj.hitbox.w / 2);
	center.y = obj.hitbox.y + (obj.hitbox.h / 2);
	return center;
}

bool collides_with_point(Collision obj, Vector2 point) {
	int lx = obj.hitbox.x;
	int rx = obj.hitbox.x + obj.hitbox.w;
	int uy = obj.hitbox.y;
	int dy = obj.hitbox.y + obj.hitbox.h;
	bool is_between_x = point.x >= lx && point.x <= rx;
	bool is_between_y = point.y >= uy && point.y <= dy;
	return is_between_y && is_between_x;
}

bool collides_with_collision(Collision a, Collision b) {
	return (a.hitbox.x < b.hitbox.x + b.hitbox.w &&
			a.hitbox.x + a.hitbox.w > b.hitbox.x &&
			a.hitbox.y < b.hitbox.y + b.hitbox.h &&
			a.hitbox.y + a.hitbox.h > b.hitbox.y);
}
