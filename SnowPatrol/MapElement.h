#pragma once
#include<SDL.h>
#include<SDL_image.h>

class MapElement
{
private:

	//ProtectionBonus
	SDL_Rect cropRectProtectionBonus;
	SDL_Texture *textureProtectionBonus;
	float frameCounterProtectionBonus;
	int frameWidthProtectionBonus, frameHeightProtectionBonus;
	int textureWidthProtectionBonus;
	int ProtectionBonusSeconds;
	SDL_Rect positionRectProtectionBonus;

	//Star
	SDL_Rect cropRectStar;
	SDL_Texture *textureStar;
	float frameCounterStar;
	int frameWidthStar, frameHeightStar;
	int textureWidthStar;
	SDL_Rect positionRectStar;

	//lifeBonus
	SDL_Rect cropRectLifeBonus;
	SDL_Texture *textureLifeBonus;
	float frameCounterLifeBonus;
	int frameWidthLifeBonus, frameHeightLifeBonus;
	int textureWidthLifeBonus;
	int LifeBonusSeconds;
	SDL_Rect positionRectLifeBonus;

	//fireBonus
	SDL_Rect cropRectFireBonus;
	SDL_Texture *textureFireBonus;
	float frameCounterFireBonus;
	int frameWidthFireBonus, frameHeightFireBonus;
	int textureWidthFireBonus;
	int FireBonusSeconds;
	SDL_Rect positionRectFireBonus;

public:
	bool block;
	bool player;
	bool aiplayer;
	bool star;
	bool fireBonus;
	bool lifeBonus;
	bool protectionBonus;
	bool snowball;

	
	MapElement(SDL_Renderer *renderTarget, int x, int y);

	void Draw(SDL_Renderer *renderTarget);
	void Update(float delta, int x, int y);
	void UpdateProtectionBonus(float delta, int x, int y);
	void UpdateLifeBonus(float delta, int x, int y);
	void UpdateStar(float delta, int x, int y);

	~MapElement();
};

