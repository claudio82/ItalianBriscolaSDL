#include "TextureManager.h"

SDL_Texture* TextureManager::loadTexture(SDL_Renderer* renderer, const char* fileName)
{
    SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_DestroySurface(tempSurface);

	return tex;
}

void TextureManager::draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_FRect src, SDL_FRect dest, SDL_FlipMode flip)
{
    SDL_RenderTextureRotated(renderer, tex, &src, &dest, 0.0, NULL, flip);
}