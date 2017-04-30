#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include"Player.h"
#include"Block.h"
#include"AIPlayer.h"


#include<vector>
#include<queue>
using namespace std;

SDL_Texture *LoadTexture(std::string filePath, SDL_Renderer *renderTarget)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = IMG_Load(filePath.c_str());
	if (surface == NULL)
		std::cout << "Could not load the image at: " << filePath << ": " << SDL_GetError() << std::endl;
	else
	{
		texture = SDL_CreateTextureFromSurface(renderTarget, surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}


int main(int argc, char *argv[])
{

	int Level = 0;

	bool **map = new bool*[15];
	for (int i = 0; i < 15; i++)
	{
		map[i] = new bool[15];
		for (int j = 0; j < 15; j++)
			map[i][j] = false;
	}

	//Blocks Making

	map[5][0] = true;
	map[11][0] = true;

	map[1][1] = true;
	map[2][1] = true;
	map[3][1] = true;
	map[5][1] = true;
	map[7][1] = true;
	map[11][1] = true;
	map[13][1] = true;

	map[5][2] = true;
	map[7][2] = true;
	map[13][2] = true;

	map[1][3] = true;
	map[3][3] = true;
	map[4][3] = true;
	map[5][3] = true;
	map[7][3] = true;
	map[8][3] = true;
	map[9][3] = true;
	map[10][3] = true;
	map[11][3] = true;
	map[12][3] = true;
	map[13][3] = true;

	map[1][4] = true;
	map[3][4] = true;
	map[11][4] = true;

	map[1][5] = true;
	map[3][5] = true;
	map[5][5] = true;
	map[6][5] = true;
	map[7][5] = true;
	map[9][5] = true;
	map[11][5] = true;
	map[13][5] = true;
	map[14][5] = true;

	map[1][6] = true;
	map[3][6] = true;
	map[9][6] = true;
	map[11][6] = true;

	map[1][7] = true;
	map[3][7] = true;
	map[5][7] = true;
	map[11][7] = true;
	map[12][7] = true;
	map[13][7] = true;
	map[14][7] = true;

	map[1][8] = true;
	map[5][8] = true;
	map[9][8] = true;
	map[11][8] = true;

	map[1][9] = true;
	map[2][9] = true;
	map[3][9] = true;
	map[5][9] = true;
	map[6][9] = true;
	map[7][9] = true;
	map[8][9] = true;
	map[9][9] = true;
	map[11][9] = true;
	map[13][9] = true;
	map[14][9] = true;

	map[11][10] = true;

	map[0][11] = true;
	map[2][11] = true;
	map[4][11] = true;
	map[5][11] = true;
	map[6][11] = true;
	map[7][11] = true;
	map[8][11] = true;
	map[9][11] = true;
	map[11][11] = true;
	map[12][11] = true;
	map[13][11] = true;

	map[2][12] = true;
	map[12][12] = true;

	map[2][13] = true;
	map[3][13] = true;
	map[4][13] = true;
	map[5][13] = true;
	map[6][13] = true;
	map[7][13] = true;
	map[8][13] = true;
	map[9][13] = true;
	map[10][13] = true;
	map[12][13] = true;


	map[7][14] = true;


	// Initializing and loading variables
	SDL_Window *window = nullptr;
	SDL_Renderer *renderTarget = nullptr;
	int currentTime = 0;
	int prevTime = 0;
	float delta = 0.0f;
	const Uint8 *keyState;
	
	//Music
	SDL_Init(SDL_INIT_VIDEO /*| SDL_INIT_AUDIO*/);

	

	window = SDL_CreateWindow("Snowball party!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 32 * 15, 32 * 20, SDL_WINDOW_SHOWN);
	renderTarget = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	//Music
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		cout << "Audio Error: " << Mix_GetError() << endl;

	Mix_Music *bgndMusic = Mix_LoadMUS("bm.mp3"); //Фоновая Music 
	Mix_PlayMusic(bgndMusic, -1); // Включаем фоновую музыку -1 - значит будет бесконечно повторяться

	vector< vector<MapElement *> > mapel;

	mapel.resize(15);
	for (int i = 0, size = mapel.size(); i < size; ++i)   mapel[i].resize(15);  //По аналогии с выделением памяти у двумерного масива 15*15

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			mapel[i][j] = new MapElement(renderTarget, i, j);
			mapel[i][j]->player = false;
			mapel[i][j]->aiplayer = false;
			mapel[i][j]->fireBonus = false;
			mapel[i][j]->protectionBonus = false;
			mapel[i][j]->star = false;
			if (map[i][j])
				mapel[i][j]->block = true;
			else
				mapel[i][j]->block = false;
		}
	}

	Block *blocks[90]; // массив указателей на блоки

	int blocksAmount = 0; //будет 37
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (map[i][j])
			{ 
				blocks[blocksAmount] = new Block(renderTarget, "snow.png", i * 32, j * 32);
				blocksAmount++;
			}
		}
	}


	

//background


int imgFlags = IMG_INIT_PNG;
if (!(IMG_Init(imgFlags) & imgFlags))
std::cout << "Error: " << IMG_GetError() << std::endl;
SDL_Texture *background = LoadTexture("background.png", renderTarget);

SDL_Rect backgroundRect;
backgroundRect.x = backgroundRect.y = 0;
backgroundRect.w = 480;
backgroundRect.h = 640;

//digits of Life
SDL_Texture *lifeDigit = LoadTexture("digits.png", renderTarget);
SDL_Rect lifeDigitRect;
SDL_Rect lifeDigitPosition;
lifeDigitPosition.x = 64;
lifeDigitPosition.y = 590;
lifeDigitPosition.w = 43;
lifeDigitPosition.h = 32;
int lifeDigitframeWidth, lifeDigitframeHeight;
int lifeDigitWidth, lifeDigitHeight;
SDL_QueryTexture(lifeDigit, NULL, NULL, &lifeDigitWidth, &lifeDigitHeight);
lifeDigitframeWidth = lifeDigitWidth / 10;
lifeDigitframeHeight = lifeDigitHeight;

lifeDigitRect.x = lifeDigitRect.y = 0;
lifeDigitRect.w = lifeDigitframeWidth;
lifeDigitRect.h = lifeDigitHeight;


//digits of Level = star
SDL_Texture *levelDigit = LoadTexture("digits.png", renderTarget);
SDL_Rect levelDigitRect;
SDL_Rect levelDigitPosition;
levelDigitPosition.x = 232;
levelDigitPosition.y = 590;
levelDigitPosition.w = 43;
levelDigitPosition.h = 32;
int levelDigitframeWidth, levelDigitframeHeight;
int levelDigitWidth, levelDigitHeight;
SDL_QueryTexture(levelDigit, NULL, NULL, &levelDigitWidth, &levelDigitHeight);
levelDigitframeWidth = levelDigitWidth / 10;
levelDigitframeHeight = levelDigitHeight;

levelDigitRect.x = levelDigitRect.y = 0;
levelDigitRect.w = levelDigitframeWidth;
levelDigitRect.h = levelDigitHeight;

//Picture with suggestion to play again
SDL_Texture *suggestionPicture = LoadTexture("playerDead.png", renderTarget);
SDL_Rect suggestionPictureRect;
SDL_Rect suggestionPicturePosition;
suggestionPicturePosition.x = suggestionPicturePosition.y = 0;
suggestionPicturePosition.w = 480;
suggestionPicturePosition.h = 640;
int suggestionPictureframeWidth, suggestionPictureframeHeight;
int suggestionPictureWidth, suggestionPictureHeight;
SDL_QueryTexture(suggestionPicture, NULL, NULL, &suggestionPictureWidth, &suggestionPictureHeight);
suggestionPictureframeWidth = suggestionPictureWidth;
suggestionPictureframeHeight = suggestionPictureHeight;

suggestionPictureRect.x = suggestionPictureRect.y = 0;
suggestionPictureRect.w = suggestionPictureframeWidth;
suggestionPictureRect.h = suggestionPictureHeight;

//Player

Player player(renderTarget, "player.png", 14 * 32, 14 * 32);

mapel[14][14]->player = true;
player.GetAlive();

//AIPlayer

AIPlayer * aiPlayers[7];

aiPlayers[0] = new AIPlayer(renderTarget, "comp1.png", 0 * 32, 0 * 32);
mapel[0][0]->aiplayer = true;

//aiPlayers[3] = new AIPlayer(renderTarget, "comp4.png", 6 * 32, 9 * 32);
//mapel[9][6]->aiplayer = true;
//
//aiPlayers[4] = new AIPlayer(renderTarget, "comp5.png", 7 * 32, 9 * 32);
//mapel[9][7]->aiplayer = true;
//
//aiPlayers[5] = new AIPlayer(renderTarget, "comp6.png", 8 * 32, 9 * 32);
//mapel[9][8]->aiplayer = true;
//
//aiPlayers[6] = new AIPlayer(renderTarget, "comp7.png", 9 * 32, 9 * 32);
//mapel[9][9]->aiplayer = true;
//

bool isRunning = true;
SDL_Event ev;

aiPlayers[0]->GetAlive();
mapel[7][7]->star = true;

int currentEnemy = 0, enemiesAmount = 1;  // currentEnemy * 2+1;
bool everyoneIsKilled = false; //все враги повержены или нет?
while (isRunning)
{
	//Проверяем нажатие на крестик закрытия окна
	while (SDL_PollEvent(&ev) != 0)
	{
		// Getting the events
		if (ev.type == SDL_QUIT)
			isRunning = false;
	}

	//Если игрок жив выполняем всю анимацию
	if (player.Alive())
	{
		player.Lifes = 9;
		//Случайно выбираем место и время появления бонусов жизни и защиты
		int r1 = rand() % 100;
		int r2 = rand() % 200;
		if (r1 > r2 && r1 + r2 < 14 && !mapel[r1 - r2][r1 + r2]->block)
			mapel[r1 - r2][r1 + r2]->protectionBonus = true; 
		r1 = rand() % 100;
		r2 = rand() % 400;
		if (r1 > r2 && r1 + r2 < 14 && !mapel[r1 - r2][r1 + r2]->block)
			mapel[r1 - r2][r1 + r2]->lifeBonus = true;

		//Подсчитываем специальную временную дельту для прорисовки анимации в нижестоящих функциях Update
		prevTime = currentTime; //Предыдущее значение миллисекунд
		currentTime = SDL_GetTicks(); //Кол-во МИЛЛИсекунд с момента начала подключения библиотеки SDL
		delta = (currentTime - prevTime) / 1000.0f; //Кол-во СЕКУНД с момента предыдущего прохода по циклу по текущий момент


		//Обновляем картинки ледяных блоков
		for (int i = 0; i < blocksAmount; i++)
		{
			blocks[i]->Update(delta);
		}

		//Ловим нажатую кнопку
		keyState = SDL_GetKeyboardState(NULL);

		//Обновляем игрока
		player.Update(delta, keyState, map, renderTarget, mapel);

		switch (Level)
		{
		case 0:
			if (mapel[7][7]->star || aiPlayers[0]->Alive()) //Если поймана звезда еще не поймана или враг еще не был заморожен
			{
				aiPlayers[0]->Update(delta, map, player.placeY, player.placeX, mapel);
			}
			else
			{
				//Новый уровень
				//Удаляем старых врагов
				mapel[aiPlayers[0]->placeX][aiPlayers[0]->placeY]->aiplayer = false;
				aiPlayers[0]->~AIPlayer(); //деструтор
				delete aiPlayers[0]; //удаляем указатель?

				//Создаем новых врагов
				aiPlayers[1] = new AIPlayer(renderTarget, "comp2.png", 10 * 32, 4 * 32);
				mapel[10][4]->aiplayer = true;
				aiPlayers[1]->GetAlive();

				aiPlayers[2] = new AIPlayer(renderTarget, "comp3.png", 10 * 32, 10 * 32);
				mapel[10][10]->aiplayer = true;
				aiPlayers[2]->GetAlive();

				//Ставим звезду
				mapel[6][14]->star = true;
				
				//Новый уровень
				Level = 1;
			}
			break;
		case 1:
			if (aiPlayers[1]->Alive())
				aiPlayers[1]->Update(delta, map, player.placeY, player.placeX, mapel);
			if (mapel[6][14]->star || aiPlayers[2]->Alive())
				aiPlayers[2]->Update(delta, map, player.placeY, player.placeX, mapel);
			else
			{
				//Новый уровень
				//Удаляем старых врагов
				mapel[aiPlayers[1]->placeX][aiPlayers[1]->placeY]->aiplayer = false;
				aiPlayers[1]->~AIPlayer(); //деструтор
				delete aiPlayers[1]; //удаляем указатель?


				mapel[aiPlayers[2]->placeX][aiPlayers[2]->placeY]->aiplayer = false;
				aiPlayers[2]->~AIPlayer(); //деструтор
				delete aiPlayers[2]; //удаляем указатель?

								
				//Создаем новых врагов
				aiPlayers[3] = new AIPlayer(renderTarget, "comp4.png", 6 * 32, 12 * 32);
				mapel[6][12]->aiplayer = true;
				aiPlayers[3]->GetAlive();

				aiPlayers[4] = new AIPlayer(renderTarget, "comp5.png", 2 * 32, 3 * 32);
				mapel[2][3]->aiplayer = true;
				aiPlayers[4]->GetAlive();

				aiPlayers[5] = new AIPlayer(renderTarget, "comp6.png", 4 * 32, 2 * 32);
				mapel[4][2]->aiplayer = true;
				aiPlayers[5]->GetAlive();

				//Ставим звезду
				mapel[14][6]->star = true;

				//Новый уровень
				Level = 2;
			}
			break;
		case 2:
			if (aiPlayers[3]->Alive())
				aiPlayers[3]->Update(delta, map, player.placeY, player.placeX, mapel);
			if (aiPlayers[5]->Alive())
				aiPlayers[5]->Update(delta, map, player.placeY, player.placeX, mapel);
			if (mapel[14][6]->star || aiPlayers[4]->Alive())
				aiPlayers[4]->Update(delta, map, player.placeY, player.placeX, mapel);
			else
			{
				//Новый уровень
				//Удаляем старых врагов
				mapel[aiPlayers[3]->placeX][aiPlayers[3]->placeY]->aiplayer = false;
				aiPlayers[3]->~AIPlayer(); //деструтор
				delete aiPlayers[3]; //удаляем указатель?

				mapel[aiPlayers[4]->placeX][aiPlayers[4]->placeY]->aiplayer = false;
				aiPlayers[4]->~AIPlayer(); //деструтор
				delete aiPlayers[4]; //удаляем указатель?

				mapel[aiPlayers[5]->placeX][aiPlayers[5]->placeY]->aiplayer = false;
				aiPlayers[5]->~AIPlayer(); //деструтор
				delete aiPlayers[5]; //удаляем указатель?
									 
				//Создаем новых врагов
				aiPlayers[6] = new AIPlayer(renderTarget, "comp7.png", 8 * 32, 2 * 32);
				mapel[8][2]->aiplayer = true;
				aiPlayers[6]->GetAlive();

				aiPlayers[1] = new AIPlayer(renderTarget, "comp1.png", 8 * 32, 14 * 32);
				mapel[8][14]->aiplayer = true;
				aiPlayers[1]->GetAlive();

				aiPlayers[2] = new AIPlayer(renderTarget, "comp2.png", 0 * 32, 10 * 32);
				mapel[0][10]->aiplayer = true;
				aiPlayers[2]->GetAlive();

				//Ставим звезду
				mapel[0][14]->star = true;

				//Новый уровень
				Level = 3;
			}
			break;
		case 3:
			if (aiPlayers[6]->Alive())
				aiPlayers[6]->Update(delta, map, player.placeY, player.placeX, mapel);
			if (aiPlayers[1]->Alive())
				aiPlayers[1]->Update(delta, map, player.placeY, player.placeX, mapel);
			if (mapel[0][14]->star || aiPlayers[2]->Alive())
				aiPlayers[2]->Update(delta, map, player.placeY, player.placeX, mapel);
			else
			{
				//Новый уровень
				//Удаляем старых врагов
				mapel[aiPlayers[6]->placeX][aiPlayers[6]->placeY]->aiplayer = false;
				aiPlayers[6]->~AIPlayer(); //деструтор
				delete aiPlayers[6]; //удаляем указатель?

				mapel[aiPlayers[1]->placeX][aiPlayers[1]->placeY]->aiplayer = false;
				aiPlayers[1]->~AIPlayer(); //деструтор
				delete aiPlayers[1]; //удаляем указатель?

				mapel[aiPlayers[2]->placeX][aiPlayers[2]->placeY]->aiplayer = false;
				aiPlayers[2]->~AIPlayer(); //деструтор
				delete aiPlayers[2]; //удаляем указатель?

				//					 //Создаем новых врагов
				//aiPlayers[6] = new AIPlayer(renderTarget, "comp7.png", 8 * 32, 2 * 32);
				//mapel[8][2]->aiplayer = true;
				//aiPlayers[6]->GetAlive();

				//aiPlayers[1] = new AIPlayer(renderTarget, "comp1.png", 8 * 32, 14 * 32);
				//mapel[8][14]->aiplayer = true;
				//aiPlayers[1]->GetAlive();

				//aiPlayers[2] = new AIPlayer(renderTarget, "comp2.png", 0 * 32, 0 * 32);
				//mapel[0][10]->aiplayer = true;
				//aiPlayers[2]->GetAlive();

				////Ставим звезду
				//mapel[0][14]->star = true;

				////Новый уровень
				//Level = 3;
				//TODO: level shoould be copmplete
				
			}
			break;
		}

		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				mapel[i][j]->Update(delta, i, j);
			}
		}

		//Очищаем предыдущее состояние рендера
		SDL_RenderClear(renderTarget);

		//Рисуем все на экране

		SDL_RenderCopy(renderTarget, background, &backgroundRect, NULL); //фон


		for (int i = 0; i < blocksAmount; i++)
		{
			blocks[i]->Draw(renderTarget); //ледяные блоки
		}

		player.Draw(renderTarget); // игрока

		switch (Level) // В зависимости от уровня конкретных игроков
		{
		case 0:
			aiPlayers[0]->Draw(renderTarget);
			break;
		case 1:
			aiPlayers[1]->Draw(renderTarget);
			aiPlayers[2]->Draw(renderTarget);
			break;
		case 2:
			aiPlayers[3]->Draw(renderTarget);
			aiPlayers[4]->Draw(renderTarget);
			aiPlayers[5]->Draw(renderTarget);
			break;
		case 3:
			aiPlayers[6]->Draw(renderTarget);
			aiPlayers[1]->Draw(renderTarget);
			aiPlayers[2]->Draw(renderTarget);
			break;
		}

		for (int i = 0; i < 15; i++)
		{
			for (int j = 0; j < 15; j++)
			{
				mapel[i][j]->Draw(renderTarget); // бонусную карту 
			}
		}

		//digts of life Баллы за жизнь
		lifeDigitRect.x = player.Lifes*lifeDigitframeWidth;
		SDL_RenderCopy(renderTarget, lifeDigit, &lifeDigitRect, &lifeDigitPosition);

		//digts of level Кол-во уровней - собранных звезд
		levelDigitRect.x = Level*lifeDigitframeWidth;
		SDL_RenderCopy(renderTarget, levelDigit, &levelDigitRect, &levelDigitPosition);

		//Выполняем таргетинг
		SDL_RenderPresent(renderTarget);
	}
	else //значит его убили
	{
		//показываем картинку с предложением пройти игру заново, нажав кнопку пробел
		SDL_RenderClear(renderTarget);
		SDL_RenderCopy(renderTarget, suggestionPicture, &suggestionPictureRect, &suggestionPicturePosition);
		SDL_RenderPresent(renderTarget);
		
		//Ловим нажатую кнопку
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_SPACE]) // если пробел
		{
			//удаляем всех врагов с предыдущщих мест
			switch (Level) //в Зависимости от левела удаляем врагов
			{
			case 0:
				mapel[aiPlayers[0]->placeX][aiPlayers[0]->placeY]->aiplayer = false;
				delete aiPlayers[0]; //удаляем указатель?
				break;
			case 1:
				mapel[aiPlayers[1]->placeX][aiPlayers[1]->placeY]->aiplayer = false;
				delete aiPlayers[1]; //удаляем указатель?

				mapel[aiPlayers[2]->placeX][aiPlayers[2]->placeY]->aiplayer = false;
				delete aiPlayers[2]; //удаляем указатель?
				break;
			case 2:
				mapel[aiPlayers[3]->placeX][aiPlayers[3]->placeY]->aiplayer = false;
				delete aiPlayers[3]; //удаляем указатель?

				mapel[aiPlayers[4]->placeX][aiPlayers[4]->placeY]->aiplayer = false;
				delete aiPlayers[4]; //удаляем указатель?

				mapel[aiPlayers[5]->placeX][aiPlayers[5]->placeY]->aiplayer = false;
				delete aiPlayers[5]; //удаляем указатель?

				break;
			case 3:
				mapel[aiPlayers[6]->placeX][aiPlayers[6]->placeY]->aiplayer = false;
				delete aiPlayers[6]; //удаляем указатель?

				mapel[aiPlayers[1]->placeX][aiPlayers[1]->placeY]->aiplayer = false;
				delete aiPlayers[1]; //удаляем указатель?

				mapel[aiPlayers[2]->placeX][aiPlayers[2]->placeY]->aiplayer = false;
				delete aiPlayers[2]; //удаляем указатель?

				break;
			}


			Level = 0; // Возвращаемся к исходному уровню
			
			player.GetAlive(); //возрождаем игрока
			player.Lifes = 3;

			aiPlayers[0] = new AIPlayer(renderTarget, "comp1.png", 0 * 32, 0 * 32); //заново создаем врага номер 0
			mapel[0][0]->aiplayer = true;
			aiPlayers[0]->GetAlive(); //оживляем его
			mapel[7][7]->star = true; //ставим звезду на место
		}
	}
}
	//Music
	Mix_PauseMusic();
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderTarget);
	//Music
	Mix_FreeMusic(bgndMusic);

	bgndMusic = nullptr;
	window = nullptr;
	renderTarget = nullptr;

	IMG_Quit();
	SDL_Quit();

	return 0;
}
