#ifndef CARD_OBJECT_H
#define CARD_OBJECT_H

#include <stdbool.h>
#include "./texture_object.h"

#define DECK_SIZE 52

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

typedef enum Suit {
	CLUBS,
	DIAMONDS,
	HEARTS,
	SPADES,
	SUIT_COUNT
} Suit;

typedef struct Card {
	TextureResource* resource;
	Rank rank;
	Suit suit;
	bool black;
	bool faceup;
} Card;

typedef struct Deck {
	Card** data;
} Deck;

Deck* new_deck();
bool is_card_black(Suit suit);
TextureResource* get_card_texture(Suit suit, Rank rank, int x, int y);
void shuffle(Deck* deck);
void swap_cards(Deck* deck, int a, int b);
int randint(int max_value);
void free_deck(Deck* deck);

#endif
