
#pragma once
#include "SDL_inc.h"
#include <string.h>
#include "mariomonster.h"
#include "pomocnicze.h"



enum block_t {
	solid,
	breakable,
	coin,
	floor_block,
	end_level
};


typedef struct {
	int pos_x = NULL;
	int pos_y = NULL;
	block_t type;
} blocks_pos;

class mariolevel
{
public:
	mariolevel(int lvl);
	~mariolevel();
	int getPos_x(int i);
	int getPos_y(int i);
	int getBType(int i);
	void setPos_x();
	void setPos_y();
	bool deleteBlock(int i);
	int getNumOfMonsters();
	int getMonsterPosX(int index);
	int getMonsterPosY(int index);
	int getMonsterBoundsX();
	int getMonsterBoundsY();
	int getMonsterMove(int index);
	int getMonsterDirection(int index);
	int getNumOfBlocks();
	int getBSizeY();
	int getBSizeX();
	void moveBlocks(int wekt);
	int getSumPrzes();
	void setSumPrzes(int przes);
	int getCurrentLevel();
	void setCurrentLevel(int i);
	void resetSumPrzes();
	void minusTimeLeft();
	int getTimeLeft();
	int getMonsterMaxMove(int index);
	void draw(SDL_Surface* screen);
	SDL_Surface* getMonsterSur();
	void drawMonsters(SDL_Surface* screen);
	int getCastleBound();
	void clearLevel();
	bool setLevel();
	void resetTimeLeft();
	void deleteLevel();
	bool loadLevel(int lvl);
	int getMarioStartX();
	int getMarioStartY();
	int getLoadedCoins();
	int getLoadedLifes();
	int getLevelTime();
	int getPosXLoad();
	int getPosYLoad();
private:
	int coinsLoad;
	int lifesLoad;
	int posXLoad;
	int posYLoad;
	int currentLevel;
	void drawBlocks(SDL_Surface* screen);
	void initMonsters();
	void loadMonsters(FILE** plik);
	
	int level_time;
	int time_left;
	int sumPrzes;
	int hero_startX;
	int hero_startY;
	mariomonster monster;
	int num_of_monsters;
	blocks_pos* positions;
	SDL_Surface* monster_sur = NULL;
	SDL_Surface* castle = NULL;
	SDL_Surface* floor_b = NULL;
	SDL_Surface* breakable_b = NULL;
	SDL_Surface* coin_b = NULL;
	SDL_Surface* solid_b = NULL;
	int block_sx;
	int block_sy;
	int castle_sx;
	int num_of_blocks;
	void initBlockPositions(int num);
	void initBlockSurface();
	void initBlocksBounds();
};

