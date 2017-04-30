#pragma once

#include<SDL.h>
#include<string>

class Block
{
private:
	SDL_Rect cropRect;
	SDL_Texture *texture;
	float frameCounter;
	int frameWidth, frameHeight;
	int textureWidth;

public:
public:
	Block(SDL_Renderer *renderTarget, std::string filePath, int x, int y);
	~Block();

	void Update(float delta);
	void Draw(SDL_Renderer *renderTarget);

	SDL_Rect positionRect;
};

