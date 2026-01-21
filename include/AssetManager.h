#pragma once
#include <map>
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    void addTexture(std::string id, const char* path, SDL_Renderer* renderer);
    SDL_Texture* getTexture(std::string id);

    void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
    std::map<std::string, SDL_Texture*> textures;
    std::map<std::string, TTF_Font*> fonts;
};

