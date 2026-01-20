#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./constants.h"
#include "./vector.h"


typedef enum TextureName {
	CARD_BACK,
	AH,
	H2,
	H3,
	H4,
	H5,
	H6,
	H7,
	H8,
	H9,
	H10,
	JH,
	QH,
	KH,
	AD,
	D2,
	D3,
	D4,
	D5,
	D6,
	D7,
	D8,
	D9,
	D10,
	JD,
	QD,
	KD,
	AS,
	S2,
	S3,
	S4,
	S5,
	S6,
	S7,
	S8,
	S9,
	S10,
	JS,
	QS,
	KS,
	AC,
	C2,
	C3,
	C4,
	C5,
	C6,
	C7,
	C8,
	C9,
	C10,
	JC,
	QC,
	KC,
	TEXTURE_NAME_COUNT
} TextureName;

typedef struct Texture {
	TextureName name;
	SDL_Texture* image;
	Vector2 size;
} Texture;

typedef struct TextureIndex {
	int size;
	int capacity;
	Texture** data;
} TextureIndex;

typedef struct RenderItem {
	Texture* texture;
	Vector2 position;
} RenderItem;

typedef struct RenderBuffer {
	int size;
	int capacity;
	RenderItem* data;
} RenderBuffer;

extern TextureIndex* LoadedTextures;

int Init_Textures(SDL_Renderer* r);
Texture* Get_Texture(TextureName name);
void RenderItems(SDL_Renderer* r);
void Destroy_Textures();
Texture* load_texture(SDL_Renderer* e, TextureName name, int width, int height, char* path);

#endif
