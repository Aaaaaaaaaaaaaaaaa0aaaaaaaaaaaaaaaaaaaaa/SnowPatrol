#include "MapElement.h"
#include<iostream>

MapElement::MapElement(SDL_Renderer *renderTarget, int x, int y)
{
	//ProtectionBonus
	SDL_Surface *surfaceProtectionBonus = IMG_Load("protectionBonus.png");
	if (surfaceProtectionBonus == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureProtectionBonus = SDL_CreateTextureFromSurface(renderTarget, surfaceProtectionBonus);
		if (textureProtectionBonus == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceProtectionBonus);

	SDL_QueryTexture(textureProtectionBonus, NULL, NULL, &cropRectProtectionBonus.w, &cropRectProtectionBonus.h);

	textureWidthProtectionBonus = cropRectProtectionBonus.w; // Ширина спрайта - текстуры

	cropRectProtectionBonus.w /= 5; // 5 кадров в ширину у спрайта анимации
	cropRectProtectionBonus.h /= 6; // 6 кадров в высоту у спрайта анимации

	frameWidthProtectionBonus = positionRectProtectionBonus.w = cropRectProtectionBonus.w;
	frameHeightProtectionBonus = positionRectProtectionBonus.h = cropRectProtectionBonus.h;

	ProtectionBonusSeconds = 300;


	//LifeBonus
	SDL_Surface *surfaceLifeBonus = IMG_Load("lifeBonus.png");
	if (surfaceLifeBonus == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureLifeBonus = SDL_CreateTextureFromSurface(renderTarget, surfaceLifeBonus);
		if (textureLifeBonus == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceLifeBonus);

	SDL_QueryTexture(textureLifeBonus, NULL, NULL, &cropRectLifeBonus.w, &cropRectLifeBonus.h);

	textureWidthLifeBonus = cropRectLifeBonus.w; // Ширина спрайта - текстуры

	cropRectLifeBonus.w /= 5; // 5 кадров в ширину у спрайта анимации
	cropRectLifeBonus.h /= 6; // 6 кадров в высоту у спрайта анимации

	frameWidthLifeBonus = positionRectLifeBonus.w = cropRectLifeBonus.w;
	frameHeightLifeBonus = positionRectLifeBonus.h = cropRectLifeBonus.h;

	LifeBonusSeconds = 300;

	//Star
	SDL_Surface *surfaceStar = IMG_Load("star.png");
	if (surfaceStar == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureStar = SDL_CreateTextureFromSurface(renderTarget, surfaceStar);
		if (textureStar == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceStar);

	SDL_QueryTexture(textureStar, NULL, NULL, &cropRectStar.w, &cropRectStar.h);

	textureWidthStar = cropRectStar.w; // Ширина спрайта - текстуры

	cropRectStar.w /= 5; // 5 кадров в ширину у спрайта анимации
	cropRectStar.h /= 6; // 6 кадров в высоту у спрайта анимации

	frameWidthStar = positionRectStar.w = cropRectStar.w;
	frameHeightStar = positionRectStar.h = cropRectStar.h;
}


void MapElement::Draw(SDL_Renderer *renderTarget)
{
	if (protectionBonus)
		SDL_RenderCopy(renderTarget, textureProtectionBonus, &cropRectProtectionBonus, &positionRectProtectionBonus);
	if (lifeBonus)
		SDL_RenderCopy(renderTarget, textureLifeBonus, &cropRectLifeBonus, &positionRectLifeBonus);
	if (star)
		SDL_RenderCopy(renderTarget, textureStar, &cropRectStar, &positionRectStar);
}

void MapElement::Update(float delta, int x, int y)
{
	if (protectionBonus)
	{
		if (ProtectionBonusSeconds > 0)
		{
			UpdateProtectionBonus(delta, x , y);
			ProtectionBonusSeconds--;
		}
		else
		{
			ProtectionBonusSeconds = 300;
			protectionBonus = false;
		}
	}

	if (lifeBonus)
	{
		if (LifeBonusSeconds > 0)
		{
			UpdateLifeBonus(delta, x, y);
			LifeBonusSeconds--;
		}
		else
		{
			LifeBonusSeconds = 300;
			lifeBonus = false;
		}
	}

	if (star)
	{
		UpdateStar(delta, x, y);
	}
	//if (aiplayer&&snowball)
	//{

	//}
}


void MapElement::UpdateProtectionBonus(float delta, int x, int y)
{
	positionRectProtectionBonus.x = x * 32 - 27; //позиция такая: чтобы в середине была ячейка игрового поля
	positionRectProtectionBonus.y = y * 32 - 27;

	frameCounterProtectionBonus += delta;

	if (frameCounterProtectionBonus >= 0.05f)
	{
		frameCounterProtectionBonus = 0;
		cropRectProtectionBonus.x += frameHeightProtectionBonus;
		if (cropRectProtectionBonus.x >= textureWidthProtectionBonus)
		{
			cropRectProtectionBonus.x = 0;
			cropRectProtectionBonus.y += frameHeightProtectionBonus;
			if (cropRectProtectionBonus.y > 5 * frameHeightProtectionBonus)
			{
				cropRectProtectionBonus.y = 0;
			}
		}
	}
}


void MapElement::UpdateLifeBonus(float delta, int x, int y)
{
	positionRectLifeBonus.x = x * 32 - 27; //позиция такая: чтобы в середине была ячейка игрового поля
	positionRectLifeBonus.y = y * 32 - 27;

	frameCounterLifeBonus += delta;

	if (frameCounterLifeBonus >= 0.05f)
	{
		frameCounterLifeBonus = 0;
		cropRectLifeBonus.x += frameHeightLifeBonus;
		if (cropRectLifeBonus.x >= textureWidthLifeBonus)
		{
			cropRectLifeBonus.x = 0;
			cropRectLifeBonus.y += frameHeightLifeBonus;
			if (cropRectLifeBonus.y > 5 * frameHeightLifeBonus)
			{
				cropRectLifeBonus.y = 0;
			}
		}
	}
}

void MapElement::UpdateStar(float delta, int x, int y)
{
	positionRectStar.x = x * 32 - 27; //позиция такая: чтобы в середине была ячейка игрового поля
	positionRectStar.y = y * 32 - 27;

	frameCounterStar += delta;

	if (frameCounterStar >= 0.05f)
	{
		frameCounterStar = 0;
		cropRectStar.x += frameHeightStar;
		if (cropRectStar.x >= textureWidthStar)
		{
			cropRectStar.x = 0;
			cropRectStar.y += frameHeightStar;
			if (cropRectStar.y > 5 * frameHeightStar)
			{
				cropRectStar.y = 0;
			}
		}
	}
}

MapElement::~MapElement()
{
}
