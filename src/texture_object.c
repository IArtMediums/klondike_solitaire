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
	GTextureRenderBuffer = new_texture_resource_list(54);
	if (GTextureResources == NULL) {
		return;
	}
	GTextureResources->list[CARD_FACEUP] = new_texture_resource(r, "textures/test.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[AH] = new_texture_resource(r, "textures/ah.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H2] = new_texture_resource(r, "textures/2h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H3] = new_texture_resource(r, "textures/3h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H4] = new_texture_resource(r, "textures/4h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H5] = new_texture_resource(r, "textures/5h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H6] = new_texture_resource(r, "textures/6h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H7] = new_texture_resource(r, "textures/7h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H8] = new_texture_resource(r, "textures/8h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H9] = new_texture_resource(r, "textures/9h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[H10] = new_texture_resource(r, "textures/10h.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[JH] = new_texture_resource(r, "textures/jh.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[QH] = new_texture_resource(r, "textures/qh.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[KH] = new_texture_resource(r, "textures/kh.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[AD] = new_texture_resource(r, "textures/ad.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D2] = new_texture_resource(r, "textures/2d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D3] = new_texture_resource(r, "textures/3d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D4] = new_texture_resource(r, "textures/4d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D5] = new_texture_resource(r, "textures/5d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D6] = new_texture_resource(r, "textures/6d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D7] = new_texture_resource(r, "textures/7d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D8] = new_texture_resource(r, "textures/8d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D9] = new_texture_resource(r, "textures/9d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[D10] = new_texture_resource(r, "textures/10d.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[JD] = new_texture_resource(r, "textures/jd.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[QD] = new_texture_resource(r, "textures/qd.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[KD] = new_texture_resource(r, "textures/kd.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[AS] = new_texture_resource(r, "textures/as.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S2] = new_texture_resource(r, "textures/2s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S3] = new_texture_resource(r, "textures/3s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S4] = new_texture_resource(r, "textures/4s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S5] = new_texture_resource(r, "textures/5s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S6] = new_texture_resource(r, "textures/6s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S7] = new_texture_resource(r, "textures/7s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S8] = new_texture_resource(r, "textures/8s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S9] = new_texture_resource(r, "textures/9s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[S10] = new_texture_resource(r, "textures/10s.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[JS] = new_texture_resource(r, "textures/js.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[QS] = new_texture_resource(r, "textures/qs.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[KS] = new_texture_resource(r, "textures/ks.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[AC] = new_texture_resource(r, "textures/ac.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C2] = new_texture_resource(r, "textures/2c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C3] = new_texture_resource(r, "textures/3c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C4] = new_texture_resource(r, "textures/4c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C5] = new_texture_resource(r, "textures/5c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C6] = new_texture_resource(r, "textures/6c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C7] = new_texture_resource(r, "textures/7c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C8] = new_texture_resource(r, "textures/8c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C9] = new_texture_resource(r, "textures/9c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[C10] = new_texture_resource(r, "textures/10c.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[JC] = new_texture_resource(r, "textures/jc.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[QC] = new_texture_resource(r, "textures/qc.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[KC] = new_texture_resource(r, "textures/kc.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
	GTextureResources->list[BACK] = new_texture_resource(r, "textures/back.png", WINDOW_WIDTH/2, WINDOW_HEIGHT/2, CARD_WIDTH, CARD_HEIGHT);
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
