#include "Block.h"
#include<SDL_image.h>
#include<iostream>



Block::Block(SDL_Renderer *renderTarget, std::string filePath, int x, int y)
{
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		if (texture == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &cropRect.w, &cropRect.h);

	positionRect.x = x; // Позиция картинки по оси Х
	positionRect.y = y; //Позиция картинкаи по оси У

	textureWidth = cropRect.w; // Ширина спрайта - текстуры

	cropRect.w /= 3; // 3 кадра в ширину у спрайта анимации
	cropRect.h /= 4; // 4 кадра в высоту у спрайта анимации

	frameWidth = positionRect.w = cropRect.w;
	frameHeight = positionRect.h = cropRect.h;

}

Block::~Block()
{
	SDL_DestroyTexture(texture);
}


void Block::Update(float delta)
{
	int r;

	frameCounter += delta;
	if (frameCounter >= 0.2f)
	{

		r = rand() % 3;
		cropRect.y = frameHeight * r;

		frameCounter = 0;
		cropRect.x += frameWidth;
		if (cropRect.x >= textureWidth)
			cropRect.x = 0;
	}

}

void Block::Draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect);
}

