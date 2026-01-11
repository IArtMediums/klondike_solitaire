#ifndef HAND_H
#define HAND_H

#include "./game_board.h"
#include "./collision.h"

void hand_update(void);
void hand_down(int x, int y);
void update_card_position(Stack* items);
void issue_command(CollisionData data, int x, int y);
void set_mouse_down_offset(int x1, int x2, int y1, int y2);

#endif // HAND_H
