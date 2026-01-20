#ifndef CARD_H
#define CARD_H

#include <stdlib.h>
#include "./constants.h"
#include "./vector.h"
#include "./renderer.h"


typedef enum Suit {
	HEARTS,
	DIAMONDS,
	CLUBS,
	SPADES,
	SUIT_COUNT
} Suit;

typedef enum Rank {
	ACE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	TEN,
	JACK,
	QUEEN,
	KING,
	RANK_COUNT
} Rank;

typedef struct Card {
	Texture* front_texture;
	Texture* back_texture;
	Vector2 position;
	int is_facedown;
	Suit suit;
	Rank rank;
	int is_card_black;
} Card;

typedef struct Deck {
	int size;
	int capacity;
	Card** data;
} Deck;

extern Deck* CurrentDeck;

int Init_Deck();
void Destroy_Deck();
void deck_shuffle(Deck* deck);
int randint(int max_value);
Card* new_card(Suit suit, Rank rank);
RenderItem Get_CardRenderItem(Card* card);

#endif
