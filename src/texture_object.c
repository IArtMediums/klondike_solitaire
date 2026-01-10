#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./texture_object.h"
#include "./constants.h"
#include "./game_board.h"

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
	if (texture->transform == NULL) {
		SDL_DestroyTexture(texture->image);
		free(texture->image_path);
		free(texture);
		return NULL;
	}
	texture->transform->x = x;
	texture->transform->y = y;
	texture->transform->w = w;
	texture->transform->h = h;
	texture->queue_free = false;
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
	GTextureResources->list[CARD_FACEUP] = new_texture_resource(r, "textures/test.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);

}

TextureResource* get_texture_resource(LoadedResources type, int x, int y) {
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
	new->queue_free = false;
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
	// collect render buffers
	TextureResourceList* card_buffer = get_card_render_buffer();

	// render in order
	if (card_buffer != NULL) {
		render_buffer(r, card_buffer);
	}
}

void destroy_texture_buffer(TextureResourceList* buffer) {
	free(buffer->list);
	free(buffer);
}

void render_buffer(SDL_Renderer* r, TextureResourceList* buffer) {
	if (buffer == NULL) return;
	if (buffer->size == 0) return;
	for (int i = 0; i < buffer->size; i++) {
		TextureResource* t = buffer->list[i];
		SDL_RenderCopy(r, t->image, NULL, t->transform);
	}
	destroy_texture_buffer(buffer);
}

void update_buffer(TextureResourceList* obj) {
	TextureResourceList* to_remove = new_texture_resource_list(8);
	for (int i = 0; i < obj->size; i++) {
		if (obj->list[i]->queue_free) {
			add_texture_resource_to_list(to_remove, obj->list[i]);
		}
	}
	for (int i = 0; i < to_remove->size; i++) {
		remove_from_queue(obj, to_remove->list[i]);
		to_remove->list[i] = NULL;
	}
	free(to_remove->list);
	free(to_remove);
}

void remove_from_queue(TextureResourceList* obj, TextureResource* texture) {
	int index = -1;
	for (int i = 0; i < obj->size; i++) {
		if (obj->list[i] == texture) {
			index = i;
			break;
		}
	}
	if (index == -1) return;
	free_texture_resource(obj->list[index]);
	for (int i = index; i < obj->size - 1; i++) {
		obj->list[i] = obj->list[i + 1];
	}
	obj->size--;
	obj->list[obj->size] = NULL;
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
