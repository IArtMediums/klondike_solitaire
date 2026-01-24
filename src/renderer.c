#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./renderer.h"
#include "./vector.h"
#include "./win.h"
#include "./board.h"

TextureIndex* LoadedTextures = NULL;


int Init_Textures(SDL_Renderer* r) {
	if (LoadedTextures != NULL) return FALSE;
	LoadedTextures = malloc(sizeof(TextureIndex));
	if (LoadedTextures == NULL) return FALSE;
	LoadedTextures->size = TEXTURE_NAME_COUNT;
	LoadedTextures->capacity = TEXTURE_NAME_COUNT;
	LoadedTextures->data = calloc(TEXTURE_NAME_COUNT, sizeof(Texture*));
	if (LoadedTextures->data == NULL) {
		free(LoadedTextures);
		return FALSE;
	}
	LoadedTextures->data[CARD_BACK] = load_texture(r, CARD_BACK, CARD_WIDTH, CARD_HEIGHT, "textures/back.png");
	// Load Hearts (AH through KH)
	LoadedTextures->data[AH] = load_texture(r, AH, CARD_WIDTH, CARD_HEIGHT, "textures/ah.png");
	LoadedTextures->data[AH+1] = load_texture(r, AH+1, CARD_WIDTH, CARD_HEIGHT, "textures/2h.png");
	LoadedTextures->data[AH+2] = load_texture(r, AH+2, CARD_WIDTH, CARD_HEIGHT, "textures/3h.png");
	LoadedTextures->data[AH+3] = load_texture(r, AH+3, CARD_WIDTH, CARD_HEIGHT, "textures/4h.png");
	LoadedTextures->data[AH+4] = load_texture(r, AH+4, CARD_WIDTH, CARD_HEIGHT, "textures/5h.png");
	LoadedTextures->data[AH+5] = load_texture(r, AH+5, CARD_WIDTH, CARD_HEIGHT, "textures/6h.png");
	LoadedTextures->data[AH+6] = load_texture(r, AH+6, CARD_WIDTH, CARD_HEIGHT, "textures/7h.png");
	LoadedTextures->data[AH+7] = load_texture(r, AH+7, CARD_WIDTH, CARD_HEIGHT, "textures/8h.png");
	LoadedTextures->data[AH+8] = load_texture(r, AH+8, CARD_WIDTH, CARD_HEIGHT, "textures/9h.png");
	LoadedTextures->data[AH+9] = load_texture(r, AH+9, CARD_WIDTH, CARD_HEIGHT, "textures/10h.png");
	LoadedTextures->data[AH+10] = load_texture(r, AH+10, CARD_WIDTH, CARD_HEIGHT, "textures/jh.png");
	LoadedTextures->data[AH+11] = load_texture(r, AH+11, CARD_WIDTH, CARD_HEIGHT, "textures/qh.png");
	LoadedTextures->data[AH+12] = load_texture(r, AH+12, CARD_WIDTH, CARD_HEIGHT, "textures/kh.png");
	
	// Load Diamonds (AD through KD)
	LoadedTextures->data[AD] = load_texture(r, AD, CARD_WIDTH, CARD_HEIGHT, "textures/ad.png");
	LoadedTextures->data[AD+1] = load_texture(r, AD+1, CARD_WIDTH, CARD_HEIGHT, "textures/2d.png");
	LoadedTextures->data[AD+2] = load_texture(r, AD+2, CARD_WIDTH, CARD_HEIGHT, "textures/3d.png");
	LoadedTextures->data[AD+3] = load_texture(r, AD+3, CARD_WIDTH, CARD_HEIGHT, "textures/4d.png");
	LoadedTextures->data[AD+4] = load_texture(r, AD+4, CARD_WIDTH, CARD_HEIGHT, "textures/5d.png");
	LoadedTextures->data[AD+5] = load_texture(r, AD+5, CARD_WIDTH, CARD_HEIGHT, "textures/6d.png");
	LoadedTextures->data[AD+6] = load_texture(r, AD+6, CARD_WIDTH, CARD_HEIGHT, "textures/7d.png");
	LoadedTextures->data[AD+7] = load_texture(r, AD+7, CARD_WIDTH, CARD_HEIGHT, "textures/8d.png");
	LoadedTextures->data[AD+8] = load_texture(r, AD+8, CARD_WIDTH, CARD_HEIGHT, "textures/9d.png");
	LoadedTextures->data[AD+9] = load_texture(r, AD+9, CARD_WIDTH, CARD_HEIGHT, "textures/10d.png");
	LoadedTextures->data[AD+10] = load_texture(r, AD+10, CARD_WIDTH, CARD_HEIGHT, "textures/jd.png");
	LoadedTextures->data[AD+11] = load_texture(r, AD+11, CARD_WIDTH, CARD_HEIGHT, "textures/qd.png");
	LoadedTextures->data[AD+12] = load_texture(r, AD+12, CARD_WIDTH, CARD_HEIGHT, "textures/kd.png");
	
	// Load Spades (AS through KS)
	LoadedTextures->data[AS] = load_texture(r, AS, CARD_WIDTH, CARD_HEIGHT, "textures/as.png");
	LoadedTextures->data[AS+1] = load_texture(r, AS+1, CARD_WIDTH, CARD_HEIGHT, "textures/2s.png");
	LoadedTextures->data[AS+2] = load_texture(r, AS+2, CARD_WIDTH, CARD_HEIGHT, "textures/3s.png");
	LoadedTextures->data[AS+3] = load_texture(r, AS+3, CARD_WIDTH, CARD_HEIGHT, "textures/4s.png");
	LoadedTextures->data[AS+4] = load_texture(r, AS+4, CARD_WIDTH, CARD_HEIGHT, "textures/5s.png");
	LoadedTextures->data[AS+5] = load_texture(r, AS+5, CARD_WIDTH, CARD_HEIGHT, "textures/6s.png");
	LoadedTextures->data[AS+6] = load_texture(r, AS+6, CARD_WIDTH, CARD_HEIGHT, "textures/7s.png");
	LoadedTextures->data[AS+7] = load_texture(r, AS+7, CARD_WIDTH, CARD_HEIGHT, "textures/8s.png");
	LoadedTextures->data[AS+8] = load_texture(r, AS+8, CARD_WIDTH, CARD_HEIGHT, "textures/9s.png");
	LoadedTextures->data[AS+9] = load_texture(r, AS+9, CARD_WIDTH, CARD_HEIGHT, "textures/10s.png");
	LoadedTextures->data[AS+10] = load_texture(r, AS+10, CARD_WIDTH, CARD_HEIGHT, "textures/js.png");
	LoadedTextures->data[AS+11] = load_texture(r, AS+11, CARD_WIDTH, CARD_HEIGHT, "textures/qs.png");
	LoadedTextures->data[AS+12] = load_texture(r, AS+12, CARD_WIDTH, CARD_HEIGHT, "textures/ks.png");
	
	// Load Clubs (AC through KC)
	LoadedTextures->data[AC] = load_texture(r, AC, CARD_WIDTH, CARD_HEIGHT, "textures/ac.png");
	LoadedTextures->data[AC+1] = load_texture(r, AC+1, CARD_WIDTH, CARD_HEIGHT, "textures/2c.png");
	LoadedTextures->data[AC+2] = load_texture(r, AC+2, CARD_WIDTH, CARD_HEIGHT, "textures/3c.png");
	LoadedTextures->data[AC+3] = load_texture(r, AC+3, CARD_WIDTH, CARD_HEIGHT, "textures/4c.png");
	LoadedTextures->data[AC+4] = load_texture(r, AC+4, CARD_WIDTH, CARD_HEIGHT, "textures/5c.png");
	LoadedTextures->data[AC+5] = load_texture(r, AC+5, CARD_WIDTH, CARD_HEIGHT, "textures/6c.png");
	LoadedTextures->data[AC+6] = load_texture(r, AC+6, CARD_WIDTH, CARD_HEIGHT, "textures/7c.png");
	LoadedTextures->data[AC+7] = load_texture(r, AC+7, CARD_WIDTH, CARD_HEIGHT, "textures/8c.png");
	LoadedTextures->data[AC+8] = load_texture(r, AC+8, CARD_WIDTH, CARD_HEIGHT, "textures/9c.png");
	LoadedTextures->data[AC+9] = load_texture(r, AC+9, CARD_WIDTH, CARD_HEIGHT, "textures/10c.png");
	LoadedTextures->data[AC+10] = load_texture(r, AC+10, CARD_WIDTH, CARD_HEIGHT, "textures/jc.png");
	LoadedTextures->data[AC+11] = load_texture(r, AC+11, CARD_WIDTH, CARD_HEIGHT, "textures/qc.png");
	LoadedTextures->data[AC+12] = load_texture(r, AC+12, CARD_WIDTH, CARD_HEIGHT, "textures/kc.png");

	// Load Win screen
	LoadedTextures->data[WIN] = load_texture(r, WIN, WIN_WIDTH, WIN_HEIGHT, "textures/win.png");

	// Load Board Backgroud
	LoadedTextures->data[BOARD] = load_texture(r, BOARD, WINDOW_WIDTH, WINDOW_HEIGHT, "textures/board.png");

	return TRUE;
}

Texture* Get_Texture(TextureName name) {
	return LoadedTextures->data[name];
}

void RenderItems(SDL_Renderer* r) {
	// Render Backgroud
	Texture* bg = Get_Texture(BOARD);
	SDL_Rect bg_transform = {0, 0, bg->size.x, bg->size.y};
	SDL_RenderCopy(r, bg->image, NULL, &bg_transform);
	// Add Card buffer getter
	RenderBuffer cards = Get_BoardRenderBuffer();
	if (cards.size == 0) {
		if (cards.data != NULL) free(cards.data);
		return;
	}
	for (int i = 0; i < cards.size; i++) {
		RenderItem item = cards.data[i];
		SDL_Rect transform;
		transform.x = item.position.x;
		transform.y = item.position.y;
		transform.w = item.texture->size.x;
		transform.h = item.texture->size.y;
		SDL_RenderCopy(r, item.texture->image, NULL, &transform);
	}
	free(cards.data);
	// Add Win buffer getter
	RenderBuffer win_render = Get_WinRenderBuffer();
	if (win_render.size == 0) {
		if (win_render.data != NULL) free(win_render.data);
		return;
	}
	for (int i = 0; i < win_render.size; i++) {
		RenderItem item = win_render.data[i];
		SDL_Rect transform;
		transform.x = item.position.x;
		transform.y = item.position.y;
		transform.w = item.texture->size.x;
		transform.h = item.texture->size.y;
		SDL_RenderCopy(r, item.texture->image, NULL, &transform);
	}
	free(win_render.data);
}

void Destroy_Textures() {
	for (int i = 0; i < LoadedTextures->size; i++) {
		Texture* t = LoadedTextures->data[i];
		SDL_DestroyTexture(t->image);
		free(t);
	}
	free(LoadedTextures->data);
	free(LoadedTextures);
}

Texture* load_texture(SDL_Renderer* r, TextureName name, int width, int height, char* path) {
	Texture* texture = malloc(sizeof(Texture));
	if (texture == NULL) return NULL;
	texture->image = IMG_LoadTexture(r, path);
	if (texture->image == NULL) {
		free(texture);
		return NULL;
	}
	texture->size = New_Vector2(width, height);
	texture->name = name;
	return texture;
}
