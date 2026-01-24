#include <stdlib.h>
#include "./board.h"
#include "./constants.h"
#include "./card.h"
#include "./commands.h"


CommandQueue* Current_CommandQueue = NULL;

int Init_CommandQueue() {
	if (Current_CommandQueue != NULL) {
		Destroy_CommandQueue();
	}
	Current_CommandQueue = malloc(sizeof(CommandQueue));
	if (Current_CommandQueue == NULL) return FALSE;
	Current_CommandQueue->size = 0;
	Current_CommandQueue->capacity = 50;
	Current_CommandQueue->data = calloc(Current_CommandQueue->capacity, sizeof(Command));
	if (Current_CommandQueue->data == NULL) {
		free(Current_CommandQueue);
		return FALSE;
	}
	return TRUE;
}

void Restart_CurrentBoard() {
	if (Current_CommandQueue == NULL) return;
	if (Current_CommandQueue->size < 1) return;
	int reverse_count = Current_CommandQueue->size;
	for (int i = 0; i < reverse_count; i ++) {
		Reverse_Command();
	}
}

void Destroy_CommandQueue() {
	if (Current_CommandQueue == NULL) return;
	if (Current_CommandQueue->data != NULL) {
		free(Current_CommandQueue->data);
	}
	free(Current_CommandQueue);
	Current_CommandQueue = NULL;
}

void Issue_Command(Commands type, Stack* src, Stack* dest, int src_index) {
	Command command = {type, src, dest, src_index};
	if (is_command_valid(command) == TRUE) {
		command = process_command(command);
		Queue_Add(command);
	} 
}

Command process_command(Command command) {
	switch (command.type) {
		case STOCK_TO_WASTE: {
			Card* pop = Stack_PopCard(command.src);
			pop->is_facedown = FALSE;
			Stack_PushCard(command.dest, pop);
		}
			break;
		case REFILL_STOCK:
			for (int i = command.src->size - 1; i >= 0; i--) {
				Card* pop = Stack_PopCard(command.src);
				pop->is_facedown = TRUE;
				Stack_PushCard(command.dest, pop);
			}
			break;
		case MOVE_CARD: {
			int reverse_index = command.dest->size;
			Stack_SplitAndMerge(command.src, command.dest, command.src_index);
			command.src_index = reverse_index;
			if (command.src->type == TABLEAU && command.src->size > 0) {
				if (command.src->data[command.src->size - 1]->is_facedown == TRUE) {
					Issue_Command(TURN_FACEUP, command.src, NULL, command.src->size - 1);
				}
			}
		}
			break;
		case TURN_FACEUP:
			command.src->data[command.src_index]->is_facedown = FALSE;
			break;
		case NONE:
			break;
	}
	return command;
}

int is_command_valid(Command command) {
	if (command.type == MOVE_CARD) {
		if (command.dest->type == TABLEAU) {
			Card* src_card = command.src->data[command.src_index];
			if (command.dest->size == 0) {
				if (src_card->rank != KING) return FALSE;
				return TRUE;
			}
			Card* dest_card = command.dest->data[command.dest->size - 1];
			Rank desired_rank = dest_card->rank - 1;
			if (desired_rank != src_card->rank) return FALSE;
			if (dest_card->is_card_black == src_card->is_card_black) return FALSE;
		}
		if (command.dest->type == FOUNDATION) {
			int cards_to_move = command.src->size - command.src_index;
			if (cards_to_move > 1) return FALSE;
			Card* src_card = command.src->data[command.src_index];
			if (command.dest->column_index != src_card->suit) return FALSE;
			if (command.dest->size == 0) {
				if (src_card->rank != ACE) return FALSE;
				return TRUE;
			}
			Card* dest_card = command.dest->data[command.dest->size - 1];
			Rank desired_rank = dest_card->rank + 1;
			if (src_card->rank != desired_rank) return FALSE;
		}
	}
	return TRUE;
}

void Reverse_Command() {
	if (Current_CommandQueue == NULL) return;
	if (Current_CommandQueue->data == NULL) return;
	if (Current_CommandQueue->size == 0) return;
	Command reverse = Queue_Pop();
	Command next = Queue_Peek();
	if (next.type == TURN_FACEUP && reverse.type == MOVE_CARD) {
		next.src->data[next.src_index]->is_facedown = TRUE;
		Queue_Pop();
	}
	switch (reverse.type) {
		case STOCK_TO_WASTE: {
			Card* dest_card = Stack_PopCard(reverse.dest);
			Stack_PushCard(reverse.src, dest_card);
			dest_card->is_facedown = TRUE;
			break;
		}
		case REFILL_STOCK:
			for (int i = reverse.dest->size - 1; i >= 0; i--) {
				Card* dest_card = Stack_PopCard(reverse.dest);
				Stack_PushCard(reverse.src, dest_card);
				dest_card->is_facedown = FALSE;
			}
			break;
		case MOVE_CARD:
			Stack_SplitAndMerge(reverse.dest, reverse.src, reverse.src_index);
			break;
		case TURN_FACEUP:
			return;
		case NONE:
			return;
	}

}

void Queue_Add(Command command) {
	if (Current_CommandQueue == NULL) return;
	if (Current_CommandQueue->data == NULL) return;
	if (Current_CommandQueue->capacity == Current_CommandQueue->size) {
		int new_cap = Current_CommandQueue->capacity * 2;
		Command* old_data = Current_CommandQueue->data;
		Current_CommandQueue->data = realloc(Current_CommandQueue->data, new_cap * sizeof(Command));
		if (Current_CommandQueue->data == NULL) {
			Current_CommandQueue->data = old_data;
			return;
		}
		Current_CommandQueue->capacity = new_cap;
	}
	Current_CommandQueue->data[Current_CommandQueue->size] = command;
	Current_CommandQueue->size++;
}

Command Queue_Pop() {
	Command pop = {NONE, NULL, NULL, 0};
	if (Current_CommandQueue == NULL) return pop;
	if (Current_CommandQueue->data == NULL) return pop;
	if (Current_CommandQueue->size == 0) return pop;
	Current_CommandQueue->size--;
	pop = Current_CommandQueue->data[Current_CommandQueue->size];
	return pop;
}

Command Queue_Peek() {
	Command peek = {NONE, NULL, NULL, 0};
	if (Current_CommandQueue == NULL) return peek;
	if (Current_CommandQueue->data == NULL) return peek;
	if (Current_CommandQueue->size == 0) return peek;
	peek = Current_CommandQueue->data[Current_CommandQueue->size - 1];
	return peek;
}
