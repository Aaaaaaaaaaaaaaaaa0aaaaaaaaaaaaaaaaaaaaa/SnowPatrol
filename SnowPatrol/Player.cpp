#include "Player.h"
#include<SDL_image.h>
#include<iostream>

Player::Player(SDL_Renderer *renderTarget, std::string filePath, int x, int y)
{
	Lifes = 3; //3 lifes at the beginning
	Points = 0; //Amount of points 

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

	isReadyToMove = true; // На момент создания объект - не активен , т.е. готов к нажатию кнопок и движению
	usedFramesAmount = 0;
	whereLook = 'd';

	placeX = x / 32; // 32 пикселя на ячейку
	placeY = y / 32; // 32 пикселя на ячейку

	speed = 2;
	
	//SnowBall

	SDL_Surface *surfaceSnowBall = IMG_Load("snowball.png");
	if (surfaceSnowBall == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureSnowBall = SDL_CreateTextureFromSurface(renderTarget, surfaceSnowBall);
		if (textureSnowBall == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceSnowBall);

	SDL_QueryTexture(textureSnowBall, NULL, NULL, &cropRectSnowBall.w, &cropRectSnowBall.h);

	positionRectSnowBall.x = 5 * 32; // Позиция картинки по оси Х (по умолчанию 0)
	positionRectSnowBall.y = 3 * 32; //Позиция картинкаи по оси У (по умолчанию 0)

	textureHeightSnowBall = cropRectSnowBall.h; // Ширина спрайта - текстуры

	cropRectSnowBall.w /= 1; // 1 кадр в ширину у спрайта анимации
	cropRectSnowBall.h /= 10; // 10 кадров в высоту у спрайта анимации

	frameWidthSnowBall = positionRectSnowBall.w = cropRectSnowBall.w;
	frameHeightSnowBall = positionRectSnowBall.h = cropRectSnowBall.h;

	ballThrown = false;

	//Protection Mode

	SDL_Surface *surfaceProtection = IMG_Load("Protection.png");
	if (surfaceProtection == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureProtection = SDL_CreateTextureFromSurface(renderTarget, surfaceProtection);
		if (textureProtection == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceProtection);

	SDL_QueryTexture(textureProtection, NULL, NULL, &cropRectProtection.w, &cropRectProtection.h);

	positionRectProtection.x = positionRect.x - 41; //позиция такая: чтобы в середине был человечек
	positionRectProtection.y = positionRect.y - 41;

	textureWidthProtection = cropRectProtection.w; // Ширина спрайта - текстуры

	cropRectProtection.w /= 5; // 5 кадров в ширину у спрайта анимации
	cropRectProtection.h /= 3; // 3 кадрa в высоту у спрайта анимации

	frameWidthProtection = positionRectProtection.w = cropRectProtection.w;
	frameHeightProtection = positionRectProtection.h = cropRectProtection.h;

	protectionSeconds = 300;

	//Star sound
	StarSound = Mix_LoadWAV("starGot.ogg");
}

void Player::UpdateProtection(float delta)
{
	positionRectProtection.x = positionRect.x - 41; //позиция такая: чтобы в середине был человечек
	positionRectProtection.y = positionRect.y - 41;

	frameCounterProtection += delta;

	if (frameCounterProtection >= 0.05f)
	{
		frameCounterProtection = 0;
		cropRectProtection.x += frameHeightProtection;
		if (cropRectProtection.x >= textureWidthProtection)
		{
			cropRectProtection.x = 0;
			cropRectProtection.y += frameHeightProtection;
			if (cropRectProtection.y > 2 * frameHeightProtection)
			{
				cropRectProtection.y = 0;
			}
		}
	}
}

void Player::UpdateSnowBall(float delta, vector<vector<MapElement *>> mapel)
{
	if (positionRectSnowBall.y < 0 || positionRectSnowBall.y >= 15 * 32 ||
		positionRectSnowBall.x < 0 || positionRectSnowBall.x >= 15 * 32 )
		ballThrown = false;
	else
	{
		
		if (mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->block ||
			mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->aiplayer)
		{
			if (mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->aiplayer)
				mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->snowball = true;
			ballThrown = false;
		}
		else
		{
			ballThrown = true;
			mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->snowball = false;
		}
	}
	if (ballThrown)
	{
		frameCounterSnowBall += delta;

		if (frameCounterSnowBall >= 0.025f)
		{
			frameCounterSnowBall = 0;
			cropRectSnowBall.y += frameHeightSnowBall;
			if (cropRectSnowBall.y >= textureHeightSnowBall)
				cropRectSnowBall.y = 0;
		}

		switch (whereToGo)
		{
		case 'u':
			positionRectSnowBall.y -= 4;
			break;

		case 'd':
			positionRectSnowBall.y += 4;
			break;

		case 'l':
			positionRectSnowBall.x -= 4;
			break;

		case 'r':
			positionRectSnowBall.x += 4;
			break;
		default:
			break;
		}
	}
}

Player::~Player()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(textureSnowBall);
	Mix_FreeChunk(StarSound);
	StarSound = nullptr;
}
void Player::Update(float delta, const Uint8 *keyState, bool **map, SDL_Renderer *renderTarget, vector<vector<MapElement *>> mapel)
{
	if (mapel[placeX][placeY]->snowball && mapel[placeX][placeY]->player)
	{
		Lifes--;
		mapel[placeX][placeY]->player = false;
		mapel[placeX][placeY]->snowball = false;
	}

	mapel[placeX][placeY]->player = false; //говорим что нас на предыдущей клетке уже нет


	if (protectionMode)
	{
		if (protectionSeconds > 0)
		{
			UpdateProtection(delta);
			protectionSeconds--;
		}
		else
		{
			protectionSeconds = 300;
			protectionMode = false;
		}
	}

	prevPositionX = placeX * 32;
	prevPositionY = placeY * 32;

	if (keyState[SDL_SCANCODE_SPACE] && !ballThrown)
	{
		ballThrown = true;
		whereToGo = whereLook;
		positionRectSnowBall.x = placeX * 32 + 8;
		positionRectSnowBall.y = placeY * 32 + 8;
		UpdateSnowBall(delta, mapel);
	}

	if (ballThrown)
		UpdateSnowBall( delta, mapel);

	if (isReadyToMove)
	{
		if (keyState[SDL_SCANCODE_UP]) 
		{
			whereLook = 'u';
			isReadyToMove = false;
			usedFramesAmount = 0;
		}
		else
			if (keyState[SDL_SCANCODE_DOWN]) 
			{
				whereLook = 'd';
				isReadyToMove = false;
				usedFramesAmount = 0;
			}
		else
			if (keyState[SDL_SCANCODE_LEFT]) 
			{
				whereLook = 'l';
				isReadyToMove = false;
				usedFramesAmount = 0;
			}
		else
			if (keyState[SDL_SCANCODE_RIGHT])
			{
				whereLook = 'r';
				isReadyToMove = false;
				usedFramesAmount = 0;
			}
	}

// horizontal sprite animation
	if (!isReadyToMove)
	{
		frameCounter += delta;

		if (frameCounter >= 0.25f/speed)
		{
			frameCounter = 0;
			cropRect.x += frameWidth;
			if (cropRect.x >= textureWidth)
				cropRect.x = 0;
		}
	}
	else
	{
		frameCounter = 0;
		cropRect.x = frameWidth;
	}

//vertical sprite animation and moving rectangle
	if (!isReadyToMove)
	{
		switch (whereLook)
		{
		case 'u':
			if (placeY - 1 >= 0 && !map[placeX][placeY - 1])
			{
				cropRect.y = frameHeight * 3;

				if (usedFramesAmount < 32/speed)
				{
					usedFramesAmount++;
					positionRect.y -= speed;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}

				if (prevPositionY - 32 >= positionRect.y)
					placeY--;
			}
			else
			{
				usedFramesAmount = 0;
				isReadyToMove = true;
			}
			break;

		case 'd':
			if (placeY + 1 < 15 && !map[placeX][placeY + 1])
			{
				cropRect.y = 0;

				if (usedFramesAmount < 32/speed)
				{
					usedFramesAmount++;
					positionRect.y += speed;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}

				if (prevPositionY + 32 <= positionRect.y)
					placeY++;
			}
			else 
			{
				usedFramesAmount = 0;
				isReadyToMove = true;
			}

			break;

		case 'l':
			if (placeX - 1 >= 0 && !map[placeX - 1][placeY])
			{
				cropRect.y = frameHeight;

				if (usedFramesAmount < 32/speed)
				{
					usedFramesAmount++;
					positionRect.x -= speed;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}

				if (prevPositionX - 32 >= positionRect.x)
					placeX--;
			}
			else
			{
				usedFramesAmount = 0;
				isReadyToMove = true;
			}
			break;

		case 'r':
			if (placeX + 1 < 15 && !map[placeX + 1][placeY])
			{
				cropRect.y = frameHeight * 2;

				if (usedFramesAmount < 32/speed)
				{
					usedFramesAmount++;
					positionRect.x += speed;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}

				if (prevPositionX + 32 <= positionRect.x)
					placeX++;
			}
			else
			{
				usedFramesAmount = 0;
				isReadyToMove = true;
			}
			break;
		default:
			break;
		//else
			//isReadyToMove = false;
		}
	}

	if (mapel[placeX][placeY]->protectionBonus)
	{
		mapel[placeX][placeY]->protectionBonus = false;
		protectionMode = true;
	}

	if (mapel[placeX][placeY]->star)
	{
		Points++;
		Mix_PlayChannel(-1, StarSound, 0);
		mapel[placeX][placeY]->star = false;
	}

	if (mapel[placeX][placeY]->aiplayer && !protectionMode)
	{
		Lifes--;
	}

	if (mapel[placeX][placeY]->lifeBonus)
	{
		Lifes++;
		mapel[placeX][placeY]->lifeBonus = false;
	}

	if (!protectionMode)
	{
		mapel[placeX][placeY]->player = true;
	}
	else
	{
		mapel[placeX][placeY]->player = false;
	}

	if (Lifes == 0)
		isAlive = false;
}
void Player::Draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect);
	if (ballThrown)
		SDL_RenderCopy(renderTarget, textureSnowBall, &cropRectSnowBall, &positionRectSnowBall);
	if (protectionMode)
		SDL_RenderCopy(renderTarget, textureProtection, &cropRectProtection, &positionRectProtection);
}

bool Player::Alive()
{
	return isAlive;
}

void Player::GetAlive()
{
	isAlive = true;
}
