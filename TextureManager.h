#ifndef TEXTUREMANAGER_H
	#define TEXTUREMANAGER_H

#include<SDL.h>
#include<SDL_image.h>
#include<stdio.h>

class TextureManager{
public:
	//static texture load and rendering, just for a bit cleaner code
	//easiest code of all B)
	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, SDL_bool transparency ,const char* file);
	static void RenderTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* destRect);
private:
};

#endif // !TEXTUREMANAGER_H
