#pragma once
#include<SDL.h>
#include<SDL_mixer.h>
#include<string>
#include "MapElement.h"
#include<vector>
using namespace std;

class AIPlayer
{
private:
	bool ReadyToKill;
	bool isAlive;

	SDL_Rect cropRect;
	SDL_Texture *texture;
	float moveSpeed;
	float frameCounter;
	int frameWidth, frameHeight;
	int textureWidth;
	bool isReadyToMove;
	int prevPositionX, prevPositionY;
	int usedFramesAmount;
	char whereLook; //  u, d, l, r  - куда смотрит человечек
	
	//SnowBall
	SDL_Rect cropRectSnowBall;
	SDL_Texture *textureSnowBall;
	float frameCounterSnowBall;
	int frameWidthSnowBall, frameHeightSnowBall;
	int textureHeightSnowBall;
	bool ballThrown;
	char whereToGo; //  u, d, l, r  - куда летит снежок


	//SnowBurnt
	bool snowBurnt;
	SDL_Rect cropRectSnowBurnt;
	SDL_Texture *textureSnowBurnt;
	float frameCounterSnowBurnt;
	int frameWidthSnowBurnt, frameHeightSnowBurnt;
	int textureWidthSnowBurnt;
	int SnowBurntSeconds;
	SDL_Rect positionRectSnowBurnt;
	bool isKilled;
	Mix_Chunk * snowBurntSound;
	bool snowBurntSoundIsPLayed;

public:
	AIPlayer(SDL_Renderer *renderTarget, std::string filePath, int x, int y);
	~AIPlayer();

	void Update(float delta, bool **map, int playerX, int playerY, vector<vector<MapElement *>> mapel);
	void Draw(SDL_Renderer *renderTarget);
	int placeX, placeY; //место в игровой таблице - х - по вертикали, у-по горизонтали
	
	void UpdateSnowBurnt(float delta);

	bool Alive();
	void GetAlive();
	bool Killed();


	//SnowBall
	void Kill(float delta, vector<vector<MapElement *>> mapel);
	SDL_Rect positionRectSnowBall;

	SDL_Rect positionRect;
};

