#include "AIPlayer.h"
#include<SDL_image.h>
#include<iostream>
#include<vector>
#include<queue>
using namespace std;

AIPlayer::AIPlayer(SDL_Renderer *renderTarget, std::string filePath, int x, int y)
{
	ReadyToKill = false;
	isAlive = false;
	isKilled = false;
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

	placeX = x / 32; // 32 пикселя на ячейку
	placeY = y / 32; // 32 пикселя на ячейку

	moveSpeed = 60.0f; //Скорость движения

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

	//SnowBurnt
	SDL_Surface *surfaceSnowBurnt = IMG_Load("snowBurnt.png");
	if (surfaceSnowBurnt == NULL)
		std::cout << "Error" << std::endl;
	else
	{
		textureSnowBurnt = SDL_CreateTextureFromSurface(renderTarget, surfaceSnowBurnt);
		if (textureSnowBurnt == NULL)
			std::cout << "Error" << std::endl;
	}

	SDL_FreeSurface(surfaceSnowBurnt);

	SDL_QueryTexture(textureSnowBurnt, NULL, NULL, &cropRectSnowBurnt.w, &cropRectSnowBurnt.h);	

	textureWidthSnowBurnt = cropRectSnowBurnt.w; // Ширина спрайта - текстуры

	cropRectSnowBurnt.w /= 5; // 5 кадров в ширину у спрайта анимации
	cropRectSnowBurnt.h /= 3; // 3 кадрa в высоту у спрайта анимации

	frameWidthSnowBurnt = positionRectSnowBurnt.w = cropRectSnowBurnt.w;
	frameHeightSnowBurnt = positionRectSnowBurnt.h = cropRectSnowBurnt.h;
	
	snowBurnt = false;

	SnowBurntSeconds = 100;

	snowBurntSound = Mix_LoadWAV("snowburnt.ogg"); // soundEffect for snowBurnt
}


AIPlayer::~AIPlayer()
{
	SDL_DestroyTexture(texture);
	Mix_FreeChunk(snowBurntSound);
	snowBurntSound = nullptr;
}

void Algorithm_Lee(vector<char> & direction, int startPos_x, int startPos_y, int finPos_x, int finPos_y, int mapL, int mapW, bool**map)
{
	//Алгоритм ЛИ

	int ax = startPos_x;
	int ay = startPos_y;
	int bx = finPos_x;
	int by = finPos_y;

	int **path = new int*[mapW];
	for (int i = 0; i < mapW; i++)
	{
		path[i] = new int[mapL];
		for (int j = 0; j < mapL; j++)
		{
			if (map[j][i]) //если стена
				path[i][j] = -2;
			else
				path[i][j] = 0;
		}
	}

	int **used = new int*[mapW];
	for (int i = 0; i < mapW; i++)
	{
		used[i] = new int[mapL];
		for (int j = 0; j < mapL; j++)
		{
			used[i][j] = map[j][i]; // все снежные стены помечены цифрой числом -2
		}
	}


	queue<pair<int, int> > q;
	q.push(make_pair(ax, ay));


	used[ax][ay] = 1;

	path[ax][ay] = -1;

	while (!q.empty() || (ax == bx && ay == by))
	{
		pair<int, int> v = make_pair(q.front().first, q.front().second);
		q.pop();
		pair<int, int> to;

		if (v.first - 1 >= 0)
		{
			to = make_pair(v.first - 1, v.second);
			if (!used[to.first][to.second])
			{
				used[to.first][to.second] = 1;
				q.push(to);
				path[to.first][to.second] = path[v.first][v.second] + 1;
			}
		}

		if (v.first + 1 < mapL)
		{
			to = make_pair(v.first + 1, v.second);
			if (!used[to.first][to.second])
			{
				used[to.first][to.second] = 1;
				q.push(to);
				path[to.first][to.second] = path[v.first][v.second] + 1;
			}
		}

		if (v.second - 1 >= 0)
		{
			to = make_pair(v.first, v.second - 1);
			if (!used[to.first][to.second])
			{
				used[to.first][to.second] = 1;
				q.push(to);
				path[to.first][to.second] = path[v.first][v.second] + 1;
			}
		}

		if (v.second + 1 < mapW)
		{
			to = make_pair(v.first, v.second + 1);
			if (!used[to.first][to.second])
			{
				used[to.first][to.second] = 1;
				q.push(to);
				path[to.first][to.second] = path[v.first][v.second] + 1;
			}
		}

	}

	pair<int, int> to = make_pair(bx, by);

	while (path[to.first][to.second] != -1)
	{
		if ((to.first - 1 >= 0) && path[to.first - 1][to.second] == path[to.first][to.second] - 1)
		{
			direction.push_back('u');
			to = make_pair(to.first - 1, to.second);
			continue;
		}
		if ((to.first + 1 < mapL) && path[to.first + 1][to.second] == path[to.first][to.second] - 1)
		{
			direction.push_back('d');
			to = make_pair(to.first + 1, to.second);
			continue;
		}
		if ((to.second - 1 >= 0) && path[to.first][to.second - 1] == path[to.first][to.second] - 1)
		{
			direction.push_back('l');
			to = make_pair(to.first, to.second - 1);
			continue;
		}
		if ((to.second + 1 < mapW) && path[to.first][to.second + 1] == path[to.first][to.second] - 1)
		{
			direction.push_back('r');
			to = make_pair(to.first, to.second + 1);
			continue;
		}
	}

	//Удаляем хвост
	char c;
	int i = direction.size() - 1;
	if (i >= 0)
	{
		c = direction[i];
		while (i >= 0 && c == direction[i])
		{
			i--;
			direction.pop_back();
		}
	}
	direction.push_back(c);
	
}

void AIPlayer::Update(float delta, bool **map, int playerX, int playerY, vector<vector<MapElement *>> mapel)
{
	if (ReadyToKill  && !ballThrown && !snowBurnt)
	{
		ballThrown = true;
		whereToGo = whereLook;
		positionRectSnowBall.x = placeX * 32 + 8;
		positionRectSnowBall.y = placeY * 32 + 8;
		Kill(delta, mapel);
	}

	if (ballThrown)
		Kill(delta, mapel);

	if (snowBurnt || mapel[placeX][placeY]->aiplayer && mapel[placeX][placeY]->snowball)
	{
		snowBurnt = true;
		if (!snowBurntSoundIsPLayed)
		{
			Mix_PlayChannel(-1, snowBurntSound, 0);
			snowBurntSoundIsPLayed = true;
		}

		mapel[placeX][placeY]->aiplayer = false;
		mapel[placeX][placeY]->snowball = false;
		mapel[placeX][placeY]->protectionBonus = false;

		if (SnowBurntSeconds > 0)
		{
			UpdateSnowBurnt(delta);
			SnowBurntSeconds--;
		}
		else
		{
			SnowBurntSeconds = 50;
			snowBurnt = false;
			isAlive = false;
			isKilled = true;
			snowBurntSoundIsPLayed = false;
		}
	}

	vector<char> direction;

	if (!ReadyToKill && !snowBurnt && (placeX != playerY || placeY != playerX)) //Если хотя бы одна координата не совпадает - оси по разному направлены в sdl и массиве
	{
		mapel[placeX][placeY]->aiplayer = false;
		if (isReadyToMove)
		{
			Algorithm_Lee(direction, playerX, playerY, placeY, placeX, 15, 15, map);

			if (direction.size())
				whereLook = direction[0];
			else
				isReadyToMove = true;
		}
	
		prevPositionX = placeX * 32;
		prevPositionY = placeY * 32;

		if (isReadyToMove)
		{
			isReadyToMove = false;
			usedFramesAmount = 0;
		}

		// horizontal sprite animation
		if (!isReadyToMove)
		{
			frameCounter += delta;

			if (frameCounter >= 0.25f)
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

		if (!isReadyToMove)
		{
			//Анимация движения
			switch (whereLook)
			{
			case 'u':
				if (placeY - 1 >= 0 && !map[placeX][placeY - 1] && !mapel[placeX][placeY - 1]->aiplayer)
				{
					
					if (usedFramesAmount < 32 )
					{
						usedFramesAmount++;
						positionRect.y -= 1; 
						cropRect.y = frameHeight * 3;
					}
					else
					{
						usedFramesAmount = 0;
						isReadyToMove = true;
					}

					if (prevPositionY - 32 == positionRect.y)
						placeY--;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}
				break;

			case 'd':
				if (placeY + 1 < 15 && !map[placeX][placeY + 1] && !mapel[placeX][placeY + 1]->aiplayer)
				{
					
					if (usedFramesAmount < 32)
					{
						usedFramesAmount++;
						positionRect.y += 1;
						cropRect.y = 0;
					}
					else
					{
						usedFramesAmount = 0;
						isReadyToMove = true;
					}

					if (prevPositionY + 32 == positionRect.y)
						placeY++;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}

				break;

			case 'l':
				if (placeX - 1 >= 0 && !map[placeX - 1][placeY] && !mapel[placeX - 1][placeY]->aiplayer)
				{
					

					if (usedFramesAmount < 32)
					{
						usedFramesAmount++;
						positionRect.x -= 1;
						cropRect.y = frameHeight;
					}
					else
					{
						usedFramesAmount = 0;
						isReadyToMove = true;
					}

					if (prevPositionX - 32 == positionRect.x)
						placeX--;
				}
				else
				{
					usedFramesAmount = 0;
					isReadyToMove = true;
				}
				break;

			case 'r':
				if (placeX + 1 < 15 && !map[placeX + 1][placeY] && !mapel[placeX+1][placeY]->aiplayer)
				{
					
					if (usedFramesAmount < 32)
					{
						usedFramesAmount++;
						positionRect.x += 1;
						cropRect.y = frameHeight * 2;
					}
					else
					{
						usedFramesAmount = 0;
						isReadyToMove = true;
					}

					if (prevPositionX + 32 == positionRect.x)
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
		mapel[placeX][placeY]->aiplayer = true;
	}

	if (!isAlive)
	{
		mapel[placeX][placeY]->aiplayer = false;
	}

	if (direction.size() == 1 && ( placeX == playerY || placeY == playerX ) )
		ReadyToKill = true;

}

void AIPlayer::Kill(float delta, vector<vector<MapElement *>> mapel)
{
	if (positionRectSnowBall.y < 0 || positionRectSnowBall.y >= 15 * 32 ||
		positionRectSnowBall.x < 0 || positionRectSnowBall.x >= 15 * 32)
	{
		ballThrown = false; 
		 ReadyToKill = false;
	}
	else
	{

		if (mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->block ||
			mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->player)
		{
			if (mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->player)
				mapel[positionRectSnowBall.x / 32][positionRectSnowBall.y / 32]->snowball = true;
			ballThrown = false;
			ReadyToKill = false;
		}
		else
		{
			ballThrown = true;
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

		switch (whereLook)
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
void AIPlayer::UpdateSnowBurnt(float delta)
{
	positionRectSnowBurnt.x = positionRect.x - 80; //позиция такая: чтобы в середине была ячейка игрового поля
	positionRectSnowBurnt.y = positionRect.y - 80;

	frameCounterSnowBurnt += delta;

	if (frameCounterSnowBurnt >= 0.05f)
	{
		frameCounterSnowBurnt = 0;
		cropRectSnowBurnt.x += frameHeightSnowBurnt;
		if (cropRectSnowBurnt.x >= textureWidthSnowBurnt)
		{
			cropRectSnowBurnt.x = 0;
			cropRectSnowBurnt.y += frameHeightSnowBurnt;
			if (cropRectSnowBurnt.y > 3 * frameHeightSnowBurnt)
			{
				cropRectSnowBurnt.y = 0;
			}
		}
	}
}

void AIPlayer::Draw(SDL_Renderer *renderTarget)
{
	SDL_RenderCopy(renderTarget, texture, &cropRect, &positionRect);
	if (snowBurnt)
		SDL_RenderCopy(renderTarget, textureSnowBurnt, &cropRectSnowBurnt, &positionRectSnowBurnt);
	if (ballThrown)
		SDL_RenderCopy(renderTarget, textureSnowBall, &cropRectSnowBall, &positionRectSnowBall);

}

bool AIPlayer::Alive()
{
	return isAlive;
}

void AIPlayer::GetAlive()
{
	isAlive = true;
}

bool AIPlayer::Killed()
{
	return isKilled;
}