#include <stdlib.h>
#include "./constants.h"
#include "./vector.h"
#include "./renderer.h"
#include "./card.h"

Deck* CurrentDeck = NULL;

int Init_Deck() {
	if (CurrentDeck != NULL) {
		Destroy_Deck();
	}
	Deck* d = malloc(sizeof(Deck));
	if (d == NULL) return FALSE;
	d->size = DECK_SIZE;
	d->capacity = DECK_SIZE;
	d->data = calloc(d->size, sizeof(Card*));
	if (d->data == NULL) {
		free(d);
		return FALSE;
	}
	int card_count = 0;
	for (int suit = 0; suit < SUIT_COUNT; suit++) {
		for (int rank = 0; rank < RANK_COUNT; rank++) {
			d->data[card_count] = new_card(suit, rank);
			if (d->data[card_count] == NULL) {
				for (int i = 0; i < card_count; i++) {
					free(d->data[i]);
				}
				free(d->data);
				free(d);
				return FALSE;
			}
			card_count++;
		}
	}
	deck_shuffle(d);
	CurrentDeck = d;
	return TRUE;
}

void Destroy_Deck() {
	if (CurrentDeck == NULL) return;
	for (int i = 0; i < CurrentDeck->size; i++) {
		free(CurrentDeck->data[i]);
	}
	free(CurrentDeck->data);
	free(CurrentDeck);
	CurrentDeck = NULL;
}

void deck_shuffle(Deck* deck) {
	for (int i = deck->size - 1; i > 0; i--) {
		int j = randint(i);
		Card* temp = deck->data[i];
		deck->data[i] = deck->data[j];
		deck->data[j] = temp;
	}
}

int randint(int max_value) {
	return rand() % (max_value + 1);
}


Card* new_card(Suit suit, Rank rank) {
	Card* card = malloc(sizeof(Card));
	if (card == NULL) return NULL;

	int suit_offset;
	switch (suit) {
		case HEARTS:	suit_offset = AH; break;
		case DIAMONDS:	suit_offset = AD; break;
		case SPADES:	suit_offset = AS; break;
		case CLUBS:		suit_offset = AC; break;
		default:		suit_offset = AH; break;
	}
	card->front_texture = Get_Texture(suit_offset + rank);
	card->back_texture = Get_Texture(CARD_BACK);
	card->position = New_Vector2(0, 0);
	card->is_facedown = TRUE;
	card->suit = suit;
	card->rank = rank;
	card->is_card_black = TRUE;
	if (suit == HEARTS || suit == DIAMONDS) {
		card->is_card_black = FALSE;
	}
	return card;
}

RenderItem Get_CardRenderItem(Card* card) {
	RenderItem ri = {0};
	if (card == NULL) return ri;
	ri.texture = card->front_texture;
	if (card->is_facedown) {
		ri.texture = card->back_texture;
	}
	ri.position = card->position;
	return ri;
}
