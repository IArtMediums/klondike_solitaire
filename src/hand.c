#include <SDL2/SDL.h>
#include "./game_board.h"
#include "./collision.h"
#include "./card_object.h"
#include "./hand.h"

Vector2 mouse_down_offset = {0, 0};

void hand_update() {
	if (Board == NULL) return;
	if (Board->hand->size > 0) {
		update_card_position(Board->hand);
	}
}

void hand_down(int x, int y) {
	if (Board == NULL) return;
	Collision collision = new_collision(x, y, 2, 2);
	CollisionData data = collide_with_board(Board, collision);
	issue_command(data, x, y);
}

void update_card_position(Stack* items) {
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_x -= mouse_down_offset.x;
	mouse_y -= mouse_down_offset.y;
	// TODO: implement card movement logic here
}

void issue_command(CollisionData data, int x, int y) {
	switch (data.object_type) {
		case OBJ_TABLEAU:
			return;
		case OBJ_FOUNDATION:
			if (data.object_data.d_foundation->data->size == 0) return;
			// move top card from foundation column x to hand
			set_mouse_down_offset(x, data.object_data.d_foundation->hitbox.hitbox.x, y, data.object_data.d_foundation->hitbox.hitbox.y);
			break;
		case OBJ_STOCK:
			if (data.object_data.d_stack->size == 0 && Board->waste->size != 0) {
				// restock
				return;
			}
			if (data.object_data.d_stack->size > 0) {
				// move top card from stock to waste
				return;
			}
			break;
		case OBJ_WASTE:
			if (data.object_data.d_stack->size == 0) {
				// move top card from stock to waste
				return;
			}
			// move top card from waste to hand
			set_mouse_down_offset(x, data.object_data.d_stack->hitbox.hitbox.x, y, data.object_data.d_stack->hitbox.hitbox.y);
			break;
		case OBJ_CARD:
			// move card/s from tableau column x row y to hand
			set_mouse_down_offset(x, data.object_data.d_card->resource->transform->x, y, data.object_data.d_card->resource->transform->y);
			break;
		case OBJ_NONE:
			break;
		default:
			return;
	}
}

void set_mouse_down_offset(int x1, int x2, int y1, int y2) {
	mouse_down_offset.x = x1 - x2;
	mouse_down_offset.y = y1 - y2;
}
