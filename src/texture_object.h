#ifndef TEXTURE_TYPES_H
#define TEXTURE_TYPES_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define TEXTURE_RESOURCE_COUNT 1

typedef enum LoadedResources {
	CARD_FACEUP,
} LoadedResources;

typedef struct TextureResource {
	SDL_Texture* image;
	char* image_path;
	SDL_Rect* transform;
	bool queue_free;
} TextureResource;

typedef struct TextureResourceList {
	int capacity;
	int size;
	TextureResource** list;
} TextureResourceList;

TextureResource* new_texture_resource(
	SDL_Renderer* renderer, 
	const char* image_path, 
	int x,
	int y,
	int w,
	int h
);
void init_texture_resources(SDL_Renderer* r);
void render_texture_buffer(SDL_Renderer* r);
void destroy_texture_resources();
void destroy_texture_resource_list(TextureResourceList* obj);
void destroy_texture_asset_list(TextureResourceList* obj);
void free_texture_resource(TextureResource* obj);
void free_texture_asset(TextureResource* obj);
TextureResource* get_texture_resource(LoadedResources type, int x, int y);
TextureResourceList* new_texture_resource_list(int capacity);
void add_texture_resource_to_list(TextureResourceList* obj, TextureResource* res);
void update_buffer(TextureResourceList* obj);
void remove_from_queue(TextureResourceList* obj, TextureResource* texture);
void render_buffer(SDL_Renderer* r, TextureResourceList* buffer);
void destroy_texture_buffer(TextureResourceList* buffer);


#endif
