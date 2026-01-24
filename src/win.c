#include "./constants.h"
#include "./board.h"
#include "./renderer.h"
#include "./win.h"
#include "./vector.h"

int Is_GameCompleted() {
	if (CurrentBoard == NULL) return FALSE;
	if (CurrentBoard->foundation == NULL) return FALSE;
	int completed_count = 0;
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		Stack* current_stack = CurrentBoard->foundation[i];
		if (current_stack == NULL) return FALSE;
		if (current_stack->size == 13) completed_count++;
	}
	if (completed_count == 4) return TRUE;
	return FALSE;
}

RenderBuffer Get_WinRenderBuffer() {
	if (Is_GameCompleted() != TRUE) return (RenderBuffer){0};

	RenderItem* item = calloc(1, sizeof(RenderItem));
	if (item == NULL) return (RenderBuffer){0};
	item->texture = Get_Texture(WIN);
	item->position = New_Vector2(WIN_X, WIN_Y);
	return (RenderBuffer){1, 1, item};
}

void Debug_Win() {
	if (CurrentBoard == NULL) return;
	if (CurrentBoard->foundation == NULL) return;
	for (int i = 0; i < FOUNDATION_COLUMNS; i++) {
		Stack* current_stack = CurrentBoard->foundation[i];
		if (current_stack == NULL) return;
		current_stack->size = 13;
	}
}

