#include "AssetManager.h"
#include "ErrorMessage.h"

AssetManager::AssetManager()
{}

AssetManager::~AssetManager()
{}

void AssetManager::addTexture(std::string id, const char* path, SDL_Renderer* renderer)
{
	SDL_Surface* tempSurface = IMG_Load(path);
    if (tempSurface)
    {
	    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
	    SDL_DestroySurface(tempSurface);

	    textures.emplace(id, tex);
    }
    else
    {
        ErrorMessage::showSDlError("ERROR: IMG_Load()");
    }
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}

void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
    TTF_Font* font = TTF_OpenFont(path.c_str(), fontSize);
    if (font)
    {
        fonts.emplace(id, font);
    }
    else
    {
        ErrorMessage::showSDlError("ERROR: TTF_OpenFont()");
    }
}

TTF_Font* AssetManager::getFont(std::string id)
{
    return fonts[id];
}