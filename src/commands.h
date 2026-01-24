#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdlib.h>
#include "./board.h"
#include "./constants.h"
#include "./card.h"

typedef enum Commands {
	STOCK_TO_WASTE,
	REFILL_STOCK,
	MOVE_CARD,
	TURN_FACEUP,
	NONE
} Commands;

typedef struct Command {
	Commands type;
	Stack* src;
	Stack* dest;
	int src_index;
} Command;

typedef struct CommandQueue {
	int size;
	int capacity;
	Command* data;
} CommandQueue;

extern CommandQueue* Current_CommandQueue;

int Init_CommandQueue();
void Destroy_CommandQueue();
void Issue_Command(Commands type, Stack* src, Stack* dest, int src_index);
Command process_command(Command command);
int is_command_valid(Command command);
void Reverse_Command();
void Queue_Add(Command command);
Command Queue_Pop();
Command Queue_Peek();
void Restart_CurrentBoard();

#endif
