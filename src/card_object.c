#include <stdbool.h>
#include <stdlib.h>
#include "./texture_object.h"
#include "./card_object.h"

Deck* new_deck(){
	Deck* d = malloc(sizeof(Deck));
	if (d == NULL) return NULL;
	d->data = calloc(DECK_SIZE, sizeof(Card*));
	if (d->data == NULL) {
		free(d);
		return NULL;
	}
	for (int i = 0; i < DECK_SIZE; i++) {
		d->data[i] = malloc(sizeof(Card));
		if (d->data[i] == NULL) {
			for (int j = 0; j < i; j++) {
				free(d->data[j]);
			}
			free(d->data);
			free(d);
			return NULL;
		}
	}
	int card_count = 0;
	for (int s = 0; s < SUIT_COUNT; s++) {
		for (int r = 0; r < RANK_COUNT; r++) {
			Card* c = d->data[card_count];
			c->rank = r;
			c->suit = s;
			c->black = is_card_black(s);
			c->faceup = true;
			c->resource = get_card_texture(s, r, s * 100, r * 100);
			card_count++;
		}
	}
	shuffle(d);
	return d;
}

bool is_card_black(Suit suit) {
	return suit == SPADES || suit == CLUBS;
}

TextureResource* get_card_texture(Suit suit, Rank rank, int x, int y) {
	int texture_index;
	
	// Calculate base index for each suit (13 cards per suit)
	int suit_offset;
	switch(suit) {
		case HEARTS:   suit_offset = AH;  break;  // AH = 1
		case DIAMONDS: suit_offset = AD;  break;  // AD = 14
		case SPADES:   suit_offset = AS;  break;  // AS = 27
		case CLUBS:    suit_offset = AC;  break;  // AC = 40
		default:       suit_offset = AH;  break;
	}
	
	// Add rank offset (assuming rank enum goes ACE=0, TWO=1, ..., KING=12)
	texture_index = suit_offset + rank;
	
	return get_texture_resource(texture_index, x, y);
}
void shuffle(Deck* deck) {
	for (int i = 0; i < DECK_SIZE; i++) {
		int a = randint(DECK_SIZE - 1);
		int b = randint(DECK_SIZE - 1);
		swap_cards(deck, a, b);
	}
}

void swap_cards(Deck* deck, int a, int b) {
	Card* temp = deck->data[a];
	deck->data[a] = deck->data[b];
	deck->data[b] = temp;
}

int randint(int max_value) {
	// gives rand number between 0 and max_value inclusive
	return rand() % (max_value + 1);
}

void free_deck(Deck* deck) {
	for (int i = 0; i < DECK_SIZE; i++) {
		deck->data[i]->resource->queue_free = true;
		free(deck->data[i]);
	}
	free(deck->data);
	free(deck);
}


