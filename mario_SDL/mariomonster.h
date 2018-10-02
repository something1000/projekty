#pragma once

#include "SDL_inc.h"
#include "pomocnicze.h"

#define MONSTER_SPEED 2 //px per frame

typedef struct {
	int pos_x;
	int pos_y;
	int max_move;
	int move;
	int move_right;
} monster_inf_t;
class mariomonster
{
public:
	mariomonster();
	~mariomonster();
	void drawMonsters(SDL_Surface* screen);
	void setMonsterPositions(int index,int x, int y);
	void setMonsterMaxMove(int index, int move);
	//void setMonsterSurface(SDL_Surface* img);
	void moveMonsters(int wek);
	void setNumOfMonsters(int num);
	void initMonsters(int num_of_monsters);
	int getMonsterPositionY(int index);
	int getMonsterPositionX(int index);
	int getMonsterBoundsX();
	int getMonsterBoundsY();
	int getMonsterDirection(int index);
	int getMonsterMove(int index);
	int getMonsterMaxMove(int index);
	void setMonsterMove(int index, int move);
	void setMonsterDirection(int index, int dir);//1 right 0- left
	void clearList();
	void deleteMonster();
private:
	int keyframe;
	double frequency;
	SDL_Surface* animate();
	void monsterWalking();
	int num_of_monsters;
	monster_inf_t* monsters_list;
	SDL_Surface* monster;
	SDL_Surface* monster_l;
	int monster_bounds_x;
	int monster_bounds_y;
};