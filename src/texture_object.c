#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "texture_object.h"
#include "constants.h"

TextureResourceList* GTextureResources = NULL;
TextureResourceList* GTextureRenderBuffer = NULL;
TextureResource* new_texture_resource(
	SDL_Renderer* renderer,
	const char* image_path,
	int x,
	int y,
	int w,
	int h
) {
	TextureResource* texture = malloc(sizeof(TextureResource));
	if (texture == NULL) {
		return NULL;
	}
	char* path = malloc(strlen(image_path) + 1);
	if (path == NULL) {
		free(texture);
		return NULL;
	}
	strcpy(path, image_path);
	texture->image_path = path;
	texture->image = IMG_LoadTexture(renderer, texture->image_path);
	if (texture->image == NULL) {
		free(path);
		free(texture);
		return NULL;
	}
	texture->transform = malloc(sizeof(SDL_Rect));
	if (texture->transform == NULL) return NULL;
	texture->transform->x = x;
	texture->transform->y = y;
	texture->transform->w = w;
	texture->transform->h = h;
	return texture;
}

void free_texture_asset(TextureResource* obj) {
	SDL_DestroyTexture(obj->image);
	free(obj->image_path);
	free(obj->transform);
	free(obj);
}

void free_texture_resource(TextureResource* obj) {
	free(obj->transform);
	free(obj);
}

void init_texture_resources(SDL_Renderer* r) {
	GTextureResources = new_texture_resource_list(TEXTURE_RESOURCE_COUNT);
	GTextureRenderBuffer = new_texture_resource_list(8);
	if (GTextureResources == NULL) {
		return;
	}
	GTextureResources->list[CARD_FACEUP] = new_texture_resource(r, "textures/test.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 64, 64);

}

TextureResource* get_texture_resource(SDL_Renderer* r, LoadedResources type, int x, int y) {
	TextureResource* new = malloc(sizeof(TextureResource));
	if (new == NULL) return NULL;
	TextureResource* src = GTextureResources->list[type];
	new->image = src->image;
	new->image_path = src->image_path;
	new->transform = malloc(sizeof(SDL_Rect));
	if (new->transform == NULL) return NULL;
	new->transform->x = x;
	new->transform->y = y;
	new->transform->w = src->transform->w;
	new->transform->h = src->transform->h;
	add_texture_resource_to_list(GTextureRenderBuffer, new);
	return new;
}

void add_texture_resource_to_list(TextureResourceList* obj, TextureResource* res) {
	if (obj == NULL) {
		return;
	}
	if (obj->capacity == obj->size) {
		int new_cap = obj->capacity * 2;
		TextureResource** old_p = obj->list;
		obj->list = realloc(obj->list, new_cap * sizeof(TextureResource*));
		if (obj->list == NULL) {
			obj->list = old_p;
			return;
		}
		obj->capacity = new_cap;
	}
	obj->list[obj->size] = res;
	obj->size++;
}

TextureResourceList* new_texture_resource_list(int capacity) {
	TextureResourceList* obj = malloc(sizeof(TextureResourceList));
	if (obj == NULL) return NULL;
	obj->list = calloc(capacity, sizeof(TextureResource*));
	if (obj->list == NULL) {
		free(obj);
		return NULL;
	}
	obj->size = 0;
	obj->capacity = capacity;
	return obj;
}

void render_texture_buffer(SDL_Renderer* r) {
	if (!GTextureRenderBuffer) return;
	if (GTextureRenderBuffer->size == 0) return;

	for (int i = 0; i < GTextureRenderBuffer->size; i++) {
		TextureResource* t = GTextureRenderBuffer->list[i];
		SDL_RenderCopy(r, t->image, NULL, t->transform);
	}
}

void destroy_texture_resources() {
	destroy_texture_resource_list(GTextureRenderBuffer);
	destroy_texture_asset_list(GTextureResources);
}

void destroy_texture_resource_list(TextureResourceList* obj) {
	for (int i = 0; i < obj->size; i++) {
		TextureResource* t = obj->list[i];
		free_texture_resource(t);
	}
	free(obj->list);
	free(obj);
}

void destroy_texture_asset_list(TextureResourceList* obj) {
	for (int i = 0; i < obj->size; i++) {
		TextureResource* t = obj->list[i];
		free_texture_asset(t);
	}
	free(obj->list);
	free(obj);
}
