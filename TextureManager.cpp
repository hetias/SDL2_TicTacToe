#include"TextureManager.h"

//load texture give a renderer, bool and file
//transparency applied with color keying on green (0,255,0,0)
SDL_Texture* TextureManager::LoadTexture(SDL_Renderer* renderer, SDL_bool hasTransparency, const char* file)
{
	SDL_Surface* texSurf = NULL;
	SDL_Texture* finalTex = NULL;
	if ( (texSurf = IMG_Load(file)) == NULL)
	{
		printf(".::CANT::CREATE::SURFACE::FROM::%s::.\n", file);
		return NULL;
	}
	else
	{
		SDL_SetColorKey(texSurf, hasTransparency, SDL_MapRGB(texSurf->format,0,255,0));

		finalTex = SDL_CreateTextureFromSurface(renderer, texSurf);
		if (finalTex == NULL)
		{
			printf(".::CANT::CREATE::TEXTURE::FROM::%s::.\n", file);
			return NULL;
		}
	}

	SDL_FreeSurface(texSurf);

	return finalTex;
}

//common sdl rendering
void TextureManager::RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect)
{
	SDL_RenderCopy(renderer, texture, srcRect, destRect);
}


