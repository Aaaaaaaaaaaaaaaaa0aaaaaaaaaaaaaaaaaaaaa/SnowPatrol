#pragma once

#include<SDL.h>
#include<SDL_mixer.h>
#include<string>
#include "MapElement.h"
#include<vector>
using namespace std;

//
//template<typename T> class Array2D
//{
//	private:
//	T data;
//	unsigned int width, height;
//public:
//	T* operator[](unsigned int i) { return data + i*height; }
//
//Array2D(unsigned int w, unsigned int h) :
//		width(w), height(h), data(new T[w*h]) {}
//~Array2D() { delete[] data; }
//};

class Player
{
private:

	bool isAlive;

	SDL_Rect cropRect;
	SDL_Texture *texture;
	float frameCounter;
	int frameWidth, frameHeight;
	int textureWidth;
	bool isReadyToMove;
	int usedFramesAmount;
	int prevPositionX, prevPositionY;
	char whereLook; //  u, d, l, r  - куда смотрит человечек
	int speed; // должен быть кратен 3


	//protection
	SDL_Rect cropRectProtection;
	SDL_Texture *textureProtection;
	float frameCounterProtection;
	int frameWidthProtection, frameHeightProtection;
	int textureWidthProtection;
	int protectionSeconds;

	//SnowBall
	SDL_Rect cropRectSnowBall;
	SDL_Texture *textureSnowBall;
	float frameCounterSnowBall;
	int frameWidthSnowBall, frameHeightSnowBall;
	int textureHeightSnowBall;
	bool ballThrown;
	char whereToGo; //  u, d, l, r  - куда летит снежок

	//Star sound effects
	Mix_Chunk * StarSound;

public:

	int Lifes;
	int Points;

	Player(SDL_Renderer *renderTarget, std::string filePath, int x, int y);
	~Player();

	int placeX, placeY; //место в игровой таблице - х - по вертикали, у-по горизонтали

	void Update(float delta, const Uint8 *keyState, bool **map, SDL_Renderer *renderTarget, vector<vector<MapElement *>> mapel);
	void Draw(SDL_Renderer *renderTarget);

	SDL_Rect positionRect;

	//SnowBall
	void UpdateSnowBall(float delta, vector<vector<MapElement *>> mapel);
	SDL_Rect positionRectSnowBall;

	void UpdateProtection(float delta);
	SDL_Rect positionRectProtection;

	bool protectionMode;

	bool Alive();
	void GetAlive();

};

